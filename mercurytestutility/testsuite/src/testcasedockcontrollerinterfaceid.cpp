#include "testcasedockcontrollerinterfaceid.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseDockControllerInterfaceId::TestCaseDockControllerInterfaceId(QObject* parent) :
        TestCaseDockController("Interface ID", parent)
    {
    }

    QString TestCaseDockControllerInterfaceId::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerInterfaceId::execute(bool manualSteps)
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

            // Test interface ID
            uint8_t id;
            TEST_STEP(pHarness->getInterfaceId(id), "get interface ID from test jig");
            TEST_BOOL("Interface ID", (id == kExpectedIfId), QString("0x%1").arg(id, 1, 16, QChar('0')));

            // Clean up - attempt regardless of test state
            pHarness->enablePower(false);
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
