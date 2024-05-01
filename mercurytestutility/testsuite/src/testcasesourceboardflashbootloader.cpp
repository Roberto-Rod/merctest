#include "testcasesourceboardflashapp.hpp"
#include "testcasesourceboardflashbootloader.hpp"
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
   const QString TestCaseSourceBoardFlashBootloader::kBootloaderConfigData = "67 48 0a 00 01 09 03 4f 00 10";   // new header version (09) Temper enable delay (disabled)
   //const QString TestCaseSourceBoardFlashBootloader::kBootloaderConfigData = "67 48 0a 00 01 08 03 4f 00 10";   // Osc disabled
   //const QString TestCaseSourceBoardFlashBootloader::kBootloaderConfigData = "67 48 0a 00 01 08 03 0f 00 10";   // Osc enabled

    TestCaseSourceBoardFlashBootloader::TestCaseSourceBoardFlashBootloader(QObject* parent) :
        TestCaseSourceBoard("Flash Bootloader", parent)
    {
    }

    QString TestCaseSourceBoardFlashBootloader::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardFlashBootloader::execute(bool manualSteps)
    {
        (void)manualSteps;

        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Set to default value, set to true if in 'Smart Download' mode
        // and all firmware present.
        bool automaticSmartTestfail = true;

        //dut::RunMethod runMethod = TestCase::getRunMode();
        dut::RunMethod runMethod = dut::RunMethod::AUTOMATIC_RUN;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            //int rev = common::Settings::instance()->getRevision();

            // Get a pointer to the SourceBoard object
            m_pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());
            retVal &= (m_pSrcBrd != 0);

            QObject* ancestor = this;
            TestCase * topleveltestcase;
            topleveltestcase =  dynamic_cast<TestCase *> (ancestor->parent());

            //testsuite::TestCase * tptr = static_cast<testsuite::TestCase *> (dut()->topLevelTestCase());
            //runMethod = tptr->getRunMode();

            runMethod = topleveltestcase->getRunMode();

            // Get a pointer to the Test Harness object
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            // Determine if in 'Smart Download' mode
            // If so power up board, break in and issue unlock command.
            if (runMethod == dut::RunMethod::AUTOMATIC_SMART)
            {
                QString serial, config, filelist;
                uint32_t uintserial, uintserialk;

                // Power cycle the DUT
                TEST_STEP(pHarness->powerCycle(), "power cycle DUT");
                // Short delay after entering ISP mode and before send FlashMagic commands
                QThread::msleep(500);

                // Do these without a TEST_STEP as they need to happen quickly - can't let user delay them
                if (KEEP_GOING && !m_pSrcBrd->connect())           { common::Logging::instance()->log("ERROR: Failed to connect to DUT"); retVal = false; }
                if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

                TEST_STEP(m_pSrcBrd->unlockBootloader(), "unlock bootloader commands");

                TEST_STEP(m_pSrcBrd->readBootloaderSerial(serial), "read serial number from bootloader");

                uintserial = serial.toUInt(&retVal, 16);
                uintserialk =  m_pSrcBrd->serialNumber().toUInt(&retVal,16);

                TEST_STEP((uintserial == uintserialk), "check serial number");

                TEST_STEP(m_pSrcBrd->readBootloaderConfig(config), "read bootloader config");

                TEST_STEP((config == kBootloaderConfigData), "check bootloader config data");

                TEST_STEP (m_pSrcBrd->sendBootloaderFileList(filelist), "Check File List");

                if(filelist.contains("FPGA.BIN",Qt::CaseInsensitive))
                {
                    topleveltestcase->setSkipTestFlag(true);
                }

                if (retVal)
                {
                    automaticSmartTestfail = false;
                } // if retVal
                else
                {
                    pHarness->powerOff();
                    gui::MessageDialog msg("Invalid Firmware, Power Cycle Test Jig and Select DDS Source Board <P> and either:  Test-> Run (Automated) or Test->Run (Manual Steps) option");
                    msg.exec();
                    retVal = false;
                }
            }   // if RunMethod()

            // If not in 'Smart Download' mode or 'Smart Download' test  failed
            if(automaticSmartTestfail)
            {
                QString programmingBatchFile;
                if(KEEP_GOING)
                {
                    programmingBatchFile = common::Settings::instance()->programmingScriptsDirectory() + "/progbl.bat";
                }

                // Power cycle the DUT
                TEST_STEP(pHarness->powerCycle(1), "power cycle DUT");

                // Short delay after powering on and before sending FlashMagic commands
                QThread::msleep(500);
                hardware::Jlink  jl(kDevice);

                // Program and verify using programming batch file.
                TEST_STEP(jl.flashDevice(programmingBatchFile), "program flash");

                // Short delay after entering ISP mode and before send FlashMagic commands
                QThread::msleep(500);

                // Do these without a TEST_STEP as they need to happen quickly - can't let user delay them
                if (KEEP_GOING && !m_pSrcBrd->connect())           { common::Logging::instance()->log("ERROR: Failed to connect to DUT"); retVal = false; }
                if (KEEP_GOING && !m_pSrcBrd->breakInBootloader()) { common::Logging::instance()->log("ERROR: Failed to \"break in\" to bootloader"); retVal = false; }

                TEST_STEP(m_pSrcBrd->unlockBootloader(),                              "unlock bootloader commands");

                TEST_STEP(m_pSrcBrd->sendBootloaderSerial(m_pSrcBrd->serialNumber()), "send serial number to bootloader");

                TEST_STEP(m_pSrcBrd->sendBootloaderConfig(kBootloaderConfigData),     "send bootloader config bytes");
            }  // Not 'Smart Download' Mode
        } // if (retval)

        // Set and log the status
        setStatus(retVal, testsPassed);
        return retVal;
    } // execute()
} // namespace testsuite
