#include "testcasectsboardflashfirmware.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"
#include "fileselectdialog.hpp"

#include <QFile>

namespace testsuite
{
    TestCaseCtsBoardFlashFirmware::TestCaseCtsBoardFlashFirmware(QObject* parent) :
        TestCaseCtsBoard("Load Firmware Image", parent)
    {
    }

    QString TestCaseCtsBoardFlashFirmware::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardFlashFirmware::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.

        // Set to default value, set to true if in 'Smart Download' mode
        // and all firmware present.
        bool automaticSmartTestFail(true);
        bool testsPassed(true);
        dut::RunMethod runMethod(dut::RunMethod::AUTOMATIC_RUN);

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal(TestCase::execute());

        if (retVal)
        {
            QObject* ancestor(this);
            TestCase* topLevelTestCase(dynamic_cast<TestCase*> (ancestor->parent()));
            runMethod = topLevelTestCase->getRunMode();

            if (runMethod == dut::RunMethod::AUTOMATIC_SMART)
            {
                if (topLevelTestCase->getSkipTestFlag())
                {
                    automaticSmartTestFail = false;
                }
            }
        }

        if (automaticSmartTestFail)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            QString programmingBatchFile;
            if(KEEP_GOING)
            {
                programmingBatchFile = common::Settings::instance()->programmingScriptsDirectory() + "/progcts.bat";
            }

            m_pCtsBrd = dynamic_cast<dut::CtsBoard*>(dut());
            retVal = retVal && m_pCtsBrd;

            testjig::TestHarnessCtsBoard* pHarness = static_cast<testjig::TestHarnessCtsBoard*>(dut()->testHarness());

            // Power cycle the DUT
            TEST_STEP(pHarness->powerCycle(kPowerCycleOffDelay), "power cycle DUT");

            // Short delay after powering on and before sending J-Link commands
            QThread::msleep(kPowerCycleOnDelay);
            hardware::Jlink jl(kDevice);

            // Program and verify using programming batch file.
            TEST_STEP(jl.flashDevice(programmingBatchFile), "program flash");

            // Clean up - attempt these regardless of test state
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

