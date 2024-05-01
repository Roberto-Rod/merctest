#include "testcasepowermonitorinsertionloss.hpp"
#include "testequipmentstore.hpp"
#include "logging.hpp"
#include "cablecal.hpp"
#include "powermonitor.hpp"

namespace testsuite
{
    const QList<TestLimitInsertionLoss> TestCasePowerMonitorInsertionLoss::kLowBadLimits
                                                         = QList<TestLimitInsertionLoss>()
                                                           << TestLimitInsertionLoss(520000000ull, 0.5);

    const QList<TestLimitInsertionLoss> TestCasePowerMonitorInsertionLoss::kMidBadLimits
                                                         = QList<TestLimitInsertionLoss>()
                                                           << TestLimitInsertionLoss(1540000000ull, 1.4)
                                                           << TestLimitInsertionLoss(2700000000ull, 2.3);

    const QList<TestLimitInsertionLoss> TestCasePowerMonitorInsertionLoss::kHighBadLimits
                                                         = QList<TestLimitInsertionLoss>()
                                                           << TestLimitInsertionLoss(5700000000ull, 1.2)
                                                           << TestLimitInsertionLoss(6000000000ull, 1.8);

    TestCasePowerMonitorInsertionLoss::TestCasePowerMonitorInsertionLoss(QObject* parent) :
        TestCasePowerMonitor("Insertion Loss", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCasePowerMonitorInsertionLoss::setupMessage()
    {
        return QString();
    }

    bool TestCasePowerMonitorInsertionLoss::execute(bool manualSteps)
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

            TEST_STEP(te::TestEquipment::resetTe(teList()),                          "reset test equipment");

            TEST_STEP(te::TestEquipmentStore::signalGenerator()->setRfEnabled(true), "enable signal generator RF output");

            // Get a pointer to the power monitor object
            dut::PowerMonitor* powerMonitor = static_cast<dut::PowerMonitor*>(dut());

            // Get the start/stop/step frequencies and the test limit for the given band
            uint64_t startFreq = 0ull;
            uint64_t stopFreq  = 0ull;
            uint64_t stepFreq  = 0ull;

            if (powerMonitor->band() == hardware::RfBand::LOW)
            {
                startFreq = kLowBandStartFreq;
                stopFreq  = kLowBandStopFreq;
                stepFreq  = kLowBandStepFreq;
            }
            else if (powerMonitor->band() == hardware::RfBand::MID)
            {
                startFreq = kMidBandStartFreq;
                stopFreq  = kMidBandStopFreq;
                stepFreq  = kMidBandStepFreq;
            }
            else if (powerMonitor->band() == hardware::RfBand::HIGH)
            {
                startFreq = kHighBandStartFreq;
                stopFreq  = kHighBandStopFreq;
                stepFreq  = kHighBandStepFreq;
            }
            else
            {
                common::Logging::instance()->log("ERROR: No valid Power Monitor band set");
                retVal = false;
            }

            // Loop around all of the frequencies
            for (uint64_t freq = startFreq; (freq <= stopFreq) && KEEP_GOING; freq += stepFreq)
            {
                TEST_STEP(te::TestEquipmentStore::signalGenerator()->setFrequency(freq),
                           QString("set signal generator frequency (%1 MHz)").arg(freq / 1e6));

                double sigGenPower = 0.0;
                TEST_STEP(te::TestEquipmentStore::signalGenerator()->setOutputPower(sigGenPower),
                           QString("set signal generator power (%1 dBm)").arg(sigGenPower));

                TEST_STEP(te::TestEquipmentStore::powerMeter()->setFrequency(freq),
                           QString("set signal generator frequency (%1 MHz)").arg(freq / 1e6));

                double power;
                TEST_STEP(te::TestEquipmentStore::powerMeter()->getPower(power), "get power from Power Meter");

                testjig::RfPathPowerMonitor path(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AND_POWER_METER);

                double insertionLoss = sigGenPower - testjig::CableCal::correctedPower(path, freq, power);

                // Limit to positive numbers
                if (insertionLoss < 0.0) { insertionLoss = 0.0; }

                // Round to 1 d.p.
                insertionLoss = round(insertionLoss * 10.0) / 10.0;

                double min_insertionLoss = 0.00;
                double max_insertionLoss;
                testPoint(powerMonitor->band(), freq, insertionLoss, max_insertionLoss);

                TEST_RESULT(QString("Insertion loss at %1 MHz").arg(freq / 1e6),  insertionLoss,  min_insertionLoss, max_insertionLoss, "dBm");
            }
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }

    bool TestCasePowerMonitorInsertionLoss::testPoint(hardware::RfBand::Band band, uint64_t freq, double insertionLoss, double& maxInsertionLoss)
    {
        bool retVal = false;

        const QList<TestLimitInsertionLoss>* pLimits = 0;

        if (band == hardware::RfBand::LOW)
        {
            pLimits = &kLowBadLimits;
        }
        else if (band == hardware::RfBand::MID)
        {
            pLimits = &kMidBadLimits;
        }

        else if (band == hardware::RfBand::HIGH)
        {
            pLimits = &kHighBadLimits;
        }

        if (pLimits != 0)
        {
            // Loop through all test points
            int i;
            for (i = 0; i < pLimits->size(); i++)
            {
                if (freq <= pLimits->at(i).m_frequency)
                {
                    break;
                }
            }

            // If we found point, then test against it
            if (i < pLimits->size())
            {
                maxInsertionLoss = pLimits->at(i).m_insertionLossMax;
                retVal = (insertionLoss <= pLimits->at(i).m_insertionLossMax);
            }
        }

        return retVal;
    }
}
