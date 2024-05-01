#include "pacontrolmonitor.hpp"
#include "registercontrol.hpp"
#include "registergpio.hpp"

namespace hardware
{
    PaControlMonitor::PaControlMonitor(comms::RegisterControl* pRegs, QObject* parent) :
        QObject(parent),
        m_pRegs(pRegs),
        m_baseAddr(kBaseAddrDefault)
    {
    }

    void PaControlMonitor::setBaseAddr(uint16_t addr)
    {
        m_baseAddr = addr;
    }

    bool PaControlMonitor::setPwrMonEn(bool en) const
    {
        // Set power monitor enable state
        hardware::RegisterGpio gpio(m_pRegs, m_baseAddr + kRegPaCtrlOffset);
        return gpio.setData(kPaCtrlBitPwrMonEn, en);
    }

    bool PaControlMonitor::readPwrMonitor(uint32_t& data) const
    {
        return m_pRegs->readRegister(m_baseAddr + kRegPaMonitorOffset, data);
    }

    bool PaControlMonitor::setPaShutdown(bool shdn) const
    {
        // Set shutdown state
        hardware::RegisterGpio gpio(m_pRegs, m_baseAddr + kRegPaCtrlOffset);
        return gpio.setData(kPaCtrlBitShdn, shdn);
    }

    bool PaControlMonitor::readPaController(uint8_t offset, uint8_t& data) const
    {
        // Read the PA Controller address
        uint32_t data32;
        bool retVal = m_pRegs->readRegister(m_baseAddr + kRegPaControllerBaseOffset + offset, data32);
        data = static_cast<uint8_t>(data32 & 0xFF);

        return retVal;
    }

    bool PaControlMonitor::setFixedPaBlanking(bool blank) const
    {
        // Enable fixed PA blanking
        hardware::RegisterGpio gpio(m_pRegs, m_baseAddr + kRegPaCtrlOffset);
        return gpio.setData(kPaCtrlBitMute, blank);
    }

    bool PaControlMonitor::getPaAlertState(bool& alert) const
    {
        // Get alert state
        hardware::RegisterGpio gpio(m_pRegs, m_baseAddr + kRegPaCtrlOffset);
        bool retVal = gpio.getData(kPaCtrlBitAlert, alert);

        // Invert alert as the signal is active low
        alert = !alert;

        return retVal;
    }

}
