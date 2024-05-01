#include "registercontrol.hpp"
namespace comms
{
    bool RegisterControl::setBitField(uint16_t addr, uint32_t mask, uint32_t data) const
    {
        bool retVal = false;

        uint32_t val;

        // Read-modify-write the register
        if (readRegister(addr, val))
        {
            // Clear the bits in the masked field
            val &= ~mask;

            // Set the bits in the masked field
            val |= (data & mask);

            retVal = writeRegister(addr, val);
        }

        return retVal;
    }

    bool RegisterControl::getBitField(uint16_t addr, uint32_t mask, uint32_t& data) const
    {
        bool retVal = false;

        // Just get the data and mask the requested bits
        if (readRegister(addr, data))
        {
            data &= mask;
            retVal = true;
        }

        return retVal;
    }
}
