#include "testcasepsumodulethermalsensors.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModuleThermalSensors::TestCasePsuModuleThermalSensors(QObject* parent) :
        TestCasePsuModule("Thermal Sensors", parent)
    {
    }

    QString TestCasePsuModuleThermalSensors::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleThermalSensors::execute(bool manualSteps)
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
            TEST_STEP(pPsu->connect(),                           "i2c reset/probe");

            // Perform temperature sensor tests
            int8_t degC;
            TEST_STEP(pPsu->readTemperature(dut::PsuModule::TemperatureSource::LOCAL,      degC), "read local temperature");
            TEST_RESULT("Local Temperature Sensor", degC, kLocalTemperatureMin, kLocalTemperatureMax, "°C");

            TEST_STEP(pPsu->readTemperature(dut::PsuModule::TemperatureSource::REMOTE,     degC), "read remote temperature");
            TEST_RESULT("Remote Temperature Sensor", degC, kRemoteTemperatureMin, kRemoteTemperatureMax, "°C");

            TEST_STEP(pPsu->readTemperature(dut::PsuModule::TemperatureSource::THERMISTOR, degC), "read thermistor temperature");
            TEST_RESULT("Thermistor", degC, kThermistorTemperatureMin, kThermistorTemperatureMax, "°C");

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
