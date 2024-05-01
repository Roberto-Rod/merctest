#ifndef TESTHARNESSCTSBOARD_HPP
#define TESTHARNESSCTSBOARD_HPP

#include "testharness.hpp"

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class TestHarnessCtsBoard : public TestHarness
    {
        Q_OBJECT

    public:
        TestHarnessCtsBoard(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~TestHarnessCtsBoard() {}

        bool inputCurrent(uint16_t& milliamps) const;

        bool inputVoltage(uint16_t& millivolts) const;

        bool enablePower(bool en) const;

        bool isPowerEnabled(bool& en) const;

    private:
        static const uint16_t kRegAddrCtrlBase = 0x8007; //!< CTS shares Power Monitor control channel
        static const uint16_t kRegAddrCurrAdc  = 0x8013; //!< FPGA3 ADC (U25), channel 4

        static const uint8_t  kRegWidthCtrl    = 1u;

        static constexpr double kCurrTranscond = (1.0 / (20.0 * 0.75));   //!< Amps into load per Volt at ADC input. Voltage gain = x20, current sense resistor = 0.75 Ohm
    };
}

#endif // TESTHARNESSCTSBOARD_HPP
