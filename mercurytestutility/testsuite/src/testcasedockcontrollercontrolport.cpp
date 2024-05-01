#include "testcasedockcontrollercontrolport.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "pacontrolmonitortest.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseDockControllerControlPort::TestCaseDockControllerControlPort(hardware::Port port, QObject* parent) :
        TestCaseDockController(QString("Control Port "), parent),
        m_port(port)
    {
        if (port == hardware::Port::A)      { m_name += "A (J2)"; }
        else if (port == hardware::Port::B) { m_name += "B (J6)"; }
    }

    QString TestCaseDockControllerControlPort::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerControlPort::execute(bool manualSteps)
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

            // Get a pointer to the PA control/monitor test jig object
            testjig::PaControlMonitorTest* paCtrlMonTest = pHarness->paCtrlMonTest(m_port);

            // Power cycle DUT & wait 1 second to allow FPGA to configure
            TEST_STEP(pHarness->powerCycle(),                            "power cycle DUT");
            if (KEEP_GOING) { QThread::sleep(1); }

            // Test comms by reading register with known content, use the drawing number as this should never change.
            uint16_t dwgNum;
            TEST_STEP(pDut->getDrawingNumber(dwgNum, m_port),            "get drawing number from DUT");
            TEST_BOOL("Read Drawing Number", (dwgNum == kExpectedDwgNum), "");

            // External blanking
            bool blank;
            TEST_STEP(pHarness->setBlankIn(m_port, false),               "de-assert (high) blanking input to control port from test jig");
            TEST_STEP(paCtrlMonTest->getPaBlankState(blank),             "get PA port blanking output state from test jig");
            TEST_BOOL("Blanking Input De-Asserted (high)", !blank, "");
            TEST_STEP(pHarness->setBlankIn(m_port, true),                "assert (low) blanking input to control port from test jig");
            TEST_STEP(paCtrlMonTest->getPaBlankState(blank),             "get PA port blanking output state from test jig");
            TEST_BOOL("Blanking Input Asserted (low)", blank, "");

            // Clean up - attempt regardless of test state
            pHarness->enablePower(false);
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
