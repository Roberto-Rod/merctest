#include "testcasepsumoduleprogmicro.hpp"
#include "psumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "logging.hpp"
#include "manualtestdialog.hpp"

namespace testsuite
{
    TestCasePsuModuleProgMicro::TestCasePsuModuleProgMicro(QObject* parent) :
        TestCasePsuModule("Program Micro", parent)
    {
    }

    QString TestCasePsuModuleProgMicro::setupMessage()
    {
        return QString();
    }

    bool TestCasePsuModuleProgMicro::execute(bool manualSteps)
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

            // Enable power to DUT
            TEST_STEP(pHarness->enablePower(true),       "enable PSU supply on test jig");

            // Instruct user to program microcontroller and get status from user
            gui::ManualTestDialog dialog("Program Microcontroller",
                                          "Please load the latest \"Mercury Fan and PSU Controller Firmware\"<BR>"
                                          "(SW0011) image into the PIC microcontroller."
                                          "<P>Press PASS if the programmer reports success."
                                          "<P>Press FAIL if the programmer reports an error."
                                          "<P>Press Cancel to cancel all tests."
                                          "<P><B>Ensure programmer is disconnected from DUT after programming.</B>"
                                          "<P><I>Note: if latest firmware is already loaded then just press PASS.</I>");

            // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
            TEST_STEP(dialog.exec(), "prompt user to program FPGA");

            // Get the test state
            TEST_BOOL("Program FPGA", dialog.status(), "");

            // Disable power regardless of test state
            pHarness->enablePower(false);
        }

        // Set final status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
