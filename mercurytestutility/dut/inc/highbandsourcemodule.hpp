#ifndef HIGHBANDSOURCEMODULE_HPP
#define HIGHBANDSOURCEMODULE_HPP

#include "sourceboard.hpp"

namespace dut
{
    class HighBandSourceModule : public SourceBoard
    {
        Q_OBJECT

    public:
        HighBandSourceModule(testjig::TestJig* parentJig = 0);

        bool isPathDoubled(uint8_t path);
    };
}

#endif // HIGHBANDSOURCEMODULE_HPP
