#include "sourceboardbit.hpp"

namespace dut
{
    bool operator <(const SourceBoardBit& lhs,  const SourceBoardBit& rhs)
    {
        if ((lhs.m_current < rhs.m_current) ||
             (lhs.m_rail1V2 < rhs.m_rail1V2) ||
             (lhs.m_rail1V8 < rhs.m_rail1V8) ||
             (lhs.m_rail3V3 < rhs.m_rail3V3) ||
             (lhs.m_rail5V5 < rhs.m_rail5V5) ||
             (lhs.m_railVin < rhs.m_railVin) ||
             (lhs.m_intTemp < rhs.m_intTemp) ||
             (lhs.m_ddsTemp < rhs.m_ddsTemp))
        {
             return true;
        }
        else
        {
            return false;
        }
    }

    bool operator >(const SourceBoardBit& lhs,  const SourceBoardBit& rhs)
    {
        if ((lhs.m_current > rhs.m_current) ||
             (lhs.m_rail1V2 > rhs.m_rail1V2) ||
             (lhs.m_rail1V8 > rhs.m_rail1V8) ||
             (lhs.m_rail3V3 > rhs.m_rail3V3) ||
             (lhs.m_rail5V5 > rhs.m_rail5V5) ||
             (lhs.m_railVin > rhs.m_railVin) ||
             (lhs.m_intTemp > rhs.m_intTemp) ||
             (lhs.m_ddsTemp > rhs.m_ddsTemp))
        {
             return true;
        }
        else
        {
            return false;
        }
    }

    void SourceBoardBit::clear()
    {
        m_current = 0u;
        m_rail1V2 = 0u;
        m_rail1V8 = 0u;
        m_rail3V3 = 0u;
        m_rail5V5 = 0u;
        m_railVin = 0u;
        m_intTemp = 0;
        m_ddsTemp = 0;
    }
}
