#ifndef GPIOUNIT_HPP
#define GPIOUNIT_HPP

#include "gpio.hpp"

#include "stdint.h"

namespace comms
{
    class RegisterControl;
}

namespace hardware
{
    class RegisterGpio : public Gpio
    {
    public:
        RegisterGpio(comms::RegisterControl *pRegs, uint16_t baseAddr = 0u, uint8_t regWidth = 32u);
        ~RegisterGpio() {}

        bool setBaseAddr(uint16_t baseAddr);
        bool setRegWidth(uint8_t regWidth);

        uint8_t width() const;

        bool setData(uint8_t bitPos, bool val) const;
        bool getData(uint8_t bitPos, bool& val) const;
        bool setData(uint8_t lsbPos, uint8_t width, uint32_t val) const;
        bool getData(uint8_t lsbPos, uint8_t width, uint32_t& val) const;

        bool setDirection(Dir dir) const;
        bool setDirection(uint8_t bitPos, Gpio::Dir dir) const;
        bool setDirection(uint8_t lsbPos, uint8_t width, Gpio::Dir dir) const;
        bool getDirection(uint8_t bitPos, Gpio::Dir& dir) const;

    private:
        static const uint8_t kMinRegWidth = 1u;     //!< Minimum register width
        static const uint8_t kMaxRegWidth = 32u;    //!< Maximum register width

        comms::RegisterControl* m_pRegs;            //!< Pointer to the register control object to use
        uint16_t m_dataAddr;                        //!< The address of the GPIO data register we are controlling
        uint16_t m_dirAddr;                         //!< The address of the GPIO direction register we are controlling
        uint8_t  m_regWidth;                        //!< The width of the GPIO register we are controlling
    };
}

#endif // GPIOUNIT_HPP
