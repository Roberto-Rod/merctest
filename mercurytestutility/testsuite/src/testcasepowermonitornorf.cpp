#include "testcasepowermonitornorf.hpp"
#include "powermonitor.hpp"
#include "testharnesspowermonitor.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"
#include "rfpath.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePowerMonitorNoRf::TestCasePowerMonitorNoRf(QObject* parent) :
        TestCasePowerMonitor("ADC Check (no RF)", parent)
    {
        // Register the test equipment that this Test Case uses. Although this is a no RF
        // test, we turn the signal generator RF output off to be sure it is not active,
        // so we need control of it.
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
    }

    QString TestCasePowerMonitorNoRf::setupMessage()
    {
        return QString();
    }

    bool TestCasePowerMonitorNoRf::execute(bool manualSteps)
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

            // Enable jig power & DUT
            TEST_STEP(harness->enablePower(true),            "enable 5.5V rail on test jig");
            TEST_STEP(harness->driveEnablePin(true),         "assert enable pin to DUT");

            TEST_STEP(te::TestEquipment::resetTe(teList()),  "reset test equipment");

            // Get a pointer to the power monitor object
            dut::PowerMonitor* powerMonitor = static_cast<dut::PowerMonitor*>(dut());

            // Get the start/stop/step frequencies and the test limit for the given band
            uint16_t testLimitNoRfFwdMin;
            uint16_t testLimitNoRfFwdMax;
            uint16_t testLimitNoRfRevMin;
            uint16_t testLimitNoRfRevMax;

            if (powerMonitor->band() == hardware::RfBand::LOW)
            {
                testLimitNoRfFwdMin = kLowBandNoRfFwdMin;
                testLimitNoRfFwdMax = kLowBandNoRfFwdMax;
                testLimitNoRfRevMin = kLowBandNoRfRevMin;
                testLimitNoRfRevMax = kLowBandNoRfRevMax;
            }
            else if (powerMonitor->band() == hardware::RfBand::MID)
            {
                testLimitNoRfFwdMin = kMidBandNoRfFwdMin;
                testLimitNoRfFwdMax = kMidBandNoRfFwdMax;
                testLimitNoRfRevMin = kMidBandNoRfRevMin;
                testLimitNoRfRevMax = kMidBandNoRfRevMax;
            }
            else if (powerMonitor->band() == hardware::RfBand::HIGH)
            {
                testLimitNoRfFwdMin = kHighBandNoRfFwdMin;
                testLimitNoRfFwdMax = kHighBandNoRfFwdMax;
                testLimitNoRfRevMin = kHighBandNoRfRevMin;
                testLimitNoRfRevMax = kHighBandNoRfRevMax;
            }

            else
            {
                common::Logging::instance()->log("ERROR: No valid Power Monitor band set");
                retVal = false;
            }

            uint16_t fwd, rev;
            TEST_STEP(pwrMon->getReading(fwd, dut::PowerMonitor::Direction::FORWARD), "get power monitor forward reading");
            TEST_STEP(pwrMon->getReading(rev, dut::PowerMonitor::Direction::REVERSE), "get power monitor reverse reading");

            TEST_RESULT("Forward Reading", fwd, testLimitNoRfFwdMin, testLimitNoRfFwdMax, "mV");
            TEST_RESULT("Reverse Reading", rev, testLimitNoRfRevMin, testLimitNoRfRevMax, "mV");

            TEST_STEP(harness->enablePower(false), "disable 5.5V rail on test jig");
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
