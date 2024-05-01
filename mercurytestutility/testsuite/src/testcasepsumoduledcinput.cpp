#include "testcasepsumoduledcinput.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModuleDcInput::TestCasePsuModuleDcInput(QObject* parent) :
        TestCasePsuModule("DC Input", parent)
    {
    }

    QString TestCasePsuModuleDcInput::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleDcInput::execute(bool manualSteps)
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

            // Get pointers to the DUT and the Test Harness
            dut::PsuModule* pPsu = static_cast<dut::PsuModule*>(dut());
            testjig::TestHarnessPsuModule* pHarness = static_cast<testjig::TestHarnessPsuModule*>(pPsu->testHarness());

            TEST_STEP(pHarness->powerCycle(kPowerCycleDelay),    "power cycle DUT");

            uint16_t milliamps;
            TEST_STEP(pHarness->inputCurrent(milliamps),         "read input current");

            TEST_RESULT("DC Input Power (PSU disabled)", milliamps, kDisabledCurrentMin, kDisabledCurrentMax, "mA");

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
