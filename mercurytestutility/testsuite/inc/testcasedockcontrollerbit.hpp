#ifndef TESTCASEDOCKCONTROLLERBIT_HPP
#define TESTCASEDOCKCONTROLLERBIT_HPP

#include "testcasedockcontroller.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseDockControllerBit : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerBit(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kInputCurrentMin = 25;       //!< Input current, min (mA)
        static const uint16_t kInputCurrentMax = 45;       //!< Input current, max (mA)
        static const uint16_t kInputVoltageMin = 22800;    //!< Input voltage, min (mV)
        static const uint16_t kInputVoltageMax = 25200;    //!< Input voltage, max (mV)
        static const uint16_t k3V0RailMin      = 2850;     //!< 3V0 rail voltage (3.0000V nom.), min (mV)
        static const uint16_t k3V0RailMax      = 3150;     //!< 3V0 rail voltage (3.0000V nom.), max (mV)
        static const uint16_t k3V3RailMin      = 3182;     //!< 3V3 rail voltage (3.3498V nom.), min (mV)
        static const uint16_t k3V3RailMax      = 3517;     //!< 3V3 rail voltage (3.3498V nom.), max (mV)
        static const uint16_t k5V5RailMin      = 5211;     //!< 5V5 rail voltage (5.4857V nom.), min (mV)
        static const uint16_t k5V5RailMax      = 5760;     //!< 5V5 rail voltage (5.4857V nom.), max (mV)
        static const int8_t   kDegCMin         = 15;       //!< Internal temperature, min (°C) (this is a live reading so needs wide range)
        static const int8_t   kDegCMax         = 40;       //!< Internal temperature, min (°C) (this is a live reading so needs wide range)
    };
}

#endif // TESTCASEDOCKCONTROLLERBIT_HPP
