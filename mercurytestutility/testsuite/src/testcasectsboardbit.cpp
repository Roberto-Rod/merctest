#include "testcasectsboardbit.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseCtsBoardBit::TestCaseCtsBoardBit(QObject* parent) :
        TestCaseCtsBoard("Built In Test", parent)
    {
    }

    QString TestCaseCtsBoardBit::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardBit::execute(bool manualSteps)
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

            uint16_t millivolts;
            int16_t degreesC;
            dut::CtsBoard::BitStatus status;

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "initialise DUT");

            // Get BIT status flags
            TEST_STEP(m_pCtsBrd->getBitStatus(status), "get BIT flags");
            TEST_BOOL("BIT Flag: Processor Initialisation", status.m_procInitOk, "");
            TEST_BOOL("BIT Flag: Board Support Package", status.m_bspOk, "");
            TEST_BOOL("BIT Flag: RF Synthesizer", status.m_synthOk, "");
            TEST_BOOL("BIT Flag: Internal Flash", status.m_internalFlashOk, "");
            TEST_BOOL("BIT Flag: Flash Mission File", status.m_flashMissionFileOk, "");
            TEST_BOOL("BIT Flag: Anti-Tamper Device", status.m_tamperDeviceOk, "");

            // Get BIT voltage/temperature readings
            TEST_STEP(m_pCtsBrd->getTamperVoltage(millivolts), "get anti-tamper voltage");
            TEST_RESULT("Anti-Tamper Voltage", millivolts, kTamperVoltageMin_mV, kTamperVoltageMax_mV, "mV");

            TEST_STEP(m_pCtsBrd->getProcessorTemperature(degreesC), "get processor temperature");
            TEST_RESULT("Processor Temperature", degreesC, kProcessorTemperatureMin_degC, kProcessorTemperatureMax_degC, "°C");

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
