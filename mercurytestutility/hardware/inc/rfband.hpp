#ifndef RFBAND_HPP
#define RFBAND_HPP

#include <QString>

namespace hardware
{
    class RfBand
    {
    public:
        enum Band { LOW, MID, HIGH };

        static QString description(Band band);
    };
}

#endif // RFBAND_HPP
