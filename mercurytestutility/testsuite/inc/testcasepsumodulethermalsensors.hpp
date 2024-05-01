#ifndef TESTCASEPSUMODULETHERMALSENSORS_HPP
#define TESTCASEPSUMODULETHERMALSENSORS_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleThermalSensors : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleThermalSensors(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const int8_t kLocalTemperatureMin      = 15;  //!< Local temperature minimum, °C. This is a live reading so needs wide range.
        static const int8_t kLocalTemperatureMax      = 40;  //!< Local temperature maximum, °C. This is a live reading so needs wide range.
        static const int8_t kRemoteTemperatureMin     = 113; //!< Remote temperature minimum, °C. This is fixed to a nominal +115 C
        static const int8_t kRemoteTemperatureMax     = 117; //!< Remote temperature maximum, °C. This is fixed to a nominal +115 C
        static const int8_t kThermistorTemperatureMin = 8;   //!< Thermistor temperature minimum, °C. This is fixed to a nominal +10 C
        static const int8_t kThermistorTemperatureMax = 12;  //!< Thermistor temperature maximum, °C. This is fixed to a nominal +10 C
    };
}

#endif // TESTCASEPSUMODULETHERMALSENSORS_HPP
