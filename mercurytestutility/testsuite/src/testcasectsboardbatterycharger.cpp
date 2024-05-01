#include "testcasectsboardbatterycharger.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"
#include "actiondialog.hpp"

namespace testsuite
{
    TestCaseCtsBoardBatteryCharger::TestCaseCtsBoardBatteryCharger(QObject* parent) :
        TestCaseCtsBoard("Battery Charger", parent)
    {
    }

    QString TestCaseCtsBoardBatteryCharger::setupMessage()
    {
        return QString("<B>Battery Charger Test Setup:</B>"
                       "<P>1. Remove CAB0366 from battery terminals."
                       "<P>2. Install battery into the DUT battery holder."
                       "<P>3. Connect Test Jig \"PSU J1\" connector to DUT J4 using CAB0077 + CAB0362.");
    }

    bool TestCaseCtsBoardBatteryCharger::execute(bool manualSteps)
    {
        const auto kCharging(dut::CtsBoard::ChargeStatus::Charging);
        const auto kNearlyComplete(dut::CtsBoard::ChargeStatus::NearlyComplete);
        const auto kComplete(dut::CtsBoard::ChargeStatus::Complete);

        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed(true);

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal(TestCase::execute());

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            uint16_t millivolts;
            dut::CtsBoard::ChargeStatus status;
            bool fault;
            bool present;

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "initialise DUT");

            // Test that we detect charger present
            TEST_STEP(m_pCtsBrd->getUsbPowerStatus(present), "get charger present status");
            TEST_BOOL("Charger Present", present, "");

            // USB power is enabled, charge status pin should indicate one of the charging states
            TEST_STEP(m_pCtsBrd->getChargeStatus(status), "get charge status (DUT powered from battery + USB charger)");
            TEST_BOOL("Charge Status Pin Charging or Nearly Complete (with Charger)", (status == kCharging) || (status == kNearlyComplete), "");

            // Disable USB power, test that we detect charger present
            TEST_STEP(true, "turn power off to DUT USB port");
            m_pCtsBrd->testHarness()->powerOff();
            QThread::sleep(1);
            TEST_STEP(m_pCtsBrd->getUsbPowerStatus(present), "get charger present status");
            TEST_BOOL("Charger Not Present", !present, "");

            // Charge status pin should indicate "complete" (this also tests that DUT is still running)
            // Note that the charge status pin goes to the "complete" state but the CTS knows that the charger is not
            // connected and so the overall battery state would not be "charging complete".
            TEST_STEP(m_pCtsBrd->getChargeStatus(status), "get charge status (DUT powered from battery)");
            TEST_BOOL("Charge Status Pin Complete (no Charger)", status == kComplete, "");

            // Enable RF section to apply higher load to battery, run until battery voltage <= kDischargeVoltage_mV
            TEST_STEP(m_pCtsBrd->enableRfPower(true), "enable DUT RF section");
            QThread::sleep(1);

            // Inform user of status via action dialog
            if (KEEP_GOING)
            {
                QString title("Partial Battery Discharge");
                gui::ActionDialog dialog(title, "");

                connect(&dialog, SIGNAL(getTestState()),            this, SLOT(getTestStateDischarge()));
                connect(   this, SIGNAL(testState(bool)),        &dialog, SLOT(testState(bool)));
                connect(   this, SIGNAL(updateMessage(QString)), &dialog, SLOT(updateMessage(QString)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for battery partial discharge");

                // Get the test state
                TEST_BOOL("Battery Reached Discharge Voltage", dialog.status(), "");
            }

            // Disable RF section
            TEST_STEP(m_pCtsBrd->enableRfPower(false), "disable DUT RF section");

            // Enable USB power, battery charging voltage should be in range kChargeVoltageMin_mV to kChargeVoltageMax_mV
            m_pCtsBrd->testHarness()->powerOn();
            QThread::sleep(2);
            TEST_STEP(m_pCtsBrd->getBatteryVoltage(millivolts), "get battery voltage");
            TEST_RESULT("Charge Voltage", millivolts, kChargeVoltageMin_mV, kChargeVoltageMax_mV, "mV");

            // Test fault indication, should be no fault to start with
            TEST_STEP(m_pCtsBrd->getChargerFaultStatus(fault), "get battery fault status");
            TEST_BOOL("Battery Fault: No Fault", !fault, "");

            // Instruct user to spray freezer spray at NTC, should then see fault
            if (KEEP_GOING)
            {
                QString title("Battery Fault - Cold");
                QString msg("Spray a short burst of freezer spray at the thermistor device; R96");
                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),            this, SLOT(getTestStateFault()));
                connect(   this, SIGNAL(testState(bool)),        &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for battery fault");

                // Get the test state
                TEST_BOOL("Battery Fault: Cold Fault", dialog.status(), "");
            }

            // Clean up - attempt regardless of test state
            if (m_pCtsBrd)
            {
                // Disconnect DUT serial port
                m_pCtsBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    void TestCaseCtsBoardBatteryCharger::getTestStateDischarge()
    {
        uint16_t millivolts;
        bool ok(m_pCtsBrd->getBatteryVoltage(millivolts));
        emit updateMessage(QString("Waiting until battery voltage <= %1 mV\r\nCurrently: %2 mV").arg(kDischargeVoltage_mV)
                                                                                                .arg(millivolts));
        emit testState(ok && (millivolts <= kDischargeVoltage_mV));
    }

    void TestCaseCtsBoardBatteryCharger::getTestStateFault()
    {
        // Expecting to see fault status so emit true when fault is true
        bool fault;
        bool ok(m_pCtsBrd->getChargerFaultStatus(fault) );
        emit testState(ok && fault);
    }
}
