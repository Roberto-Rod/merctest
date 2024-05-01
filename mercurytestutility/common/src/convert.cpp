#include "convert.hpp"

namespace common
{
    bool Convert::strFloatToUInt64(const QString& in, uint64_t& out)
    {
        bool retVal = false;

        // Attempt to convert string to double
        double dbl = in.toDouble(&retVal);

        // If that worked then round and cast to 64-bit unsigned integer
        if (retVal)
        {
            out = static_cast<uint64_t>(dbl + 0.5f);
        }

        return retVal;
    }
}
