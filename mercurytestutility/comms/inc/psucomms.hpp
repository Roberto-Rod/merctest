#ifndef PSUCOMMS_HPP
#define PSUCOMMS_HPP

#include <QObject>

#include "registercontrol.hpp"

namespace comms
{
    class PsuComms : public QObject, public RegisterControl
    {
        Q_OBJECT

    public:
        PsuComms(comms::RegisterControl* pJigRegs, QObject* parent = 0);

        ~PsuComms() {}

        bool resetI2cMaster() const;

        bool readRegister(uint16_t addr, uint32_t& data) const;

        bool readRegister(uint8_t addr, uint8_t& data) const;

        bool writeRegister(uint16_t addr, uint32_t data) const;

        bool writeRegister(uint8_t addr, uint8_t data) const;

    private:
        static const uint16_t kRegAddrPsuI2cBase = 0x8100; //!< PSU I2C Control Address
        static const uint16_t kRegAddrPsuI2cCtrl = 0x8200; //!< PSU I2C Registers Base Address

        static const uint8_t  kCtrlResetBit      = 0u;     //!< I2C Control: Reset bit
        static const uint8_t  kCtrlErrorBit      = 1u;     //!< I2C Control: Error bit
        static const uint8_t  kCtrlReadyBit      = 2u;     //!< I2C Control: Ready bit

        comms::RegisterControl* m_pJigRegs;                //!< Pointer to jig registers object

        bool statusOk() const;
    };
}

#endif // PSUCOMMS_HPP
