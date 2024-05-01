#ifndef TESTCASEPOWERMONITORINSERTIONLOSS_HPP
#define TESTCASEPOWERMONITORINSERTIONLOSS_HPP

#include "testcasepowermonitor.hpp"
#include "rfband.hpp"

namespace testsuite
{
    struct TestLimitInsertionLoss
    {
        TestLimitInsertionLoss(uint64_t frequency, double insertionLossMax) :
            m_frequency(frequency),
            m_insertionLossMax(insertionLossMax)
        {
        }

        uint64_t m_frequency;         //!< Limit applied to this frequency and below
        double m_insertionLossMax;    //!< Maximum insertion loss
    };

    class TestCasePowerMonitorInsertionLoss : public TestCasePowerMonitor
    {
        Q_OBJECT

    public:
        TestCasePowerMonitorInsertionLoss(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

        bool testPoint(hardware::RfBand::Band band, uint64_t freq, double insertionLoss, double& maxInsertionLoss);

    private:

        static const uint64_t kLowBandStartFreq = 20000000;      //!< Low band start frequency (Hz)
        static const uint64_t kLowBandStopFreq  = 520000000;     //!< Low band stop frequency (Hz)
        static const uint64_t kLowBandStepFreq  = 5000000;       //!< Low band step frequency (Hz)

        static const uint64_t kMidBandStartFreq = 504000000;     //!< Mid band start frequency (Hz)
        static const uint64_t kMidBandStopFreq  = 2700000000;    //!< Mid band stop frequency (Hz)
        static const uint64_t kMidBandStepFreq  = 22000000;      //!< Mid band step frequency (Hz)

        static const uint64_t kHighBandStartFreq = 2400e6;         //!< High band start frequency (Hz)
        static const uint64_t kHighBandStopFreq  = 6000e6;           //!< High band stop frequency (Hz)
        static const uint64_t kHighBandStepFreq  = 22000000;      //!< High band step frequency (Hz)

        static const QList<TestLimitInsertionLoss> kLowBadLimits;
        static const QList<TestLimitInsertionLoss> kMidBadLimits;
        static const QList<TestLimitInsertionLoss> kHighBadLimits;
    };
}

#endif // TESTCASEPOWERMONITORINSERTIONLOSS_HPP
