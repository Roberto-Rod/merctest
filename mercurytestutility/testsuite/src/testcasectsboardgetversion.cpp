#include "testcasectsboardgetversion.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseCtsBoardGetVersion::TestCaseCtsBoardGetVersion(QObject* parent) :
        TestCaseCtsBoard("Get Firmware Version", parent)
    {
    }

    QString TestCaseCtsBoardGetVersion::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardGetVersion::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed(true);

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal(TestCase::execute());

        if (retVal)
        {
            QString version;
            bool ok(false);

            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "initialise DUT");

            TEST_STEP((ok = m_pCtsBrd->getVersion(version)), "read version code from DUT");

            if (ok)
            {
                common::Logging::instance()->log(QString("Firmware Version: %1").arg(version));
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
}
