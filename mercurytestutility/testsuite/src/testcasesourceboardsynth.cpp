#include "testcasesourceboardsynth.hpp"
#include "sourceboard.hpp"
#include "logging.hpp"

#include <QThread>

namespace testsuite
{
    TestCaseSourceBoardSynth::TestCaseSourceBoardSynth(QObject* parent) :
        TestCaseSourceBoard("Synthesizer", parent)
    {
    }

    QString TestCaseSourceBoardSynth::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardSynth::execute(bool manualSteps)
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
            TEST_STEP(initialiseSrcBrd(),           "initialise DUT");

            TEST_STEP(m_pSrcBrd->initialiseSynth(), "initialise synthesizer");

            // Let the count settle for 3 seconds
            QThread::sleep(3ul);

            uint32_t count;

            TEST_STEP(m_pSrcBrd->getDdsSyncClkCnt(count), "get DDS clock count");

            // Did they pass? Calculate the count error
            uint32_t expected = m_pSrcBrd->expectedDdsSyncClkCnt();
            uint32_t error;

            if (count > expected) { error = count - expected; }
            else                  { error = expected - count; }

            // Test and log result
            TEST_RESULT("DDS Clock Count Error", error, 0u, kSyncClkCountMaxErr, "");

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
}
