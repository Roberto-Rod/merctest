#ifndef TESTHARNESS_HPP
#define TESTHARNESS_HPP

#include <cstdint>

#include <QObject>

namespace comms
{
    class RegisterControl;
}

namespace testjig
{
    class TestJig;

    class TestHarness : public QObject
    {
        Q_OBJECT

    public:
        TestHarness(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        virtual ~TestHarness() {}

        virtual bool inputVoltage(uint16_t& millivolts) const = 0;

        virtual bool inputCurrent(uint16_t& milliamps) const = 0;

        virtual bool enablePower(bool en) const = 0;

        virtual bool isPowerEnabled(bool& en) const = 0;

        bool powerCycle(uint8_t delaySecs = 2u) const;

        void powerOff() const;

        void powerOn() const;

        comms::RegisterControl* m_pJigRegs;

    protected:
        static const uint32_t kSettleTime = 250u;      //!< Settling time (in ms) before ADC reading

        bool inputCurrent(uint16_t& milliamps, uint16_t regAddr, double transcond) const;
    };
}

#endif // TESTHARNESS_HPP
