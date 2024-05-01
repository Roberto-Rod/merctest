#include "testcasesourceboardstoreddsfile.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "deviceundertest.hpp"
#include "settings.hpp"
#include "logging.hpp"
#include "messagedialog.hpp"
#include "fileselectdialog.hpp"

#include <QProcess>
#include <QThread>

namespace testsuite
{
   const QString  TestCaseSourceBoardStoreDdsFile::kBootloaderConfigData = "67 48 0a 00 01 08 03 4f 00 10";   // Osc disabled
   // const QString TestCaseSourceBoardFlashBootloader::kBootloaderConfigData = "67 48 0a 00 01 08 03 0f 00 10";   // Osc enabled

    TestCaseSourceBoardStoreDdsFile::TestCaseSourceBoardStoreDdsFile(QObject* parent) :
        TestCaseSourceBoard("Store DDS Cal to Flash", parent)
    {
    }

    QString  TestCaseSourceBoardStoreDdsFile::setupMessage()
    {
        return QString();
    }

    bool  TestCaseSourceBoardStoreDdsFile::execute(bool manualSteps)
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

            // Get a pointer to the SourceBoard object
            m_pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());
            retVal &= (m_pSrcBrd != 0);

            // Get a pointer to the Test Harness object
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            // Power cycle the DUT
            TEST_STEP(pHarness->powerCycle(1),                  "power cycle DUT");
            // Short delay after entering ISP mode
            QThread::msleep(500);

            // Do these without a TEST_STEP as they need to happen quickly - can't let user delay them
            if (KEEP_GOING && !m_pSrcBrd->connect())           { common::Logging::instance()->log("ERROR: Failed to connect to DUT"); retVal = false; }
            if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

            TEST_STEP(m_pSrcBrd->unlockBootloader(),       "unlock bootloader commands");

            TEST_STEP(m_pSrcBrd->sendBootloaderDdsStore(), "store dds cal file bootloader command");
        } // if (retval)
        // Set and log the status
        setStatus(retVal, testsPassed);
        return retVal;
     }// execute()
}  // namespace testsuite
