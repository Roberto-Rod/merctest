#include "testcasepsumodulefans.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModuleFans::TestCasePsuModuleFans(QObject* parent) :
        TestCasePsuModule("Fan Control", parent)
    {
    }

    QString TestCasePsuModuleFans::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleFans::execute(bool manualSteps)
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

            // Initialise DUT
            TEST_STEP(pHarness->powerCycle(kPowerCycleDelay), "power cycle DUT");
            TEST_STEP(pPsu->connect(),                        "i2c reset/probe");

            TEST_STEP(pPsu->setFanManual(true),               "set fans to manual mode");
            TEST_STEP(pPsu->setFanEnable(true),               "enable fans");

            // Sleep for 1 second to make sure PWM is being driven and has been counted
            QThread::sleep(1ul);

            for (int i = 1; (i < 3) && KEEP_GOING; i++)
            {
                common::Logging::instance()->log(QString("Fan %1:").arg(i));
                {
                    // Indent log (automatically de-indented when object is destroyed)
                    common::LogIndent indent;

                    // Select fan enum
                    dut::PsuModule::Fan fan = dut::PsuModule::FAN_1;
                    if (i == 2) { fan = dut::PsuModule::FAN_2; }

                    // Test 12V supply
                    uint16_t millivolts;
                    TEST_STEP(pHarness->getFanVoltage(fan, millivolts), "get fan voltage from test jig");
                    TEST_RESULT("Supply Voltage", millivolts, k12VRailMin, k12VRailMax, "mV");

                    // Test PWM drive
                    uint16_t low, high;
                    uint8_t tacho;
                    TEST_STEP(pHarness->getFanPwmCtrl(fan, low, high),   "get fan PWM output from test jig");
                    TEST_RESULT("PWM Low Time", low, kPwmLowMin, kPwmLowMax, "µs");
                    TEST_RESULT("PWM High Time", high, kPwmHighMin, kPwmHighMax, "µs");

                    // Test tacho reading
                    TEST_STEP(pHarness->setFanPwmSense(fan, 750u, 750u), "set fan PWM sense input from test jig");
                    // Sleep for a second to give DUT time to average out the new reading
                    if (KEEP_GOING) { QThread::sleep(1ul); }
                    TEST_STEP(pPsu->readFanTacho(fan, tacho),            "read tacho from DUT");
                    TEST_RESULT("Tacho Reading", tacho, kTachoReadingMin, kTachoReadingMax, "");
                }
            }

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
