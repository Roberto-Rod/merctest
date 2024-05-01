#ifndef GPIO_HPP
#define GPIO_HPP

#include "stdint.h"

namespace hardware
{
    class Gpio
    {
    public:
        enum class Dir
        {
            IN,
            OUT
        };

        virtual ~Gpio() {}

        virtual uint8_t width() const = 0;

        virtual bool setData(uint8_t bitPos, bool val) const = 0;
        virtual bool getData(uint8_t bitPos, bool& val) const = 0;
        virtual bool setData(uint8_t lsbPos, uint8_t width, uint32_t val) const = 0;
        virtual bool getData(uint8_t lsbPos, uint8_t width, uint32_t& val) const = 0;

        virtual bool setDirection(Dir dir) const = 0;
        virtual bool setDirection(uint8_t bitPos, Dir dir) const = 0;
        virtual bool setDirection(uint8_t lsbPos, uint8_t width, Dir dir) const = 0;
        virtual bool getDirection(uint8_t bitPos, Dir& dir) const = 0;

        static uint32_t getMask(uint8_t width, uint8_t lsbPos = 0u);
    };
}

#endif // GPIO_HPP
