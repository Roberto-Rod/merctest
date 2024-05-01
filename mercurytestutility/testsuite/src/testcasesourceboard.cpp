#include "testcasesourceboard.hpp"
#include "testcasesourceboardappboot.hpp"
#include "testcasesourceboardbit.hpp"
#include "testcasesourceboardcontrol.hpp"
#include "testcasesourceboarddaughter.hpp"
#include "testcasesourceboarddcpower.hpp"
#include "testcasesourceboardddspower.hpp"
#include "testcasesourceboardddsfrequency.hpp"
#include "testcasesourceboardddscalibration.hpp"
#include "testcasesourceboardddscalcheck.hpp"
#include "testcasesourceboardddscurrent.hpp"
#include "testcasesourceboarddockctrl.hpp"
#include "testcasesourceboardflashapp.hpp"
#include "testcasesourceboardflashbootloader.hpp"
#include "testcasesourceboardfileloader.hpp"
#include "testcasesourceboardgpio.hpp"
#include "testcasesourceboardsynth.hpp"
#include "testcasesourceboardtamper.hpp"
#include "testcasesourceboardpactrlmonitor.hpp"
#include "testcasesourceboardstoreddsfile.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseSourceBoard::TestCaseSourceBoard(QObject* parent) :
        TestCase(parent),
        m_pSrcBrd(nullptr)
    {
        // Make test case objects with this as their parent
        new TestCaseSourceBoardFlashBootloader(this);
        new TestCaseSourceBoardFlashApp(common::FirmwareImage::SRC_BRD_MAIN, this);
        new TestCaseSourceBoardFileLoader(common::FirmwareImage::SRC_BRD_FPGA, this);
        new TestCaseSourceBoardAppBoot(this);
        new TestCaseSourceBoardDcPower(this);
        new TestCaseSourceBoardBit(this);
        new TestCaseSourceBoardTamper(this);
        new TestCaseSourceBoardControl(this);
        new TestCaseSourceBoardGpio(this);
        new TestCaseSourceBoardDockCtrl(hardware::Port::A, this);
        new TestCaseSourceBoardPaCtrlMonitor(hardware::Port::A, this);
        new TestCaseSourceBoardDaughter(this);
        new TestCaseSourceBoardSynth(this);
        new TestCaseSourceBoardDdsCurrent(this);
        new TestCaseSourceBoardDdsPower(this);
        new TestCaseSourceBoardDdsFrequency(this);
        new TestCaseSourceBoardDdsCalibration(this);
        new TestCaseSourceBoardDdsCalCheck(this);
        new TestCaseSourceBoardStoreDdsFile(this);
    }

    TestCaseSourceBoard::TestCaseSourceBoard(QString name, QObject* parent) :
        TestCase(name, parent),
        m_pSrcBrd(nullptr)
    {
    }

    TestCaseSourceBoard::~TestCaseSourceBoard()
    {
    }

    QString TestCaseSourceBoard::setupMessage()
    {
        return QString("<B>Initial Test Setup:</B>"
                        "<P>1. Connect Test Jig to DUT PL1 (GPIO) using CAB0248."
                        "<P>2. Connect Test Jig to DUT PL2 (DOCK 1) using CAB0253."
                        "<P>3. Connect Test Jig to DUT PL4 (PA CONTROL 1) using CAB0252."
                        "<P>4. Connect Test Jig to DUT PL6 (CONTROL) using CAB0251."
                        "<P>5. Connect Test Jig to DUT PL7 (AT) using CAB0250."
                        "<P>6. Connect Test Jig to DUT PL9 (DC IN) using CAB0249."
                        "<P>7. Connect Test Jig to DUT SK6 (RF OUT 1) using CAB0255."
                        "<P>8. Connect Test Jig to DUT SK5 (RF OUT 2) using CAB0255."
                        "<P>9. Connect Test Jig to Power Meter using RF Test Cable."
                        "<P>10. Connect Test Jig to Spectrum Analyser using RF Test Cable.");
    }

    bool TestCaseSourceBoard::initialiseSrcBrd()
    {
        bool retVal = false;

        // Get a pointer to the SourceBoard object
        m_pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());

        // Enable the source board (if it's not already enabled) and wait for it to be ready
        if (m_pSrcBrd != 0)
        {
            if (m_pSrcBrd->testHarness()->enablePower(true))
            {
                if (!m_pSrcBrd->connect())
                {
                    common::Logging::instance()->log(QString("ERROR: Could not connect to Source Board!"));
                }
                else if (!m_pSrcBrd->waitReady())
                {
                    common::Logging::instance()->log(QString("ERROR: Timed out waiting for Source Board to initialise!"));
                }
                else
                {
                    retVal = true;
                }
            }
            else
            {
                common::Logging::instance()->log(QString("ERROR: Could not enable Source Board power!"));
            }
        }

        return retVal;
    }

    bool TestCaseSourceBoard::execute(bool manualSteps)
    {
        // Execute all of the test cases
        bool retVal = TestCase::execute(manualSteps);

        // Get a pointer to the DUT
        dut::SourceBoard* pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());

        // Get a pointer to the test harness
        testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(pSrcBrd->testHarness());

        // Disable power to DUT
        pHarness->enablePower(false);

        return retVal;
    }

    void TestCaseSourceBoard::setRunMode(dut::RunMethod runMethod)
    {
        m_runMethod = runMethod;
    }

    dut::RunMethod TestCaseSourceBoard::getRunMode()
    {
        return m_runMethod;
    }

    void TestCaseSourceBoard::setSkipTestFlag(bool skipTest)
    {
        m_skipTest = skipTest;
    }

    bool TestCaseSourceBoard::getSkipTestFlag()
    {
        return m_skipTest;
    }
}
