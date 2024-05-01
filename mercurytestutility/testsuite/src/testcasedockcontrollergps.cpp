#include "testcasedockcontrollergps.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "logging.hpp"
#include "actiondialog.hpp"

namespace testsuite
{
    TestCaseDockControllerGps::TestCaseDockControllerGps(QObject* parent) :
        TestCaseDockController(QString("GPS/1PPS"), parent)
    {
    }

    QString TestCaseDockControllerGps::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerGps::execute(bool manualSteps)
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

            // Power cycle DUT & wait 2 seconds to be sure that 1PPS present is cleared
            TEST_STEP(pHarness->powerCycle(), "power cycle DUT");
            if (KEEP_GOING)
            {
                QThread::sleep(2);

                QString title = "GPS Reception";
                QString msg   = "Please ensure that the active GPS antenna is plugged into J9 on the DUT<BR>"
                                "and that the GPS antenna has a clear view of the sky."
                                "<P>This dialog box will disappear when the GPS receiver achieves a lock<BR>"
                                "this should usually take less than one minute but could take several minutes.<BR>"
                                "<P>If no lock is achieved then press \"FAIL\" or press \"Cancel Test\" to stop all tests.";

                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),    this,  SLOT(getTestState()));
                connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for 1PPS to be detected");

                // Get the test state
                TEST_BOOL("1PPS Detected", dialog.status(), "");
            }

            // The 1PPS clock count can be retrieved via either port, the same information is reported on the two ports so
            // in these tests we just retrieve information from the default port (channel 1). If channel 1 comms fail
            // then these tests will fail.
            uint32_t count;
            TEST_STEP(pDut->getClockCount1Pps(count), "get 1PPS count from DUT");
            TEST_RESULT("1PPS Clock Count", count, kClkCountPpsMin, kClkCountPpsMax, "");

            // Clean up - attempt regardless of test state
            pHarness->enablePower(false);
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    void TestCaseDockControllerGps::getTestState()
    {
        // Get a pointer to the DUT
        dut::DockController* pDut = static_cast<dut::DockController*>(dut());

        // Get a pointer to the test harness
        testjig::TestHarnessDockController* pHarness = static_cast<testjig::TestHarnessDockController*>(pDut->testHarness());

        bool ppsPresent;
        bool jigCommsOk = pHarness->getPpsInPresent(ppsPresent);

        emit testState(jigCommsOk && ppsPresent);
    }
}
