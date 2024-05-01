#ifndef TESTCASECTSBOARDBATTERYCHARGER_HPP
#define TESTCASECTSBOARDBATTERYCHARGER_HPP

#include "testcasectsboard.hpp"

namespace testsuite
{
    class TestCaseCtsBoardBatteryCharger: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardBatteryCharger(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);
        void updateMessage(QString msg);

    public slots:
        void getTestStateDischarge();
        void getTestStateFault();

    private:
        static const uint16_t kDischargeVoltage_mV = 4050; //!< Voltage to discharge down to before enabling charger to test voltage increase (millivolts)
        static const uint16_t kChargeVoltageMin_mV = 4100; //!< Minimum charging voltage, with battery present (millivolts)
        static const uint16_t kChargeVoltageMax_mV = 4400; //!< Maximum charging voltage, with battery present (millivolts)
    };
}

#endif // TESTCASECTSBOARDBATTERYCHARGER_HPP

