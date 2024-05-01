#ifndef TESTHARNESSMEMBRANE_HPP
#define TESTHARNESSMEMBRANE_HPP

#include "testharness.hpp"

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class TestHarnessMembrane : public TestHarness
    {
        Q_OBJECT

    public:
        TestHarnessMembrane(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~TestHarnessMembrane() {}

        bool inputCurrent(uint16_t& milliamps) const;

        bool inputVoltage(uint16_t& millivolts) const;

        bool enablePower(bool en) const;

        bool isPowerEnabled(bool& en) const;

    private:
        static const uint16_t kRegAddrGpioBase  = 0x8006;
        static const uint8_t  kRegWidthGpio     = 1u;
    };
}

#endif // TESTHARNESSMEMBRANE_HPP
