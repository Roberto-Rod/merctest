#include "highbandsourcemodule.hpp"
#include "testcasehighbandsourcemodule.hpp"

namespace dut
{
    HighBandSourceModule::HighBandSourceModule(testjig::TestJig* parentJig) :
        SourceBoard(parentJig)
    {
        m_variants.clear();
        m_variants.append(new Variant("High-Band Source Module", "ASY0033"));        // The single variant
       m_variants.append(new Variant("High-Band Source Module", "ASY0129"));        // The single variant
        m_pTopLevelTestCase = new testsuite::TestCaseHighBandSourceModule(this);
    }

    bool HighBandSourceModule::isPathDoubled(uint8_t path)
    {
        // In mid-band source module, path 0 is bypass (not doubled), other paths are doubled
        return (path != 0u);
    }
}
