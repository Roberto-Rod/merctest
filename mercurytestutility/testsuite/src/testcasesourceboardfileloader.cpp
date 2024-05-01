#include "testcasesourceboardfileloader.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"
#include "fileselectdialog.hpp"

#include <QFile>

namespace testsuite
{
    TestCaseSourceBoardFileLoader::TestCaseSourceBoardFileLoader(common::FirmwareImage img, QObject* parent) :
        TestCaseSourceBoard("Load " + dut::SourceBoard::imageBriefDescription(img), parent),
        m_firmwareImage(img)
    {
    }

    QString TestCaseSourceBoardFileLoader::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardFileLoader::execute(bool manualSteps)
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
            TestCase * topleveltestcase;
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

            // Get the input file from the user
            gui::FileSelectDialog dialog(dut::SourceBoard::imageFullDescription(m_firmwareImage),
                                          common::Settings::instance()->firmwareImageFileName(m_firmwareImage));

            if (dialog.exec())
            {
                common::Settings::instance()->setFirmwareImageFileName(dialog.fileName(), m_firmwareImage);
            }
            else
            {
                retVal = false;
            }

            // Get a pointer to the SourceBoard object
            m_pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());
            retVal &= (m_pSrcBrd != 0);

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            // Get firmware image filename
            QString fileName = common::Settings::instance()->firmwareImageFileName(m_firmwareImage);
            QString destName = dut::SourceBoard::imageDestFileName(m_firmwareImage);
            QFile file;

            if (KEEP_GOING)
            {
                if (fileName.isEmpty())
                {
                    common::Logging::instance()->log("ERROR: could not find input file");
                    retVal = false;
                }
                else if (destName.isEmpty())
                {
                    common::Logging::instance()->log("ERROR: could not get destination file name");
                    retVal = false;
                }
                else
                {
                    file.setFileName(fileName);
                    QFileInfo info(file);
                    common::Logging::instance()->log("Transferring file: " + info.fileName());
                }
            }

            TEST_STEP(file.exists(),                            "check firmware image file exists");

            // Power cycle the Source Board and break into bootloader
            TEST_STEP(pHarness->powerCycle(kPowerCycleDelay), "power cycle DUT & \"break in\" to bootloader");

            // Do these without a TEST_STEP as they need to happen quickly - can't let user delay them
            if (KEEP_GOING && !m_pSrcBrd->connect())           { common::Logging::instance()->log("ERROR: Failed to connect to DUT"); retVal = false; }
            if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

            TEST_STEP(m_pSrcBrd->unlockBootloader(),            "unlock bootloader commands");

            TEST_STEP(m_pSrcBrd->copyFileToSdCard(file, destName, comms::FileMethod::BOOTLOADER_XMODEM), "copy firmware image to DUT SD card");

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
