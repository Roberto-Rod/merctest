#include "gpio.hpp"

namespace hardware
{
    uint32_t Gpio::getMask(uint8_t width, uint8_t lsbPos)
    {
        uint32_t mask = 0u;

        // We're returning a 32-bit mask, so if the width is greater than 32
        // or the LSB position is greater than 31 then just return 0
        if ((width <= 32u) && (lsbPos <= 31u))
        {
            // Left shift a 1 in for each bit
            for (uint8_t i = 0; i < width; i++)
            {
                mask = (mask << 1) | 1u;
            }

            // Shift the mask up to the LSB position
            mask <<= lsbPos;
        }

        return mask;
    }
}
