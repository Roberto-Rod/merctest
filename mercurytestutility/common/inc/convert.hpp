#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <cstdint>

#include <QString>

namespace common
{
    class Convert
    {
    public:
        static bool strFloatToUInt64(const QString& in, uint64_t& out);
    };
}

#endif // CONVERT_HPP
