#include "testcasesourceboardcontrol.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseSourceBoardControl::TestCaseSourceBoardControl(QObject* parent) :
        TestCaseSourceBoard(QString("Control Port (PL6)"), parent)
    {
    }

    QString TestCaseSourceBoardControl::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardControl::execute(bool manualSteps)
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

            // Initialise Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            // Run the CAN bus test
            bool canOk = false;
            TEST_STEP(pHarness->sendCanTestPacket(canOk),    "send CAN test packet");
            TEST_BOOL("CAN Bus", canOk,                      "");

            bool ppsPresent;
            // Sleep for 3 seconds to allow 1PPS detection to settle (if power cycle just occurred)
            if (KEEP_GOING) { QThread::sleep(3u); }
            TEST_STEP(pHarness->getPpsInPresent(ppsPresent), "read 1PPS out (from DUT) present from Test Jig");
            TEST_BOOL("1PPS Out Present", ppsPresent, "");

            uint32_t count;
            TEST_STEP(pHarness->setPpsOutEnable(false),      "disable Test Jig 1PPS Out");
            // Sleep for 3 seconds to allow 1PPS detection to settle
            if (KEEP_GOING) { QThread::sleep(3u); }
            TEST_STEP(m_pSrcBrd->getClockCountPps(count),    "read 1PPS clock count from DUT");
            TEST_BOOL("Detect 1PPS In Not Present", (count == 0xFFFFFFFF), "");
            TEST_STEP(pHarness->setPpsOutEnable(true),       "enable Test Jig 1PPS Out");
            // Sleep for 3 seconds to allow 1PPS detection to settle
            if (KEEP_GOING) { QThread::sleep(3u); }
            TEST_STEP(m_pSrcBrd->getClockCountPps(count),    "read 1PPS clock count from DUT");
            TEST_BOOL("Detect 1PPS In Present", (count != 0xFFFFFFFF), "");

            bool blank;
            TEST_STEP(pHarness->setBlankIn(true),            "assert blank in");
            TEST_STEP(m_pSrcBrd->setBlankOutMask(true),      "set external blanking enable on DUT");
            TEST_STEP(pHarness->getExtBlankOutState(blank),  "read blank out state");
            TEST_BOOL("Blanking Output Asserted (low) with Blanking Input Asserted (low)", blank, "");
            TEST_STEP(pHarness->setBlankIn(false),           "de-assert blank in");
            TEST_STEP(pHarness->getExtBlankOutState(blank),  "read blank out state");
            TEST_BOOL("Blanking Output De-asserted (high) with Blanking Input De-asserted (high)", !blank, "");

            // Clean up - attempt regardless of test state
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
}
