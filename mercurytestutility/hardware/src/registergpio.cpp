#include "registergpio.hpp"
#include "registercontrol.hpp"

namespace hardware
{
    RegisterGpio::RegisterGpio(comms::RegisterControl* pRegs, uint16_t baseAddr, uint8_t regWidth) :
        m_pRegs(pRegs),
        m_dataAddr(0u),
        m_dirAddr(0u),
        m_regWidth(0u)
    {
        // Call the aset base address and set register width methods. If an
        // invalid value was passed in then the values will be left at 0 as
        // initialised above.
        (void)setBaseAddr(baseAddr);
        (void)setRegWidth(regWidth);
    }

    bool RegisterGpio::setBaseAddr(uint16_t baseAddr)
    {
       bool retVal = false;

       // Make sure base address is less than max 16-bit unsigned value
       // as the top address is base address + 1
       if (baseAddr < 0xFFFF)
       {
           m_dataAddr = baseAddr;
           m_dirAddr  = baseAddr + 1u;
           retVal = true;
       }

       return retVal;
    }

    bool RegisterGpio::setRegWidth(uint8_t regWidth)
    {
        bool retVal = false;

        // Limit register width to the min/max constant values
        if ((regWidth >= kMinRegWidth) && (regWidth <= kMaxRegWidth))
        {
            m_regWidth = regWidth;
            retVal = true;
        }

        return retVal;
    }

    uint8_t RegisterGpio::width() const
    {
        return m_regWidth;
    }

    bool RegisterGpio::setData(uint8_t bitPos, bool val) const
    {
        uint32_t val32 = (val ? 1u : 0u);
        return setData(bitPos, 1u, val32);
    }

    bool RegisterGpio::getData(uint8_t bitPos, bool& val) const
    {
        bool retVal = false;
        uint32_t val32;
        if (getData(bitPos, 1u, val32))
        {
            // The only valid values to have retrieved are 0 or 1, convert to bool...
            if (val32 == 0u)
            {
                val = false;
                retVal = true;
            }
            else if (val32 == 1u)
            {
                val = true;
                retVal = true;
            }
        }

        return retVal;
    }

    bool RegisterGpio::setData(uint8_t lsbPos, uint8_t width, uint32_t val) const
    {
        bool retVal = false;

        uint32_t mask = Gpio::getMask(width, lsbPos);

        if (mask != 0u)
        {
            // Create the data field to write by shifting up to the LSB position and masking
            uint32_t data = (val << lsbPos) & mask;

            if (m_pRegs != 0)
            {
                retVal = m_pRegs->setBitField(m_dataAddr, mask, data);
            }
        }

        return retVal;
    }

    bool RegisterGpio::getData(uint8_t lsbPos, uint8_t width, uint32_t& val) const
    {
        bool retVal = false;

        if (m_pRegs != 0)
        {
            // Get the bitfield mask
            uint32_t mask = Gpio::getMask(width, lsbPos);

            if (mask != 0u)
            {
                uint32_t data;

                // Get the data
                if (m_pRegs->getBitField(m_dataAddr, mask, data))
                {
                    // Shift the data back down to the right so that returned
                    // value isn't offset
                    val = (data >> lsbPos) & Gpio::getMask(width);
                    retVal = true;
                }
            }
        }

        return retVal;
    }

    bool RegisterGpio::setDirection(Gpio::Dir dir) const
    {
        return setDirection(0u, m_regWidth, dir);
    }

    bool RegisterGpio::setDirection(uint8_t bitPos, Gpio::Dir dir) const
    {
        return setDirection(bitPos, 1u, dir);
    }

    bool RegisterGpio::setDirection(uint8_t lsbPos, uint8_t width, Gpio::Dir dir) const
    {
        bool retVal = false;

        uint32_t mask = Gpio::getMask(width, lsbPos);

        if (mask != 0u)
        {
            // Direction register: 0 = input, 1 = output
            uint32_t data = 0u;

            // If direction is in, then leave data register as all 0's
            // If direction is out, then set data register to match mask
            // (1's in respective positions)
            if (dir == Gpio::Dir::OUT)
            {
                data = mask;
            }

            if (m_pRegs != 0)
            {
                retVal = m_pRegs->setBitField(m_dirAddr, mask, data);
            }
        }

        return retVal;
    }

    bool RegisterGpio::getDirection(uint8_t bitPos, Gpio::Dir& dir) const
    {
        bool retVal = false;

        if (m_pRegs != 0)
        {
            // Get the bitfield mask
            uint32_t mask = Gpio::getMask(1u, bitPos);

            if (mask != 0u)
            {
                uint32_t data;

                // Get the data
                if (m_pRegs->getBitField(m_dirAddr, mask, data))
                {
                    // We now have a value which is either 0 (input) or with a
                    // 1 in the mask position (output)
                    if (data == 0u)
                    {
                        dir = Gpio::Dir::IN;
                    }
                    else
                    {
                        dir = Gpio::Dir::OUT;
                    }

                    retVal = true;
                }
            }
        }

        return retVal;
    }
}
