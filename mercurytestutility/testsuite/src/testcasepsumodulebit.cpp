#include "testcasepsumodulebit.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModuleBit::TestCasePsuModuleBit(QObject* parent) :
        TestCasePsuModule("Built-In Test", parent)
    {
    }

    QString TestCasePsuModuleBit::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleBit::execute(bool manualSteps)
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

            // Disabled state tests
            if (KEEP_GOING)
            {
                common::Logging::instance()->log("PSU Output & Fan 12V Disabled:");

                // Indent log (automatically de-indented when object is destroyed)
                common::LogIndent indent;

                uint16_t millivolts, milliamps;
                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::INPUT,    millivolts), "read input voltage from DUT");
                TEST_RESULT("Input Voltage", millivolts, kVoltageInMin, kVoltageInMax, "mV");

                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::OUTPUT,   millivolts), "read output voltage from DUT");
                TEST_RESULT("Output Voltage", millivolts, kDisabledVoltageOutMin, kDisabledVoltageOutMax, "mV");

                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::RAIL_3V3, millivolts), "read 3V3 rail voltage from DUT");
                TEST_RESULT("3.3V Rail", millivolts, k3V3RailMin, k3V3RailMax, "mV");

                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::RAIL_12V, millivolts), "read 12V rail voltage from DUT");
                TEST_RESULT("12V Rail", millivolts, kDisabled12VRailMin, kDisabled12VRailMax, "mV");

                TEST_STEP(pPsu->readCurrent(milliamps),                                           "read output current from DUT");
                TEST_RESULT("Output Current", milliamps, kDisabledCurrentOutMin, kDisabledCurrentOutMax, "mA");
            }

            if (KEEP_GOING) { QThread::sleep(kPsuEnPostDelay); }

            // Enabled state tests
            if (KEEP_GOING)
            {
                common::Logging::instance()->log("PSU Output & Fan 12V Enabled:");

                // Indent log (automatically de-indented when object is destroyed)
                common::LogIndent indent;

                TEST_STEP(pPsu->setPsuEnable(true), "enable PSU output");
                TEST_STEP(pPsu->setFanManual(true), "set fans to manual mode");
                TEST_STEP(pPsu->setFanEnable(true), "enable fans");

                uint16_t millivolts, milliamps;
                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::INPUT,    millivolts), "read input voltage from DUT");
                TEST_RESULT("Input Voltage", millivolts, kVoltageInMin, kVoltageInMax, "mV");

                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::OUTPUT,   millivolts), "read output voltage from DUT");
                TEST_RESULT("Output Voltage", millivolts, kEnabledVoltageOutMin, kEnabledVoltageOutMax, "mV");

                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::RAIL_3V3, millivolts), "read 3V3 rail voltage from DUT");
                TEST_RESULT("3.3V Rail", millivolts, k3V3RailMin, k3V3RailMax, "mV");

                TEST_STEP(pPsu->readVoltage(dut::PsuModule::VoltageSource::RAIL_12V, millivolts), "read 12V rail voltage from DUT");
                TEST_RESULT("12V Rail", millivolts, kEnabled12VRailMin, kEnabled12VRailMax, "mV");

                TEST_STEP(pPsu->readCurrent(milliamps),                                           "read output current from DUT");
                TEST_RESULT("Output Current", milliamps, kEnabledCurrentOutMin, kEnabledCurrentOutMax, "mA");
            }

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
