#ifndef DAUGHTERMODULE_HPP
#define DAUGHTERMODULE_HPP

#include "deviceundertest.hpp"
#include "rfband.hpp"
#include "sourceboard.hpp"

namespace dut
{
    struct DaughterBoardVariant : public Variant
    {
        DaughterBoardVariant(QString name, QString number, hardware::RfBand::Band band) :
            Variant(name, number),
            m_band(band)
        {
        }

        ~DaughterBoardVariant() {}

        hardware::RfBand::Band m_band;
    };

    class DaughterModule : public SourceBoard
    {
        Q_OBJECT

    public:
        DaughterModule(testjig::TestJig* parentJig = 0);
        hardware::RfBand::Band  band();
        uint16_t  getRfPathDivisor(uint8_t path);
        bool isMidBand();
        bool isHighBand();
    };
}

#endif // DAUGHTERMODULE_HPP
