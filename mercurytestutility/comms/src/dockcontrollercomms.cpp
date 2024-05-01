#include "dockcontrollercomms.hpp"

namespace comms
{
    DockControllerComms::DockControllerComms(RegisterControl* pJigRegs, uint16_t addrOffset, QObject* parent) :
        QObject(parent),
        m_pJigRegs(pJigRegs),
        m_addrOffset(addrOffset)
    {
    }

    bool DockControllerComms::readRegister(uint16_t addr, uint32_t& data) const
    {
        return m_pJigRegs->readRegister(m_addrOffset + addr, data);
    }

    bool DockControllerComms::writeRegister(uint16_t addr, uint32_t data) const
    {
        return m_pJigRegs->writeRegister(m_addrOffset + addr, data);
    }
}
