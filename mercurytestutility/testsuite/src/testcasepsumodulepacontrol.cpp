#include "testcasepsumodulepacontrol.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePsuModulePaControl::TestCasePsuModulePaControl(QObject* parent) :
        TestCasePsuModule("PA Control", parent)
    {
    }

    QString TestCasePsuModulePaControl::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModulePaControl::execute(bool manualSteps)
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

            bool shdn;
            TEST_STEP(pPsu->setShutdownMode(dut::PsuModule::ShutdownMode::PA_SHDN_0),    "set DUT shutdown mode to fixed low");
            TEST_STEP(pHarness->getPaShutdownState(shdn),        "read shutdown state from test jig PA control port");
            TEST_BOOL("DUT PA Shutdown De-Asserted (Low)", !shdn, "");

            TEST_STEP(pPsu->setShutdownMode(dut::PsuModule::ShutdownMode::PA_SHDN_1),    "set DUT shutdown mode to fixed high");
            TEST_STEP(pHarness->getPaShutdownState(shdn),        "read shutdown state from test jig PA control port");
            TEST_BOOL("DUT PA Shutdown Asserted (High)", shdn, "");

            TEST_STEP(pPsu->setShutdownMode(dut::PsuModule::ShutdownMode::HOST_PA_SHDN), "set DUT shutdown mode to host control");
            TEST_STEP(pHarness->setPaShutdownState(true),        "assert shutdown (high) on test jig PSU control port");
            TEST_STEP(pHarness->getPaShutdownState(shdn),        "read shutdown state from test jig PA control port");
            TEST_BOOL("Host PA Shutdown Asserted (High)", shdn, "");

            TEST_STEP(pHarness->setPaShutdownState(false),       "de-assert shutdown (low) on test jig PSU control port");
            TEST_STEP(pHarness->getPaShutdownState(shdn),        "read shutdown state from test jig PA control port");
            TEST_BOOL("Host PA Shutdown De-Asserted (Low)", !shdn, "");

            bool mute;
            TEST_STEP(pHarness->setPaMuteState(true),            "assert mute (low) on test jig PSU control port");
            TEST_STEP(pHarness->getPaMuteState(mute),            "read mute state from test jig PA control port");
            TEST_BOOL("PA Mute Asserted (Low)", mute, "");
            TEST_STEP(pHarness->setPaMuteState(false),           "de-assert mute (high) on test jig PSU control port");
            TEST_STEP(pHarness->getPaMuteState(mute),            "read mute state from test jig PA control port");
            TEST_BOOL("PA Mute De-Asserted (High)", !mute, "");

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
