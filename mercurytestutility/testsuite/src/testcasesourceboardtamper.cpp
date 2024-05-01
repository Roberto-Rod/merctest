#include "testcasesourceboardtamper.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "registergpio.hpp"
#include "logging.hpp"
#include "actiondialog.hpp"
#include "delayprogressdialog.hpp"
#include "assyrevision.hpp"

#include <QThread>

namespace testsuite
{
    TestCaseSourceBoardTamper::TestCaseSourceBoardTamper(QObject* parent) :
        TestCaseSourceBoard(QString("Anti-Tamper (PL7 / U25)"), parent)
    {
    }

    QString TestCaseSourceBoardTamper::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardTamper::execute(bool manualSteps)
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

            // Get a pointer to the SourceBoard object
            m_pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());
            retVal &= (m_pSrcBrd != 0);

            int revision = m_pSrcBrd->assyRevision()->getUUTRevInt();
            bool debounceCircuit(revision >= static_cast<int>(common::AssyRevision::PCB0044Rev::RevJ));

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            // Power cycle the Source Board and break into bootloader
            // Do these without a TEST_STEP as they need to happen quickly - can't let user delay them
            if (KEEP_GOING && !m_pSrcBrd->connect())           { common::Logging::instance()->log("ERROR: Failed to connect to DUT"); retVal = false; }
            if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

            TEST_STEP(m_pSrcBrd->unlockBootloader(), "unlock bootloader commands");

            bool tamperState;

            // ******************************************************* //
            // ** First test tamper detection with board powered up ** //
            // ******************************************************* //
            if (KEEP_GOING)
            {
                common::Logging::instance()->log("Anti-Tamper Tests with Board Powered");
                common::LogIndent indent;

                 TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");

                // Reset tamper state - this tests anti-tamper IC comms
                TEST_STEP(m_pSrcBrd->resetTamperState(), "reset anti-tamper IC state");

                for (uint8_t pin = 0; pin < 3; pin++)
                {
                   // Test tamper switch is inactive
                   TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");
                   QThread::msleep(50);

                   TEST_STEP(m_pSrcBrd->clearTamperState(), "disable tamper and clear status");
                   TEST_STEP(m_pSrcBrd->blSetMechSwitchTamper(), "enable mech swich tamper using bootloader cmd");
                   TEST_STEP(m_pSrcBrd->getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState), "get tamper state from DUT");

                   TEST_BOOL("Tamper Switch Open", !tamperState, "");

                   //  The tamper enable is now partly done via a  bootloader cmd

                   TEST_STEP(m_pSrcBrd->clearTamperState(), "disable tamper and clear status");
                   TEST_STEP(m_pSrcBrd->blSetMechSwitchTamper(), "enable mech swich tamper using bootloader cmd");

                    QString pinStr = QString("%1").arg((pin + 1) * 2);  // Physical pins on PL7 are 2,4,6
                   TEST_STEP(pHarness->assertTamperInput(pin), QString ("assert tamper on PL7 pin %1").arg(pinStr));

                   QThread::msleep(300);

                   TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");

                   TEST_STEP(m_pSrcBrd->getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH,tamperState), "get tamper state from DUT");
                   TEST_BOOL(QString("Tamper Switch Closed (300 ms)  (PL7 pin %1)").arg(pinStr),tamperState, "");

                   // allow capacitor to charge up again
                   QThread::msleep(2000);
                   TEST_STEP(m_pSrcBrd->clearTamperState(), "disable tamper and clear status");

                   // If debounce circuitry present add debounce test.
                   if(debounceCircuit)
                   {
                       TEST_STEP(m_pSrcBrd->blSetMechSwitchTamper(), "enable mech swich tamper using bootloader cmd");

                       TEST_STEP(pHarness->assertTamperInput(pin), QString ("assert tamper on PL7 pin %1").arg(pinStr));

                       QThread::msleep(180);

                       TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");

                       TEST_STEP(m_pSrcBrd->getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState), "get tamper state from DUT");
                       TEST_BOOL(QString("Tamper Switch Closed (180 ms)  (PL7 pin %1)").arg(pinStr), !tamperState, "");

                       // allow capacitor to charge up again
                       QThread::msleep(2000);

                       // end of debounce test
                   }
                }

                // Use an action dialog to get the user to cover the ALS
                if (KEEP_GOING)
                {
                    QString title = "Ambient Light Sensor";
                    QString msg   = "Please fully cover Ambient Light Sensor (TR6) e.g. using blu-tac.";

                    gui::ActionDialog dialog(title, msg);

                    connect(&dialog, SIGNAL(getTestState()),     this, SLOT(getTestStateUnilluminated()));
                    connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                    // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                    TEST_STEP(dialog.exec(), "wait for ambient light sensor to be covered");

                    // Get the test state
                    TEST_BOOL("Ambient Light Sensor Unilluminated", dialog.status(), "");
                }

                // Use an action dialog to get the user to uncover the ALS
                if (KEEP_GOING)
                {
                    QString title = "Ambient Light Sensor";
                    QString msg   = "Please uncover Ambient Light Sensor (TR6) and expose to normal room lighting.";

                    gui::ActionDialog dialog(title, msg);

                    connect(&dialog, SIGNAL(getTestState()),    this,  SLOT(getTestStateIlluminated()));
                    connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                    // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                    TEST_STEP(dialog.exec(), "wait for ambient light sensor to be uncovered");

                    // Get the test state
                    TEST_BOOL("Ambient Light Sensor Illuminated", dialog.status(), "");
                }
            }

            // ******************************************************* //
            // ** Now test tamper detection with board powered down ** //
            // ******************************************************* //
            if (KEEP_GOING)
            {
                common::Logging::instance()->log("Anti-Tamper Tests with Board Unpowered");
                common::LogIndent indent;

                // We've tested the two sources and all of the external input pins independtly,
                // just use the tamper switch source on pin 1 for the power-off test.

                // Test tamper switch is inactive
                TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");

                TEST_STEP(m_pSrcBrd->clearTamperState(), "disable tamper and clear status");
                TEST_STEP(m_pSrcBrd->blSetMechSwitchTamper(),            "enable mech swich tamper using bootloader cmd");
                TEST_STEP(m_pSrcBrd->getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState), "get tamper state from DUT");

                //TEST_STEP(m_pSrcBrd->getFreshTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState), "get tamper state from DUT");
                TEST_BOOL("Tamper Switch Open Before Power Down", !tamperState, "");

                // Start of debounce test
                if(debounceCircuit)
                {
                    // Power-off the DUT
                    TEST_STEP(m_pSrcBrd->testHarness()->enablePower(false), "disable jig power to DUT");

                    // Leave DUT off for specified time
                    gui::DelayProgressDialog progress("Waiting whilst Source Board is powered-off",
                                                   static_cast<int32_t>(kPowerCycleDelayTamper) * 1000u);

                    // Assert the tamper pin whilst the DUT is off (short pulse)
                    TEST_STEP(pHarness->assertTamperInput(0u), "assert tamper on PL7, pin 2");

                    QThread::msleep(150);

                    // De-assert all the tamper inputs
                    TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");

                    // alow capacitor to charge up again
                    QThread::msleep(2000);

                    // Power the DUT back up
                    TEST_STEP(m_pSrcBrd->testHarness()->enablePower(true), "re-enable jig power to DUT");

                    // Break into the bootloader again
                    if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

                    TEST_STEP(m_pSrcBrd->unlockBootloader(), "unlock bootloader commands");

                    // Test tamper switch was detected active whilst DUT was off
                    TEST_STEP(m_pSrcBrd->getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState), "get tamper state from DUT");

                    // Test result not looged  until hardware modified
                    TEST_BOOL(QString("Tamper Switch Closure Detected During Power Down (150ms)"),!tamperState, "");
                    }//End of debounce test

                // Power-off the DUT
                TEST_STEP(m_pSrcBrd->testHarness()->enablePower(false), "disable jig power to DUT");

                // Leave DUT off for specified time (long pulse)
                gui::DelayProgressDialog progress2("Waiting whilst Source Board is powered-off",
                                                   static_cast<int32_t>(kPowerCycleDelayTamper) * 1000u);

                // Assert the tamper pin whilst the DUT is off
                TEST_STEP(pHarness->assertTamperInput(0u), "assert tamper on PL7, pin 2");

                 QThread::msleep(300);

                // De-assert all the tamper inputs
                TEST_STEP(pHarness->deassertTamperInputs(), "de-assert all tamper inputs from jig to DUT");

                // Power the DUT back up
                TEST_STEP(m_pSrcBrd->testHarness()->enablePower(true), "re-enable jig power to DUT");

                // Break into the bootloader again
                if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

                TEST_STEP(m_pSrcBrd->unlockBootloader(), "unlock bootloader commands");

                // Test tamper switch was detected active whilst DUT was off
                TEST_STEP(m_pSrcBrd->getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState), "get tamper state from DUT");

                // Test result not looged  until hardware modified
                TEST_BOOL(QString("Tamper Switch Closure Detected During Power Down (300ms)"), tamperState, "");

                // Check the oscillator is disabled see  TestCaseSourceBoardFlashBootloader::kBootloaderConfigData
                // for enabling / disabling by bootloader
                bool oscState;
                TEST_STEP (m_pSrcBrd->getOscillatorDisabledState(oscState), "get oscillator disabled state from DUT");
                TEST_BOOL("Tamper oscillator disabled ", oscState, "");
            }

            TEST_STEP(m_pSrcBrd->clearTamperState(), "disable tamper and clear status");

            // Exit bootloader to make it reload the standard firmware
            TEST_STEP(m_pSrcBrd->exitBootloader(), "exit bootloader (reload standard firmware)");

            // Clean up - attempt these regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    void TestCaseSourceBoardTamper::getTestStateUnilluminated()
    {
        // Test ambient light sensor is unilluminated
        bool state;
        m_pSrcBrd->getFreshTamperState(dut::SourceBoard::TamperSource::LIGHT_SENSOR, state);

        emit testState(!state);
    }

    void TestCaseSourceBoardTamper::getTestStateIlluminated()
    {
        // Test ambient light sensor is illuminated
        bool state;
        m_pSrcBrd->getFreshTamperState(dut::SourceBoard::TamperSource::LIGHT_SENSOR, state);

        emit testState(state);
    }
}
