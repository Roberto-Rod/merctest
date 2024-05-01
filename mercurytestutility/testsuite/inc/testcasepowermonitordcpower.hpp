#ifndef TESTCASEPOWERMONITORDCPOWER_HPP
#define TESTCASEPOWERMONITORDCPOWER_HPP

#include "testcasepowermonitor.hpp"

namespace testsuite
{
    class TestCasePowerMonitorDcPower : public TestCasePowerMonitor
    {
        Q_OBJECT

    public:
        TestCasePowerMonitorDcPower(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kCurrStandbyMin   = 0;               //!< Standby current, min (mA)
        static const uint16_t kCurrStandbyMax   = 10;              //!< Standby current, max (mA)
        static const uint16_t kCurrActiveMin    = 90;              //!< Active current, min (mA)
        static const uint16_t kCurrActiveMax    = 130;             //!< Active current, max (mA)
        static const uint16_t kCurrActiveMinHB    = 65;              //!< Active current, min (mA)
        static const uint16_t kCurrActiveMaxHB    = 105;             //!< Active current, max (mA)
    };
}

#endif // TESTCASEPOWERMONITORDCPOWER_HPP
