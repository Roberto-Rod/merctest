#ifndef TESTCASEPOWERMONITORNORF_HPP
#define TESTCASEPOWERMONITORNORF_HPP

#include "testcasepowermonitor.hpp"

namespace testsuite
{
    class TestCasePowerMonitorNoRf : public TestCasePowerMonitor
    {
        Q_OBJECT

    public:
        TestCasePowerMonitorNoRf(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kLowBandNoRfFwdMin = 0;               //!< Low band, no RF, forward reading, min (mV)
        static const uint16_t kLowBandNoRfFwdMax = 300;             //!< Low band, no RF, forward reading, max (mV)
        static const uint16_t kLowBandNoRfRevMin = 0;               //!< Low band, no RF, reverse reading, min (mV)
        static const uint16_t kLowBandNoRfRevMax = 300;             //!< Low band, no RF, reverse reading, max (mV)

        static const uint16_t kMidBandNoRfFwdMin = 0;               //!< Mid band, no RF, forward reading, min (mV)
        static const uint16_t kMidBandNoRfFwdMax = 300;             //!< Mid band, no RF, forward reading, max (mV)
        static const uint16_t kMidBandNoRfRevMin = 0;               //!< Mid band, no RF, reverse reading, min (mV)
        static const uint16_t kMidBandNoRfRevMax = 300;             //!< Mid band, no RF, reverse reading, max (mV)

        static const uint16_t kHighBandNoRfFwdMin = 0;               //!< Mid band, no RF, forward reading, min (mV)
        static const uint16_t kHighBandNoRfFwdMax = 530;             //!< Mid band, no RF, forward reading, max (mV)
        static const uint16_t kHighBandNoRfRevMin = 0;               //!< Mid band, no RF, reverse reading, min (mV)
        static const uint16_t kHighBandNoRfRevMax = 530;             //!< Mid band, no RF, reverse reading, max (mV)
    };
}

#endif // TESTCASEPOWERMONITORNORF_HPP
