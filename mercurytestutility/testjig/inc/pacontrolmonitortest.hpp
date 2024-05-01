#ifndef PACONTROLMONITORTEST_HPP
#define PACONTROLMONITORTEST_HPP

#include <QObject>

namespace comms
{
    class RegisterControl;
}

namespace testjig
{
    class PaControlMonitorTest : public QObject
    {
        Q_OBJECT

    public:
        PaControlMonitorTest(comms::RegisterControl* pJigRegs, uint16_t addr, QObject* parent = 0);

        bool getPwrMon5V5State(uint8_t pin, bool& en) const;

        bool getPwrMonEnState(bool& en) const;

        bool getPaShdnState(bool& shdn) const;

        bool getPaBlankState(bool& blank) const;

        bool setPaAlertState(bool alert) const;

    private:
        static const uint8_t  kRegWidthPaGpio = 6u;

        static const uint8_t  kPaCtrlBitPwrMon5V5_0   = 0u;
        static const uint8_t  kPaCtrlBitPwrMonEn      = 1u;
        static const uint8_t  kPaCtrlBitPaShdn        = 2u;
        static const uint8_t  kPaCtrlBitPwrMon5V5_1   = 3u;
        static const uint8_t  kPaCtrlBitCs2N          = 4u;
        static const uint8_t  kPaCtrlBitPaAlert       = 5u;
        static const uint8_t  kPaCtrlBitPaBlank       = 6u;

        comms::RegisterControl* m_pJigRegs;     //!< Pointer to jig registers object
        uint16_t m_addr;                        //!< Base address of the GPIO register for this PA control/monitor port
    };
}

#endif // PACONTROLMONITORTEST_HPP
