#include "testcasepowermonitordcpower.hpp"
#include "powermonitor.hpp"
#include "testharnesspowermonitor.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePowerMonitorDcPower::TestCasePowerMonitorDcPower(QObject* parent) :
        TestCasePowerMonitor("DC Power", parent)
    {
    }

    QString TestCasePowerMonitorDcPower::setupMessage()
    {
        return QString();
    }

    bool TestCasePowerMonitorDcPower::execute(bool manualSteps)
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
            dut::PowerMonitor* pwrMon = static_cast<dut::PowerMonitor*>(dut());
            testjig::TestHarnessPowerMonitor* harness = static_cast<testjig::TestHarnessPowerMonitor*>(pwrMon->testHarness());

             uint16_t   currActiveMin = 0,   currActiveMax = 0;
             if (pwrMon->band() == hardware::RfBand::LOW  ||   pwrMon->band() == hardware::RfBand::MID)
             {
                 currActiveMin = kCurrActiveMin;
                 currActiveMax = kCurrActiveMax;
             }
             else if (pwrMon->band() == hardware::RfBand::HIGH)
             {
                 currActiveMin = kCurrActiveMinHB;
                 currActiveMax = kCurrActiveMaxHB;
             }

            uint16_t milliamps;

            // Enable jig power
            TEST_STEP(harness->enablePower(true),       "enable 5.5V rail on test jig");
            TEST_STEP(harness->driveEnablePin(false),   "de-assert enable pin to DUT");
            TEST_STEP(harness->inputCurrent(milliamps), "read input current");

            // ADC voltage rail is determined by the standby current
            // Need to determine voltage rail before getting max and min current values.
            pwrMon->setAdcVariant(milliamps, pwrMon->band());
            hardware::RfBand::Band band = pwrMon->band();
            TEST_RESULT("Input Current (Standby)", milliamps, pwrMon->getMinStandbyCurrent(band), pwrMon->getMaxStandbyCurrent(band), "mA");

            TEST_STEP(harness->driveEnablePin(true),    "assert enable pin to DUT");
            TEST_STEP(harness->inputCurrent(milliamps), "read input current");

            TEST_RESULT("Input Current (Active, no RF)", milliamps, currActiveMin, currActiveMax, "mA");

            TEST_STEP(harness->enablePower(false),      "disable 5.5V rail on test jig");
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
