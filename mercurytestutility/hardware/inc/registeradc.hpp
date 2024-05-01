#ifndef REGISTERADC_HPP
#define REGISTERADC_HPP

#include "registercomms.hpp"

namespace hardware
{
    class RegisterAdc
    {
    public:
        RegisterAdc(comms::RegisterControl* m_pRegs, uint16_t addr);

        bool getReading(uint16_t& millivolts);

        bool getReading(int16_t& millivolts, bool zeroNegativeResults = false);

    private:
        static const uint8_t kAveragesDefault = 8;

        comms::RegisterControl* m_pRegs;
        uint16_t m_addr;
        uint8_t m_averages;
    };
}

#endif // REGISTERADC_HPP
