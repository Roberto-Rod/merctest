#include "testcasemidbandsourcemoduleid.hpp"
#include "daughtermodule.hpp"
#include "sourceboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseMidBandSourceModuleId::TestCaseMidBandSourceModuleId(QObject* parent) :
        TestCaseSourceBoard("Daughter ID", parent)
    {
    }

    QString TestCaseMidBandSourceModuleId::setupMessage()
    {
        return QString();
    }

    bool TestCaseMidBandSourceModuleId::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

        dut::DaughterModule* pDut = static_cast<dut::DaughterModule*>(dut());

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise the Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            uint8_t id, expectedId;
             if(pDut->isMidBand())
             {
                 expectedId = kExpectedIdMB;
             }
             else
             {
                  expectedId = kExpectedIdHB;
             }

            TEST_STEP(m_pSrcBrd->getDaughterId(id), "read daughter id");
            TEST_BOOL("Daughter Board ID", (id == expectedId), "0x" + QString::number(id, 16).toUpper());

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
