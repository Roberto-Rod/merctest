#include "testcasedockcontrollerdcpower.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseDockControllerDcPower::TestCaseDockControllerDcPower(QObject* parent) :
        TestCaseDockController(QString("DC Power"), parent)
    {
    }

    QString TestCaseDockControllerDcPower::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerDcPower::execute(bool manualSteps)
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

            // Power cycle DUT
            TEST_STEP(pHarness->powerCycle(),            "power cycle DUT");

            // Wait a second and then test input current
            QThread::sleep(1ul);
            uint16_t milliamps;
            TEST_STEP(pHarness->inputCurrent(milliamps), "measure DUT current");
            TEST_RESULT("Input Current", milliamps, kInputCurrentMin, kInputCurrentMax, "mA");

            retVal &= pDut->isConnected();

            // Clean up - attempt regardless of test state
            pHarness->enablePower(false);
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
