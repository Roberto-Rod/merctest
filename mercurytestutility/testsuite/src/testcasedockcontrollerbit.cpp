#include "testcasedockcontrollerbit.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseDockControllerBit::TestCaseDockControllerBit(QObject* parent) :
        TestCaseDockController(QString("Built-In Test"), parent)
    {
    }

    QString TestCaseDockControllerBit::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerBit::execute(bool manualSteps)
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

            // Get a pointer to the DUT
            dut::DockController* pDut = static_cast<dut::DockController*>(dut());

            // Get a pointer to the test harness
            testjig::TestHarnessDockController* pHarness = static_cast<testjig::TestHarnessDockController*>(pDut->testHarness());

            // Power cycle DUT & wait 1 second to allow FPGA to configure
            TEST_STEP(pHarness->powerCycle(),                              "power cycle DUT");
            if (KEEP_GOING) { QThread::sleep(1); }

            // The BIT information can be retrieved via either port, the same information is reported on the two ports so
            // in these tests we just retrieve information from the default port (channel 1). If channel 1 comms fail
            // then these tests will fail.
            int8_t degC;
            uint16_t milliamps, millivolts, status;
            TEST_STEP(pDut->getBitCurrent(milliamps),                                            "read input current from DUT");
            TEST_RESULT("Input Current", milliamps, kInputCurrentMin, kInputCurrentMax, "mA");
            TEST_STEP(pDut->getBitVoltage(dut::DockController::BitVoltage::VOLTS_IN, millivolts),"read input voltage from DUT");
            TEST_RESULT("Input Voltage", millivolts, kInputVoltageMin, kInputVoltageMax, "mV");
            TEST_STEP(pDut->getBitVoltage(dut::DockController::BitVoltage::GPS_3V0, millivolts), "read internal 3V0 voltage from DUT");
            TEST_RESULT("Internal 3.0V Rail", millivolts, k3V0RailMin, k3V0RailMax, "mV");
            TEST_STEP(pDut->getBitVoltage(dut::DockController::BitVoltage::INT_3V3, millivolts), "read internal 3V3 voltage from DUT");
            TEST_RESULT("Internal 3.3V Rail", millivolts, k3V3RailMin, k3V3RailMax, "mV");
            TEST_STEP(pDut->getBitVoltage(dut::DockController::BitVoltage::INT_5V5, millivolts), "read internal 5V5 voltage from DUT");
            TEST_RESULT("Internal 5.5V Rail", millivolts, k5V5RailMin, k5V5RailMax, "mV");
            TEST_STEP(pDut->getBitTemperature(degC),                                             "read temperature from DUT");
            TEST_RESULT("Internal Temperature", degC, kDegCMin, kDegCMax, "°C");
            TEST_STEP(pDut->getBitStatus(status),                                                "read BIT status word from DUT");
            TEST_BOOL("BIT Status Word", (status == 0), "0x" + QString::number(status, 16));
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
