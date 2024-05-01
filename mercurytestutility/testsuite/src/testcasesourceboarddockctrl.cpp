#include "testcasesourceboarddockctrl.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseSourceBoardDockCtrl::TestCaseSourceBoardDockCtrl(hardware::Port port, QObject* parent) :
        TestCaseSourceBoard(QString("Dock Control Port "), parent),
        m_port(port)
    {
        if (port == hardware::Port::A)      { m_name += "A (PL2)"; }
        else if (port == hardware::Port::B) { m_name += "B (PL3)"; }
    }

    QString TestCaseSourceBoardDockCtrl::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDockCtrl::execute(bool manualSteps)
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

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            // Test RS485 Comms
            uint32_t data;
            TEST_STEP(m_pSrcBrd->readDockRegister(m_port, 0u, data), "dock rs485 read");
            TEST_BOOL("Dock RS485 Comms", (data == kDockRegisterExpected), "");

            // Test blanking control
            bool blankActive;

            TEST_STEP(m_pSrcBrd->setBlankIntMask(true),                 "enable source board blanking input as blanking source");

            TEST_STEP(pHarness->setBlankIn(false),                      "de-assert blanking input to source board");
            TEST_STEP(pHarness->getDockBlankState(m_port, blankActive), "read dock blanking input on test jig");
            TEST_BOOL("Blanking Output De-asserted", !blankActive, "");

            TEST_STEP(pHarness->setBlankIn(true),                       "assert blanking input to source board");
            TEST_STEP(pHarness->getDockBlankState(m_port, blankActive), "read dock blanking input on test jig");
            TEST_BOOL("Blanking Output Asserted", blankActive, "");

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
