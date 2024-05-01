#include "testcasesourceboarddcpower.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"

#include <QDebug>

namespace testsuite
{
    TestCaseSourceBoardDcPower::TestCaseSourceBoardDcPower(QObject* parent) :
        TestCaseSourceBoard("DC Power", parent)
    {
    }

    QString TestCaseSourceBoardDcPower::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDcPower::execute(bool manualSteps)
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

            // Get a pointer to the Test Harness object
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            uint16_t milliamps;

            // Disable power & enable shutdown control to DUT
            TEST_STEP(pHarness->enablePower(false),      "disable jig power to DUT");
            TEST_STEP(pHarness->driveShutdown(true),     "assert DUT shutdown");

            // Enable power to DUT & get shutdown current reading
            TEST_STEP(pHarness->enablePower(true),       "enable jig power to DUT");
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");

            TEST_RESULT("Shutdown Current", milliamps, kShutdownCurrentMin, kShutdownCurrentMax, "mA");

            // De-activate shutdown control to DUT & get power-up current reading
            TEST_STEP(pHarness->driveShutdown(false),    "de-assert DUT shutdown");
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");

            TEST_RESULT("Power-up Current", milliamps, kPowerupCurrentMin, kPowerupCurrentMax, "mA");

            // Initialise Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // Get initialised current reading
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");

            TEST_RESULT("Initialised Current", milliamps, kInitialisedCurrentMin, kInitialisedCurrentMax, "mA");

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
