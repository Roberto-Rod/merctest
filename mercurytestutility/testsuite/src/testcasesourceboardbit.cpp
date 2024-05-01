#include "testcasesourceboardbit.hpp"
#include "sourceboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    // Set limits: voltage limits are nominal +/-10%
    // Current limits are based on ranges expected during automated tests.
    // Temperature limits are sensible values for lab/factory temperatures
    // and board temperature increase during automated tests.
    const dut::SourceBoardBit TestCaseSourceBoardBit::kBitLimitMin =
    {
        .m_railVin     = 21600,
        .m_rail1V2     = 1080,
        .m_rail1V8     = 1620,
        .m_rail3V3     = 2970,
        .m_rail5V5     = 4950,
        .m_current     = 70,
        .m_intTemp     = 15,
        .m_ddsTemp     = 15,
    };

    const dut::SourceBoardBit TestCaseSourceBoardBit::kBitLimitMax =
    {
        .m_railVin     = 26400,
        .m_rail1V2     = 1320,
        .m_rail1V8     = 1980,
        .m_rail3V3     = 3630,
        .m_rail5V5     = 6050,
        .m_current     = 300,
        .m_intTemp     = 75,
        .m_ddsTemp     = 85,
    };

    TestCaseSourceBoardBit::TestCaseSourceBoardBit(QObject* parent) :
        TestCaseSourceBoard("Built-In Test", parent)
    {
    }

    QString TestCaseSourceBoardBit::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardBit::execute(bool manualSteps)
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

            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // Get the BIT response from the Source Board
            dut::SourceBoardBit bit;
            TEST_STEP(m_pSrcBrd->getBit(bit), "get BIT response from DUT");

            // Log the individual results
            TEST_RESULT("Input Voltage", bit.m_railVin, kBitLimitMin.m_railVin, kBitLimitMax.m_railVin, "mV");
            TEST_RESULT("Input Current", bit.m_current, kBitLimitMin.m_current, kBitLimitMax.m_current, "mA");
            TEST_RESULT("1.2V Rail",     bit.m_rail1V2, kBitLimitMin.m_rail1V2, kBitLimitMax.m_rail1V2, "mV");
            TEST_RESULT("1.8V Rail",     bit.m_rail1V8, kBitLimitMin.m_rail1V8, kBitLimitMax.m_rail1V8, "mV");
            TEST_RESULT("3.3V Rail",     bit.m_rail3V3, kBitLimitMin.m_rail3V3, kBitLimitMax.m_rail3V3, "mV");
            TEST_RESULT("5.5V Rail",     bit.m_rail5V5, kBitLimitMin.m_rail5V5, kBitLimitMax.m_rail5V5, "mV");
            TEST_RESULT("DDS Temp",      bit.m_intTemp, kBitLimitMin.m_intTemp, kBitLimitMax.m_intTemp, "°C");
            TEST_RESULT("Internal Temp", bit.m_ddsTemp, kBitLimitMin.m_ddsTemp, kBitLimitMax.m_ddsTemp, "°C");

            // Clean up - attempt regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
