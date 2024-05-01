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
#include <QProgressDialog>
#include <QApplication>

namespace testsuite
{
    // Spec table is searched by frequency starting at first entry in table
    const DdsPowerSpec TestCaseSourceBoardDdsPower::kDdsPowerSpecTable[] =
    {
        {520000000,  10.0}, // Require power >= 10.0dBm up to and including 520MHz
        {1500000000, 5.0}   // Require power >= 5.0dBm  up to and including 1500MHz
    };

    TestCaseSourceBoardDdsPower::TestCaseSourceBoardDdsPower(QObject* parent) :
        TestCaseSourceBoard(QString("DDS RF Output Power"), parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseSourceBoardDdsPower::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDdsPower::execute(bool manualSteps)
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

            // Set Source Board attenuation to 0 dB and disable blanking
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::DB_0),        "set Source Board attenutation to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                               "disable Source Board blanking on DUT");
            TEST_STEP(pHarness->setBlankIn(true),                                "disable external blanking from Test Jig");

            int steps = static_cast<int>((kTestStopFreq - kTestStartFreq) / kTestStepFreq);
            steps++;
            steps = steps * 2;   // repeated for 2 ports
            QProgressDialog progress("DDS RF Output Power...", "Cancel", 1, steps);
            progress.setWindowModality(Qt::ApplicationModal);
            progress.setValue(0);
            progress.setFixedSize(progress.sizeHint());
            bool progressCanceled = false;

            // Perform the test on Port 1 and Port 2
            for (int port = 1; (port < 3) && KEEP_GOING; port++)
            {
                common::Logging::instance()->log(QString("RF Port %1:").arg(port));

                // Indent log (automatically de-indented when object is destroyed)
                common::LogIndent indent;

                // Store the port as a DutRfPort type because it will be used later for
                // cable correction
                testjig::RfPathSourceAssy::DutRfPort rfPort;

                // Set the port on the DUT and the Test Jig
                if (port == 1)
                {
                    TEST_STEP(m_pSrcBrd->setRfOutputPort(dut::SourceBoard::PORT_1), "set DUT output port to: Port 1");
                    rfPort = testjig::RfPathSourceAssy::DutRfPort::LHS;
                }
                else
                {
                    TEST_STEP(m_pSrcBrd->setRfOutputPort(dut::SourceBoard::PORT_2), "set DUT output port to: Port 2");
                    rfPort = testjig::RfPathSourceAssy::DutRfPort::RHS;
                }

                TEST_STEP(jigRfSwitch.setDutRfPort(rfPort), QString("set jig DUT port"));

                for (uint64_t freq = kTestStartFreq; (freq <= kTestStopFreq) && KEEP_GOING; freq += kTestStepFreq)
                {
                    progress.setValue(progress.value() + 1);
                    progressCanceled = progress.wasCanceled();
                    // Set the DDS to output CW at this frequency at full amplitude
                    TEST_STEP(m_pSrcBrd->setDdsCw(freq, 0u), QString("set DDS to %1 MHz, full amplitude").arg(freq / 1e6));

                    // Configure the power meter for this frequency
                    TEST_STEP(pwrMtr->setFrequency(freq), QString("set power meter to %1 MHz").arg(freq / 1e6));

                    // Read power from the power meter
                    double power;
                    TEST_STEP(pwrMtr->getPower(power), "read RF power from Power Meter");

                    // Correct for specific path on test jig. This test is always performed using the
                    // short SMA-SMP cable (i.e. on the Source Board not the Source Board + Doubler)
                    testjig::RfPathSourceAssy path(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                                   rfPort,
                                                   testjig::RfPathSourceAssy::TePort::POWER_METER);

                    power = testjig::CableCal::correctedPower(path, freq, power);

                    // Round result to 1 d.p.
                    power = std::round(power * 10.0) / 10.0;

                    double min_power;
                    powerOk(freq, power, min_power);
                    TEST_RESULT(QString("Freq: %1 MHz").arg(freq / 1e6), power, min_power, 100.0, "dBm");
                    retVal  &= !progressCanceled;
                }
            }

            // Hide the progress dialog
            progress.hide();

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

    bool TestCaseSourceBoardDdsPower::powerOk(uint64_t freq, double power, double& min_power)
    {
        bool retVal = false;
        int num = sizeof(kDdsPowerSpecTable) / sizeof(DdsPowerSpec);

        int i;
        for (i = 0; (i < num) && (freq > kDdsPowerSpecTable[i].freq); i++)
        {
        }

        // If i == num then frequency was out of range
        if (i < num)
        {
            min_power = kDdsPowerSpecTable[i].pwr;
            if (power >= min_power)
            {
                retVal = true;
            }
        }

        return retVal;
    }
}
