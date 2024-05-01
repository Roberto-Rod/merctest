#ifndef TESTHARNESSPOWERMONITOR_HPP
#define TESTHARNESSPOWERMONITOR_HPP

#include "testharness.hpp"

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class TestHarnessPowerMonitor : public TestHarness
    {
        Q_OBJECT

    public:
        TestHarnessPowerMonitor(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~TestHarnessPowerMonitor() {}

        bool inputCurrent(uint16_t& milliamps) const;

        bool inputVoltage(uint16_t& millivolts) const;

        bool enablePower(bool en) const;

        bool isPowerEnabled(bool& en) const;

        bool getAdcReadings(uint16_t& fwd, uint16_t& rev) const;

        bool driveEnablePin(bool en) const;

    private:
        static const uint16_t kRegAddrPwrMonCtrlBase  = 0x8007;
        static const uint16_t kRegAddrPwrMonAdc       = 0x8008;
        static const uint16_t kRegAddrPwrMonCurrAdc   = 0x8013;     //!< FPGA3 ADC (U25), channel 4

        static const uint8_t  kRegWidthPwrMonCtrl     = 1u;

        static constexpr double kPwrMonCurrTranscond = (1.0 / (20.0 * 0.75));   //!< Amps into load per Volt at ADC input. Voltage gain = x20, current sense resistor = 0.75 Ohm
    };
}

#endif // TESTHARNESSPOWERMONITOR_HPP
