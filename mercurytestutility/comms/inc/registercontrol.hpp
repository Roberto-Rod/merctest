#ifndef REGISTERCONTROL_HPP
#define REGISTERCONTROL_HPP

#include "stdint.h"

namespace comms
{
    class RegisterControl
    {
    public:
        struct RegDetails
        {
            uint16_t addr;
            uint8_t  width;
            uint8_t  bitPos;
        };

        virtual bool readRegister(uint16_t addr, uint32_t& data) const = 0;

        virtual bool writeRegister(uint16_t addr, uint32_t data) const = 0;

        bool setBitField(uint16_t addr, uint32_t mask, uint32_t data) const;

        bool getBitField(uint16_t addr, uint32_t mask, uint32_t& data) const;
    };
}

#endif // REGISTERCONTROL_HPP
