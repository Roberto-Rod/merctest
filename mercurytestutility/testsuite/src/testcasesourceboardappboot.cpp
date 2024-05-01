#include "testcasesourceboardappboot.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"

#include <QFile>

namespace testsuite
{
    TestCaseSourceBoardAppBoot::TestCaseSourceBoardAppBoot(QObject* parent) :
        TestCaseSourceBoard("Boot Application", parent)
    {
    }

    QString TestCaseSourceBoardAppBoot::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardAppBoot::execute(bool manualSteps)
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

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            //----------------------------//

            // Power cycle the Source Board
            TEST_STEP(pHarness->powerCycle(kPowerCycleDelay), "power cycle DUT");

            // Initialise Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // If loading the FPGA image then read the version - any will pass - just log the result

            QString version;
            TEST_STEP(m_pSrcBrd->getFpgaVersion(version), "read FPGA version code");

            if (KEEP_GOING)
            {
                common::Logging::instance()->log("Read FPGA version code: " + version);
            }

            // Clean up - attempt these regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }  // retval

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }  // execute
} // testsuite
