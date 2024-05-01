#ifndef SOURCEBOARDBIT_HPP
#define SOURCEBOARDBIT_HPP

#include <cstdint>

namespace dut
{
    struct SourceBoardBit
    {
        friend bool operator <(const SourceBoardBit& lhs, const SourceBoardBit& rhs);
        friend bool operator >(const SourceBoardBit& lhs, const SourceBoardBit& rhs);

        uint16_t m_railVin;           //!< Vin rail (millivolts)
        uint16_t m_rail1V2;           //!< 1.2V rail (millivolts)
        uint16_t m_rail1V8;           //!< 1.8V rail (millivolts)
        uint16_t m_rail3V3;           //!< 3.3V rail (millivolts)
        uint16_t m_rail5V5;           //!< 5.5V rail (millivolts)
        uint16_t m_current;           //!< Input curret (milliamps)
        int16_t  m_intTemp;           //!< Internal temperature (degrees C)
        int16_t  m_ddsTemp;           //!< DDS temperature (degrees C)

        void clear();
    };
}

#endif // SOURCEBOARDBIT_HPP
