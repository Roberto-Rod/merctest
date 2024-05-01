#include "testcasesourceboardddscurrent.hpp"
#include "testcasesourceboardddspower.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"
#include "sourceboardrfcontrol.hpp"
#include "rfswitchmatrix.hpp"
#include "mercurytestjig1.hpp"
#include "testequipmentstore.hpp"
#include "settings.hpp"

#include <cmath>

namespace testsuite
{
    TestCaseSourceBoardDdsCurrent::TestCaseSourceBoardDdsCurrent(QObject* parent) :
        TestCaseSourceBoard(QString("DDS Current Consumption"), parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseSourceBoardDdsCurrent::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDdsCurrent::execute(bool manualSteps)
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

            // Initialise Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // If in Individual test mode re-initialise synthesiser.
            if(!isTopLevel())
            {
                TEST_STEP(m_pSrcBrd->initialiseSynth(), "initialise synthesizer");
            }

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            // Get a pointer to the power meter object and reset the power meter
            te::IPowerMeter* pwrMtr = te::TestEquipmentStore::powerMeter();
            TEST_STEP(te::TestEquipment::resetTe(teList()),                                         "reset test equipment");

            // Create an RF switch matrix object and select the Power Meter port
            // LHS DUT port on the test jig
            testjig::RfSwitchMatrix jigRfSwitch(pHarness->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::POWER_METER), "set jig TE port to: Power Meter");
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::LHS),          "set jig DUT port to: LHS");

            // Perform the test on Port 1 alone
            TEST_STEP(m_pSrcBrd->setRfOutputPort(dut::SourceBoard::PORT_1),       "set DUT output port to: Port 1");

            // Set Source Board attenuation to 0 dB and disable blanking
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::DB_0),        "set Source Board attenutation to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                               "disable Source Board blanking on DUT");
            TEST_STEP(pHarness->setBlankIn(false),                                "disable external blanking from Test Jig");

            // Set the DDS to output CW at the test frequency at full amplitude
            TEST_STEP(m_pSrcBrd->setDdsCw(kTestFreq, 0u), QString("set DDS to %1 MHz, full amplitude").arg(kTestFreq / 1e6));

            // Configure the power meter for the test frequency
            TEST_STEP(pwrMtr->setFrequency(kTestFreq),    QString("set power meter to %1 MHz").arg(kTestFreq / 1e6));

            // Read power from the power meter
            double power;
            TEST_STEP(pwrMtr->getPower(power), "read RF power from Power Meter");

            // Correct for specific path on test jig. This test is always performed using the
            // short SMA-SMP cable (i.e. on the Source Board not the Source Board + Doubler)
            testjig::RfPathSourceAssy path(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                           testjig::RfPathSourceAssy::DutRfPort::LHS,
                                           testjig::RfPathSourceAssy::TePort::POWER_METER);

            power = testjig::CableCal::correctedPower(path, kTestFreq, power);

            // Round result to 1 d.p.
            power = std::round(power * 10.0) / 10.0;

            double min_power;
            // Is there enough RF power?
            bool rfPowerOk = TestCaseSourceBoardDdsPower::powerOk(kTestFreq, power, min_power);

            // Write line to log for this test
            common::Logging::instance()->logResult("RF Power Level", rfPowerOk);
            testsPassed &= rfPowerOk;

            // Get the current consumption
            uint16_t milliamps;
            TEST_STEP(pHarness->inputCurrent(milliamps), "read DUT input current");

            TEST_RESULT("DC Current", milliamps, kCurrentMin, kCurrentMax, "mA");

            // Clean up - attempt these regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Set DDS to zero (DC) output
                (void)m_pSrcBrd->setDdsCw(0, 0u);

                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
