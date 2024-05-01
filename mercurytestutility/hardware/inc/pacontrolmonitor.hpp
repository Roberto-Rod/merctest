#ifndef PACONTROLMONITOR_HPP
#define PACONTROLMONITOR_HPP

#include <QObject>

namespace comms
{
    class RegisterControl;
}

namespace hardware
{
    class PaControlMonitor : public QObject
    {
        Q_OBJECT

    public:
        PaControlMonitor(comms::RegisterControl* pRegs, QObject* parent = 0);

        void setBaseAddr(uint16_t addr);

        bool setPwrMonEn(bool en) const;

        bool readPwrMonitor(uint32_t& data) const;

        bool setPaShutdown(bool shdn) const;

        bool readPaController(uint8_t offset, uint8_t& data) const;

        bool setFixedPaBlanking(bool blank) const;

        bool getPaAlertState(bool& alert) const;

    private:
        static const uint16_t kBaseAddrDefault            = 0x0170;
        static const uint16_t kRegPaCtrlOffset            = 0x0000;
        static const uint16_t kRegPaMonitorOffset         = 0x0004;
        static const uint16_t kRegPaControllerBaseOffset  = 0x0090;

        static const uint8_t  kPaCtrlBitPwrMonEn          = 0u;               //!< PA Control register: Power Monitor Enable
        static const uint8_t  kPaCtrlBitShdn              = 1u;               //!< PA Control register: Shutdown
        static const uint8_t  kPaCtrlBitMute              = 2u;               //!< PA Control register: Mute
        static const uint8_t  kPaCtrlBitAlert             = 31u;              //!< PA Control register: Alert

        comms::RegisterControl* m_pRegs;
        uint16_t m_baseAddr;
    };
}

#endif // PACONTROLMONITOR_HPP
