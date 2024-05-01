#include "testcasepsumoduledcoutput.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModuleDcOutput::TestCasePsuModuleDcOutput(QObject* parent) :
        TestCasePsuModule("DC Output", parent)
    {
    }

    QString TestCasePsuModuleDcOutput::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleDcOutput::execute(bool manualSteps)
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
            TEST_STEP(pPsu->connect(),                           "i2c reset/probe");

            uint16_t milliamps, millivolts;

            common::Logging::instance()->log("PSU Output Disabled:");
            {
                // Indent log (automatically de-indented when object is destroyed)
                common::LogIndent indent;

                TEST_STEP(pHarness->outputVoltage(millivolts),       "read output voltage (PSU output off)");
                TEST_RESULT("Output Voltage", millivolts,
                             kDisabledVoltageOutMin, kDisabledVoltageOutMax, "mV");

                TEST_STEP(pHarness->inputCurrent(milliamps),         "read input current (PSU output off)");
                TEST_RESULT("Input Current", milliamps,
                             kDisabledCurrentInMin, kDisabledCurrentInMax, "mA");
            }

            common::Logging::instance()->log("PSU Output Enabled:");
            {
                // Indent log (automatically de-indented when object is destroyed)
                common::LogIndent indent;

                TEST_STEP(pPsu->setPsuEnable(true),                  "enable PSU output");

                if (KEEP_GOING) { QThread::sleep(kPsuEnPostDelay); }

                TEST_STEP(pHarness->outputVoltage(millivolts),       "read output voltage (PSU output on)");
                TEST_RESULT("Output Voltage", millivolts,
                             kEnabledVoltageOutMin, kEnabledVoltageOutMax, "mV");

                TEST_STEP(pHarness->outputCurrent(milliamps),        "read output current (PSU output on)");
                TEST_RESULT("Output Current", milliamps,
                             kEnabledCurrentOutMin, kEnabledCurrentOutMax, "mA");

                TEST_STEP(pHarness->inputCurrent(milliamps),         "read input current (PSU output on)");
                TEST_RESULT("Input Current", milliamps,
                             kEnabledCurrentInMin, kEnabledCurrentInMax, "mA");
            }

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
