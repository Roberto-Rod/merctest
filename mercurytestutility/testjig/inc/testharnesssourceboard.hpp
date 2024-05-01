#ifndef TESTHARNESSSOURCEBOARD_HPP
#define TESTHARNESSSOURCEBOARD_HPP

#include "testharness.hpp"
#include "port.hpp"

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class PaControlMonitorTest;

    class TestHarnessSourceBoard : public TestHarness
    {
        Q_OBJECT

    public:
        TestHarnessSourceBoard(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~TestHarnessSourceBoard() {}

        bool inputCurrent(uint16_t& milliamps) const;

        bool inputVoltage(uint16_t& millivolts) const;

        bool enablePower(bool en) const;

        bool isPowerEnabled(bool& en) const;

        bool driveShutdown(bool shdn) const;

        bool setBlankIn(bool blank) const;

        bool getExtBlankOutState(bool& blank) const;

        bool setPpsOutEnable(bool en) const;

        bool getPpsInPresent(bool& present) const;

        bool deassertTamperInputs() const;

        bool assertTamperInput(uint8_t pin) const;

        bool setSlotId(uint8_t id) const;

        bool sendCanTestPacket(bool& ackReceived) const;

        bool getDockBlankState(hardware::Port port, bool& blank) const;

        PaControlMonitorTest* paCtrlMonTest(hardware::Port port) const;

    private:
        static const uint16_t kRegAddrPaGpioABase     = 0x0003;
        static const uint16_t kRegAddrSrcCtrlBase     = 0x0005;
        static const uint16_t kRegAddrTamperBase      = 0x0007;
        static const uint16_t kRegCanCtrlStat         = 0x0009;
        static const uint16_t kRegAddrDockCtrlABase   = 0x000a;
        static const uint16_t kRegAddrPpsCtrlStat     = 0x000c;

        static const uint16_t kRegAddrPaGpioBBase     = 0x4004;
        static const uint16_t kRegAddrDockCtrlBBase   = 0x400a;
        static const uint16_t kRegAddrSourceCurrAdc   = 0x4010;     //!< FPGA2 ADC (U17), channel 1

        static const uint8_t  kRegWidthSrcCtrl        = 9u;
        static const uint8_t  kRegWidthDockCtrl       = 10u;
        static const uint8_t  kRegWidthTamper         = 3u;
        static const uint8_t  kRegWidthPpsCtrlStat    = 2u;

        static const uint8_t  kSrcCtrlBitGpio0        = 0u;
        static const uint8_t  kSrcCtrlBitGpio1        = 1u;
        static const uint8_t  kSrcCtrlBitGpio2        = 2u;
        static const uint8_t  kSrcCtrlBitGpio3        = 3u;
        static const uint8_t  kSrcCtrlBitBlankOut     = 4u;
        static const uint8_t  kSrcCtrlBitBlankIn      = 5u;
        static const uint8_t  kSrcCtrlBitShdn         = 6u;

        static const uint8_t  kDockCtrlBitBlankRo     = 0u;         //!< Note that dock control bits are offset by 4 relative to numbers in bus in schematics
        static const uint8_t  kDockCtrlBitBlankReN    = 1u;
        static const uint8_t  kDockCtrlBitBlankDe     = 2u;
        static const uint8_t  kDockCtrlBitBlankDi     = 3u;

        static const uint8_t  kPpsCtrlBitPpsInPresent = 0u;
        static const uint8_t  kPpsCtrlBitEnablePpsOut = 1u;

        static const double kSourceCurrTranscond;                   //!< Amps into load per Volt at ADC input

        PaControlMonitorTest* m_pPaCtrlMonTest[2];                  //!< Pointer to objects used to read PA control/monitor status from test jig
    };
}

#endif // TESTHARNESSSOURCEBOARD_HPP
