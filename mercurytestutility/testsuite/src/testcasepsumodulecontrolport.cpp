#include "testcasepsumodulecontrolport.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModuleControlPort::TestCasePsuModuleControlPort(QObject* parent) :
        TestCasePsuModule("Control Port", parent)
    {
    }

    QString TestCasePsuModuleControlPort::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleControlPort::execute(bool manualSteps)
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
            TEST_STEP(pHarness->powerCycle(kPowerCycleDelay),    "power cycle DUT");
            TEST_BOOL("I2C Probe", pPsu->connect(), "");

            // Read & log software version
            uint8_t major, minor;
            TEST_STEP(pPsu->getSwVersion(major, minor),          "read software version from DUT");
            if (KEEP_GOING)
            {
                common::Logging::instance()->log(QString("Firmware Version: %1.%2").arg(major)
                                                                                      .arg(minor));
            }

            // Read type and test against user selection for the DUT
            dut::PsuModuleType type;
            TEST_STEP(pPsu->getType(type),                       "read type from DUT");
            TEST_BOOL("Module Type", (pPsu->type() == type),     "");

            // Set fans up so that PSU thinks they are normal
            TEST_STEP(pPsu->setFanManual(true),                  "set fans to manual mode");
            TEST_STEP(pPsu->setFanEnable(true),                  "enable fans");
            TEST_STEP(pHarness->setFanPwmSense(dut::PsuModule::FAN_1,
                                                      750u, 750u), "set fan PWM sense input from test jig to normal");

            // Enable the fan 1 alarm
            TEST_STEP(pPsu->enableFan1Alarm(true),               "enable fan 1 alarm");

            bool alert;
            TEST_STEP(pHarness->getAlertState(alert),            "get alert state");
            TEST_BOOL("Alert de-asserted (high)", !alert, "");

            // Stop the fan PWM to generate an alert
            TEST_STEP(pHarness->setFanPwmSense(dut::PsuModule::FAN_1,
                                                      0u, 0u),     "set fan PWM sense input from test jig to stopped");

            // Sleep for 2 seconds to give DUT time to flag alert
            if (KEEP_GOING) { QThread::sleep(2ul); }

            TEST_STEP(pHarness->getAlertState(alert),            "get alert state");
            TEST_BOOL("Alert asserted (low)", alert, "");

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
