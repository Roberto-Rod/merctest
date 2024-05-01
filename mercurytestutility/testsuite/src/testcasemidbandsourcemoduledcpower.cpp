#include "testcasemidbandsourcemoduledcpower.hpp"
#include "sourceboard.hpp"
#include "daughtermodule.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseMidBandSourceModuleDcPower::TestCaseMidBandSourceModuleDcPower(QObject* parent) :
        TestCaseSourceBoard("DC Power", parent)
    {
    }

    QString TestCaseMidBandSourceModuleDcPower::setupMessage()
    {
        return QString();
    }

    bool TestCaseMidBandSourceModuleDcPower::execute(bool manualSteps)
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

            // Get a pointer to the Test Harness object
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            dut::DaughterModule* pDut = static_cast<dut::DaughterModule*>(dut());
            int32_t deltaCurrentMin,  deltaCurrentMax;
            if(pDut->isMidBand())
            {
                  deltaCurrentMin = kDeltaCurrentMinMB;
                  deltaCurrentMax = kDeltaCurrentMaxMB;
            }
            else
            {
                deltaCurrentMin = kDeltaCurrentMinHB;
                deltaCurrentMax = kDeltaCurrentMaxHB;
            }

            // Disable power
            TEST_STEP(pHarness->enablePower(false), "disable jig power to DUT");

            // Initialise the Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            TEST_STEP(m_pSrcBrd->enableDaughter5V5(false), "disable daughter board 5V5");

            uint16_t milliampsInit, milliampsDgtrOn;

            TEST_STEP(pHarness->inputCurrent(milliampsInit),     "read DUT input current (daughter power disabled)");

            TEST_RESULT("Initialised Current (Daughter Off)", milliampsInit, kInitialisedCurrentMin, kInitialisedCurrentMax, "mA");

            TEST_STEP(m_pSrcBrd->enableDaughter5V5(true),        "enable daughter board 5V5");
            TEST_STEP(pHarness->inputCurrent(milliampsDgtrOn),   "read DUT input current (daughter power enabled)");

            int32_t  milliampsDelta = static_cast<int32_t >(milliampsDgtrOn) - static_cast<int32_t>(milliampsInit);

           TEST_RESULT("Delta Current (Daughter On/Daughter Off)", milliampsDelta,  deltaCurrentMin, deltaCurrentMax, "mA");

            // Clean up - attempt regardless of test state
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
