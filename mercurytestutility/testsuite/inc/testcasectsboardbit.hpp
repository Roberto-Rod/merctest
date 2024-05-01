#ifndef TESTCASECTSBOARDBIT_HPP
#define TESTCASECTSBOARDBIT_HPP

#include "testcasectsboard.hpp"

namespace testsuite
{
    class TestCaseCtsBoardBit: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardBit(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kBatteryVoltageMin_mV = 4000;    //!< Minimum battery charging voltage, no battery present (millivolts)
        static const uint16_t kBatteryVoltageMax_mV = 4400;    //!< Maximum battery charging voltage, no battery present (millivolts)
        static const uint16_t kTamperVoltageMin_mV = 2700;     //!< Minimum anti-tamper device voltage (millivolts)
        static const uint16_t kTamperVoltageMax_mV = 3300;     //!< Maximum anti-tamper device voltage (millivolts)
        static const int16_t kProcessorTemperatureMin_degC = 0;  //!< Minimum processor temperature (degrees C)
        static const int16_t kProcessorTemperatureMax_degC = 45; //!< Minimum processor temperature (degrees C)
    };
}

#endif // TESTCASECTSBOARDBIT_HPP

