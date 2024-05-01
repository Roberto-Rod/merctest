#include "testcasectsboarddcpower.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseCtsBoardDcPower::TestCaseCtsBoardDcPower(QObject* parent) :
        TestCaseCtsBoard("DC Power", parent)
    {
    }

    QString TestCaseCtsBoardDcPower::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardDcPower::execute(bool manualSteps)
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
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Get a pointer to the Test Harness object
            testjig::TestHarnessCtsBoard* pHarness(static_cast<testjig::TestHarnessCtsBoard*>(dut()->testHarness()));

            uint16_t milliamps;

            // Power-cycle DUT and get current reading
            TEST_STEP(pHarness->powerCycle(1),           "power-cycle DUT");
            QThread::sleep(2);
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");
            TEST_RESULT("Standby Current", milliamps, kStandbyCurrentMin, kStandbyCurrentMax, "mA");

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(),                "initialise DUT");

            // Enable RF chain and get current reading
            TEST_STEP(m_pCtsBrd->enableRfPower(true),    "enable power to RF section");
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");
            TEST_RESULT("RF Enabled Current", milliamps, kRfOnCurrentMin, kRfOnCurrentMax, "mA");
            TEST_STEP(m_pCtsBrd->enableRfPower(false),   "disable power to RF section");

            // Disable power and get current reading
            TEST_STEP(m_pCtsBrd->killDcPower(),          "command DUT to kill power");
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");
            TEST_RESULT("Shutdown Current", milliamps, kShutdownCurrentMin, kShutdownCurrentMax, "mA");

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
