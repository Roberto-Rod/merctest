#ifndef TESTHARNESSPSUMODULE_HPP
#define TESTHARNESSPSUMODULE_HPP

#include "testharness.hpp"
#include "psumodule.hpp"

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class TestHarnessPsuModule : public TestHarness
    {
        Q_OBJECT

    public:
        TestHarnessPsuModule(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~TestHarnessPsuModule() {}

        bool inputCurrent(uint16_t& milliamps) const;

        bool outputCurrent(uint16_t& milliamps) const;

        bool inputVoltage(uint16_t& millivolts) const;

        bool outputVoltage(uint16_t& millivolts) const;

        bool enablePower(bool en) const;

        bool isPowerEnabled(bool& en) const;

        bool setPaShutdownState(bool shdn) const;

        bool setPaMuteState(bool mute) const;

        bool getAlertState(bool& alert) const;

        bool getPaShutdownState(bool& shdn) const;

        bool getPaMuteState(bool& mute) const;

        bool getFanVoltage(dut::PsuModule::Fan fan, uint16_t& millivolts) const;

        bool getFanPwmGnd(dut::PsuModule::Fan fan, uint16_t& lowMicroseconds, uint16_t& highMicroseconds) const;

        bool getFanPwmCtrl(dut::PsuModule::Fan fan, uint16_t& lowMicroseconds, uint16_t& highMicroseconds) const;

        bool setFanPwmSense(dut::PsuModule::Fan fan, uint16_t lowMicroseconds, uint16_t highMicroseconds) const;

    private:
        static const uint16_t kRegAddrPsuOutVoltsPAdc  = 0x8010;   //!< FPGA3 ADC (U25), channel 1
        static const uint16_t kRegAddrPsuOutVoltsNAdc  = 0x8011;   //!< FPGA3 ADC (U25), channel 2
        static const uint16_t kRegAddrPsuOutCurrAdc    = 0x8012;   //!< FPGA3 ADC (U25), channel 3
        static const uint16_t kRegAddrPsuInCurrAdc     = 0x8015;   //!< FPGA3 ADC (U25), channel 6
        static const uint16_t kRegAddrFan1VoltsAdc     = 0x8016;   //!< FPGA3 ADC (U25), channel 7
        static const uint16_t kRegAddrFan2VoltsAdc     = 0x8017;   //!< FPGA3 ADC (U25), channel 8

        static const uint16_t kRegAddrPsuCtrlBase      = 0x8201;   //!< PSU Control GPIO base address
        static const uint8_t  kRegWidthPsuCtrl         = 3u;       //!< PSU Control GPIO width

        static const uint16_t kRegAddrPaCtrlBase       = 0x8203;   //!< PA Control GPIO base address
        static const uint8_t  kRegWidthPaCtrl          = 2u;       //!< PA Control GPIO width

        static const uint16_t kRegAddrFan1PwmGndLow    = 0x8205;   //!< Fan 1 PWM GND Low Count
        static const uint16_t kRegAddrFan1PwmGndHigh   = 0x8206;   //!< Fan 1 PWM GND High Count
        static const uint16_t kRegAddrFan1PwmCtrlLow   = 0x8207;   //!< Fan 1 PWM Ctrl Low Count
        static const uint16_t kRegAddrFan1PwmCtrlHigh  = 0x8208;   //!< Fan 1 PWM Ctrl High Count
        static const uint16_t kRegAddrFan1PwmSenseLow  = 0x8209;   //!< Fan 1 PWM Sense Low Count
        static const uint16_t kRegAddrFan1PwmSenseHigh = 0x820a;   //!< Fan 1 PWM Sense High Count
        static const uint16_t kRegAddrFan2PwmGndLow    = 0x820b;   //!< Fan 2 PWM GND Low Count
        static const uint16_t kRegAddrFan2PwmGndHigh   = 0x820c;   //!< Fan 2 PWM GND High Count
        static const uint16_t kRegAddrFan2PwmCtrlLow   = 0x820d;   //!< Fan 2 PWM Ctrl Low Count
        static const uint16_t kRegAddrFan2PwmCtrlHigh  = 0x820e;   //!< Fan 2 PWM Ctrl High Count
        static const uint16_t kRegAddrFan2PwmSenseLow  = 0x820f;   //!< Fan 2 PWM Sense Low Count
        static const uint16_t kRegAddrFan2PwmSenseHigh = 0x8210;   //!< Fan 2 PWM Sense High Count

        static const uint8_t  kPsuCtrlBitPosShdn       = 0u;       //!< Bit position of shutdown bit in PSU Control register
        static const uint8_t  kPsuCtrlBitPosAlert      = 1u;       //!< Bit position of alert bit in PSU Control register
        static const uint8_t  kPsuCtrlBitPosMute       = 2u;       //!< Bit position of mute bit in PSU Control register

        static const uint8_t  kPaCtrlBitPosShdn        = 0u;       //!< Bit position of shutdown bit in PA Control register
        static const uint8_t  kPaCtrlBitPosMute        = 1u;       //!< Bit position of mute bit in PA Control register

        static const uint32_t kPwmTicksPerMicrosecond  = 20ul;     //!< PWM counter ticks per microsecond

        static constexpr double kFanVoltsScale         = ((10.0 + 68.0) / 10.0);  //!< Potential divider scaling: 10k / 68k resistors.
        static constexpr double kPsuOutVoltsScale      = ((10.0 + 150.0) / 10.0); //!< Potential divider scaling: 10k / 150k resistors.
        static constexpr double kPsuOutCurrTranscond   = (1.0 / (20.0 * 0.04));   //!< Amps into load per Volt at ADC input. Voltage gain = x20, current sense resistor = 0.04 Ohm
        static constexpr double kPsuInCurrTranscond    = (1.0 / (20.0 * 0.04));   //!< Amps into PSU per Volt at ADC input. Voltage gain = x20, current sense resistor = 0.04 Ohm

        bool getFanPwm(uint16_t regAddr, uint16_t& lowMicroseconds, uint16_t& highMicroseconds) const;

        bool setFanPwm(uint16_t regAddr, uint16_t lowMicroseconds, uint16_t highMicroseconds) const;
    };
}

#endif // TESTHARNESSPSUMODULE_HPP
