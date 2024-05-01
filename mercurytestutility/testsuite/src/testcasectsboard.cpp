#include "testcasectsboard.hpp"
#include "testcasectsboardbatterycharger.hpp"
#include "testcasectsboardbit.hpp"
#include "testcasectsboarddcpower.hpp"
#include "testcasectsboarddynamicrange.hpp"
#include "testcasectsboardflashfirmware.hpp"
#include "testcasectsboardgetversion.hpp"
#include "testcasectsboardifbandwidth.hpp"
#include "testcasectsboardmembraneinterface.hpp"
#include "testcasectsboardpreselectorbandwidth.hpp"
#include "testcasectsboardsetserialnumber.hpp"
#include "testcasectsboardtamper.hpp"
#include "testcasectsboardusbserialport.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseCtsBoard::TestCaseCtsBoard(QObject* parent) :
        TestCase(parent),
        m_pCtsBrd(nullptr)
    {
        // Make test case objects with this as their parent
        new TestCaseCtsBoardFlashFirmware(this);
        new TestCaseCtsBoardGetVersion(this);
        new TestCaseCtsBoardDcPower(this);
        new TestCaseCtsBoardBit(this);
        new TestCaseCtsBoardSetSerialNumber(this);
        new TestCaseCtsBoardMembraneInterface(this);
        new TestCaseCtsBoardTamper(this);
        new TestCaseCtsBoardIfBandwidth(this);
        new TestCaseCtsBoardPreselectorBandwidth(this);
        new TestCaseCtsBoardDynamicRange(this);
        new TestCaseCtsBoardBatteryCharger(this);
        new TestCaseCtsBoardUsbSerialPort(this);
    }

    TestCaseCtsBoard::TestCaseCtsBoard(QString name, QObject* parent) :
        TestCase(name, parent),
        m_pCtsBrd(nullptr)
    {
    }

    TestCaseCtsBoard::~TestCaseCtsBoard()
    {
    }

    QString TestCaseCtsBoard::setupMessage()
    {
        return QString("<B>Initial Test Setup:</B>"
                       "<P>1. Ensure DUT does <B>not</B> have battery installed."
                       "<P>2. Connect Test Jig \"PSU J1\" connector to DUT Battery Terminals using CAB0077 + CAB0366."
                       "<P>3. Connect Test Jig Dock \"-> J5\" connector to DUT J1 using CAB0363."
                       "<P>4. Connect Segger J-Link to DUT J11 using CAB0352."
                       "<P>5. Connect Signal Generator to DUT using RF Test Cable.");
    }

    bool TestCaseCtsBoard::initialiseCtsBrd()
    {
        bool retVal(false);

        // Get a pointer to the CtsBoard object
        m_pCtsBrd = dynamic_cast<dut::CtsBoard*>(dut());

        // Enable the CTS board and wait for it to be ready, always power cycle to make
        // sure the power gets enabled
        if (m_pCtsBrd)
        {
            if (m_pCtsBrd->testHarness()->powerCycle())
            {
                if (!m_pCtsBrd->connect())
                {
                    common::Logging::instance()->log(QString("ERROR: Could not connect to CTS Board!"));
                }
                else if (!m_pCtsBrd->waitReady())
                {
                    common::Logging::instance()->log(QString("ERROR: Timed out waiting for CTS Board to initialise!"));
                }
                else
                {
                    retVal = true;
                }
            }
            else
            {
                common::Logging::instance()->log(QString("ERROR: Could not enable CTS Board power!"));
            }
        }

        return retVal;
    }

    bool TestCaseCtsBoard::execute(bool manualSteps)
    {
        // Execute all of the test cases
        bool retVal = TestCase::execute(manualSteps);

        // Get a pointer to the DUT
        dut::CtsBoard* pCtsBrd = dynamic_cast<dut::CtsBoard*>(dut());

        // Get a pointer to the test harness
        testjig::TestHarnessCtsBoard* pHarness = static_cast<testjig::TestHarnessCtsBoard*>(pCtsBrd->testHarness());

        // Disable power to DUT
        pHarness->enablePower(false);

        return retVal;
    }

    void  TestCaseCtsBoard::setRunMode(dut::RunMethod runMethod)
    {
        m_runMethod = runMethod;
    }

    dut::RunMethod TestCaseCtsBoard::getRunMode()
    {
        return m_runMethod;
    }

    void TestCaseCtsBoard::setSkipTestFlag(bool skipTest)
    {
        m_skipTest = skipTest;
    }

    bool TestCaseCtsBoard::getSkipTestFlag()
    {
        return m_skipTest;
    }

}
