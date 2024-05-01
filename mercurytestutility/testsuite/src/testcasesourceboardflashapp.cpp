#include "testcasesourceboardflashapp.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"
#include "fileselectdialog.hpp"

#include <QFile>

namespace testsuite
{
    TestCaseSourceBoardFlashApp::TestCaseSourceBoardFlashApp(common::FirmwareImage img, QObject* parent) :
        TestCaseSourceBoard("Load " + dut::SourceBoard::imageBriefDescription(img), parent),
        m_firmwareImage(img)
    {
    }

    QString TestCaseSourceBoardFlashApp::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardFlashApp::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.

        // Set to default value, set to true if in 'Smart Download' mode
        // and all firmware present.
        bool automaticSmartTestfail = true;

        dut::RunMethod runMethod = dut::RunMethod::AUTOMATIC_RUN;
        bool skipTest;

        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

        if (retVal)
        {
            QObject* ancestor = this;
            TestCase* topleveltestcase;
            topleveltestcase = dynamic_cast<TestCase *> (ancestor->parent());
            runMethod = topleveltestcase->getRunMode();

            if (runMethod == dut::RunMethod::AUTOMATIC_SMART)
            {
                //testsuite::TestCase * tptr = static_cast<testsuite::TestCase *> (dut()->topLevelTestCase());
                //runMethod =  tptr->getRunMode();
                skipTest = topleveltestcase->getSkipTestFlag();
                if (skipTest)
                {
                    automaticSmartTestfail = false;
                }
            }
        }

        if (automaticSmartTestfail)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            QString programmingBatchFile;
            if(KEEP_GOING)
            {
                programmingBatchFile = common::Settings::instance()->programmingScriptsDirectory() + "/progfw.bat";
            }

            m_pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());
            retVal &= (m_pSrcBrd != 0);

            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            // Power cycle the DUT
            TEST_STEP(pHarness->powerCycle(kPowerCycleOffDelay), "power cycle DUT");

            // Short delay after powering on and before sending FlashMagic commands
            QThread::msleep(kPowerCycleOnDelay);
            hardware::Jlink jl(kDevice);

            // Program and verify using programming batch file.
            TEST_STEP(jl.flashDevice(programmingBatchFile), "program flash");

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
