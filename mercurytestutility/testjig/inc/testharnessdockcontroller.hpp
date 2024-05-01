#ifndef TESTHARNESSDOCKCONTROLLER_HPP
#define TESTHARNESSDOCKCONTROLLER_HPP

#include "testharness.hpp"
#include "port.hpp"

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class PaControlMonitorTest;

    class TestHarnessDockController : public TestHarness
    {
        Q_OBJECT

    public:
        TestHarnessDockController(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~TestHarnessDockController() {}

        bool inputCurrent(uint16_t& milliamps) const;

        bool inputVoltage(uint16_t& millivolts) const;

        bool enablePower(bool en) const;

        bool isPowerEnabled(bool& en) const;

        bool setBlankIn(hardware::Port port, bool blank) const;

        bool getPpsInPresent(bool& present) const;

        bool getInterfaceId(uint8_t& id) const;

        PaControlMonitorTest* paCtrlMonTest(hardware::Port port) const;

    private:
        static const uint16_t kRegAddrDockCurrAdc     = 0x4011;                      //!< FPGA2 ADC (U17), channel 2
        static const uint16_t kRegAddrDockBlankA      = 0x4020;                      //!< Dock blank control, channel 1
        static const uint16_t kRegAddrDockBlankB      = 0x4023;                      //!< Dock blank control, channel 2
        static const uint16_t kRegAddrDockCommon      = 0x4026;                      //!< Dock common (1PPS/ID)
        static const uint16_t kRegAddrPaGpioABase     = 0x8009;                      //!< PA control/monitor GPIO base, channel 1
        static const uint16_t kRegAddrPaGpioBBase     = 0x800b;                      //!< PA control/monitor GPIO base, channel 2

        static const uint8_t kRegWidthDockBlank       = 1u;
        static const uint8_t kRegWidthDockCommon      = 3u;

        static const uint8_t kDockBlankBitBlankN      = 0u;
        static const uint8_t kDockCommonLsbId         = 0u;
        static const uint8_t kDockCommonLsbWidth      = 2u;
        static const uint8_t kDockCommonBitPpsPresent = 2u;

        static constexpr double kDockCurrTranscond   = (1.0 / (20.0 * 0.75));   //!< Amps into load per Volt at ADC input. Voltage gain = x20, current sense resistor = 0.75 Ohm

        PaControlMonitorTest* m_pPaCtrlMonTest[2];                  //!< Pointer to objects used to read PA control/monitor status from test jig
    };
}

#endif // TESTHARNESSDOCKCONTROLLER_HPP
