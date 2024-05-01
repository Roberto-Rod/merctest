#include "testcasesourceboardgpio.hpp"
#include "sourceboard.hpp"
#include "testharness.hpp"
#include "registergpio.hpp"
#include "gpiotester.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseSourceBoardGpio::TestCaseSourceBoardGpio(QObject* parent) :
        TestCaseSourceBoard(QString("GPIO Port (PL1)"), parent)
    {
    }

    QString TestCaseSourceBoardGpio::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardGpio::execute(bool manualSteps)
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

            // Initialise Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // Create Register GPIO objects for each of the Jig and the DUT
            hardware::RegisterGpio jigGpio(m_pSrcBrd->testHarness()->m_pJigRegs, kJigGpioBaseAddr, kGpioWidth);
            hardware::RegisterGpio dutGpio(m_pSrcBrd->dutRegs(), kDutGpioBaseAddr, kGpioWidth);

            // Create a GPIO tester object
            hardware::GpioTester tester(jigGpio, dutGpio);

            // And run it (as input and output)
            TEST_STEP(tester.execute(testsPassed), "run GPIO tests");

            // Clean up - attempt these regardless of test state
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
