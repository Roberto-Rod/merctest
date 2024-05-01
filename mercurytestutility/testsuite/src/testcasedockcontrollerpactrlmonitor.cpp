#include "testcasedockcontrollerpactrlmonitor.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "pacontrolmonitor.hpp"
#include "pacontrolmonitortest.hpp"
#include "logging.hpp"

namespace testsuite
{
    const uint8_t TestCaseDockControllerPaCtrlMonitor::kPaCtrlExpectedRegisters[] = { 0x12, 0x34, 0x56, 0x78 };

    TestCaseDockControllerPaCtrlMonitor::TestCaseDockControllerPaCtrlMonitor(hardware::Port port, QObject* parent) :
        TestCaseDockController(QString("PA Control/Monitor Port "), parent),
        m_port(port)
    {
        if (port == hardware::Port::A)      { m_name += "A (J3)"; }
        else if (port == hardware::Port::B) { m_name += "B (J7)"; }
    }

    QString TestCaseDockControllerPaCtrlMonitor::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerPaCtrlMonitor::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Get a pointer to the DUT
            dut::DockController* pDut = static_cast<dut::DockController*>(dut());

            // Get a pointer to the test harness
            testjig::TestHarnessDockController* pHarness = static_cast<testjig::TestHarnessDockController*>(pDut->testHarness());

            // Get a pointer to the PA control/monitor object
            hardware::PaControlMonitor* pPaCtrlMon = pDut->paCtrlMonitor(m_port);

            // Get a pointer to the PA control/monitor test jig object
            testjig::PaControlMonitorTest* paCtrlMonTest = pHarness->paCtrlMonTest(m_port);

            // Power cycle DUT & wait 1 second
            TEST_STEP(pHarness->powerCycle(),                            "power cycle DUT");
            if (KEEP_GOING) { QThread::sleep(1); }

            bool en;
            TEST_STEP(paCtrlMonTest->getPwrMon5V5State(0u, en),          "read 5V5 state on Test Jig, pin 1");
            TEST_BOOL("Power Monitor 5V5 Enabled (Pin 1)", en, "");
            TEST_STEP(paCtrlMonTest->getPwrMon5V5State(1u, en),          "read 5V5 state on Test Jig, pin 2");
            TEST_BOOL("Power Monitor 5V5 Enabled (Pin 2)", en, "");

            TEST_STEP(pPaCtrlMon->setPwrMonEn(true),                     "assert Power Monitor enable (high) from DUT");
            TEST_STEP(paCtrlMonTest->getPwrMonEnState(en),               "read Power Monitor enable state on Test Jig");
            TEST_BOOL("Power Monitor Enable Asserted (Pin 7 High)", en,  "");
            TEST_STEP(pPaCtrlMon->setPwrMonEn(false),                    "de-assert Power Monitor enable (low) from DUT");
            TEST_STEP(paCtrlMonTest->getPwrMonEnState(en),               "read Power Monitor enable state on Test Jig");
            TEST_BOOL("Power Monitor Enable De-asserted (Pin 7 Low)", !en, "");

            bool shdn;
            TEST_STEP(pPaCtrlMon->setPaShutdown(true),                   "assert PA shutdown (high) from DUT");
            TEST_STEP(paCtrlMonTest->getPaShdnState(shdn),               "read PA shutdown on Test Jig");
            TEST_BOOL("PA Shutdown Asserted (Pin 13 High)", shdn,        "");
            TEST_STEP(pPaCtrlMon->setPaShutdown(false),                  "de-assert PA shutdown (low) from DUT");
            TEST_STEP(paCtrlMonTest->getPaShdnState(shdn),               "read PA shutdown on Test Jig");
            TEST_BOOL("PA Shutdown De-asserted (Pin 13 Low)", !shdn,     "");

            bool alert;
            TEST_STEP(paCtrlMonTest->setPaAlertState(true),              "assert PA alert (low) from Test Jig");
            TEST_STEP(pPaCtrlMon->getPaAlertState(alert),                "read PA alert state on DUT");
            TEST_BOOL("PA Alert Asserted (Pin 14 Low)", alert,           "");
            TEST_STEP(paCtrlMonTest->setPaAlertState(false),             "de-assert PA alert (high) from Test Jig");
            TEST_STEP(pPaCtrlMon->getPaAlertState(alert),                "read PA alert state on DUT");
            TEST_BOOL("PA Alert De-asserted (Pin 14 High)", !alert,      "");

            bool blank;
            TEST_STEP(pHarness->setBlankIn(m_port, false),               "disable blanking input to DUT");
            TEST_STEP(pPaCtrlMon->setFixedPaBlanking(true),              "assert PA blanking (low) from DUT");
            TEST_STEP(paCtrlMonTest->getPaBlankState(blank),             "read PA blanking state on Test Jig");
            TEST_BOOL("PA Blank Asserted (Pin 16 Low)", blank, "");
            TEST_STEP(pPaCtrlMon->setFixedPaBlanking(false),             "de-assert PA blanking (high) from DUT");
            TEST_STEP(paCtrlMonTest->getPaBlankState(blank),             "read PA blanking state on Test Jig");
            TEST_BOOL("PA Blank De-asserted (Pin 16 High)", !blank,      "");

            uint32_t data32;
            TEST_STEP(pPaCtrlMon->readPwrMonitor(data32),                "read PA monitor register");
            TEST_BOOL("Power Monitor SPI Read", (data32 == kPaMonitorExpected), "");

            uint8_t data8;
            for (uint8_t i = 0; (i < (sizeof(kPaCtrlExpectedRegisters) / sizeof(kPaCtrlExpectedRegisters[0]))) && KEEP_GOING; i++)
            {
                TEST_STEP(pPaCtrlMon->readPaController(i, data8), QString("read PA controller register %1").arg(i));
                TEST_BOOL(QString("PA Controller I2C Read, Reg %1").arg(i), (data8 == kPaCtrlExpectedRegisters[i]), "");
            }

            // Clean up - attempt regardless of test state
            pHarness->enablePower(false);
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
