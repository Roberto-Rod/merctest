#include "psucomms.hpp"
#include "registergpio.hpp"

namespace comms
{
    PsuComms::PsuComms(comms::RegisterControl* pJigRegs, QObject* parent) :
        QObject(parent),
        m_pJigRegs(pJigRegs)
    {
    }

    bool PsuComms::resetI2cMaster() const
    {
        bool retVal = true;
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPsuI2cCtrl, 32u);

        // Toggle the RESET bit
        retVal &= gpio.setData(kCtrlResetBit, true);
        retVal &= gpio.setData(kCtrlResetBit, false);

        // Test the READY bit
        bool ready;
        retVal &= gpio.getData(kCtrlReadyBit, ready);
        retVal &= ready;

        return retVal;
    }

    bool PsuComms::readRegister(uint16_t addr, uint32_t& data) const
    {
        uint8_t data8;
        bool retVal = readRegister(static_cast<uint8_t>(addr & 0xFF), data8);

        retVal &= statusOk();

        data = static_cast<uint32_t>(data8) & 0xFF;

        return retVal;
    }

    bool PsuComms::readRegister(uint8_t addr, uint8_t& data) const
    {
        uint32_t data32;
        bool retVal = m_pJigRegs->readRegister(kRegAddrPsuI2cBase + addr, data32);

        data = static_cast<uint8_t>(data32 & 0xFF);

        return retVal;
    }

    bool PsuComms::writeRegister(uint16_t addr, uint32_t data) const
    {
        return writeRegister(static_cast<uint8_t>(addr & 0xFF),
                              static_cast<uint8_t>(data & 0xFF));
    }

    bool PsuComms::writeRegister(uint8_t addr, uint8_t data) const
    {
        bool retVal = m_pJigRegs->writeRegister(kRegAddrPsuI2cBase + addr,
                                                 static_cast<uint32_t>(data) & 0xFF);

        retVal &= statusOk();

        return retVal;
    }

    bool PsuComms::statusOk() const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPsuI2cCtrl, 32u);

        bool error;
        bool retVal = gpio.getData(kCtrlErrorBit, error);
        retVal &= !error;

        return retVal;
    }
}
