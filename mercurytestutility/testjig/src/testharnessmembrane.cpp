#include "testharnessmembrane.hpp"
#include "registergpio.hpp"

namespace testjig
{
    TestHarnessMembrane::TestHarnessMembrane(comms::RegisterControl* pJigRegs, QObject* parent) :
        TestHarness(pJigRegs, parent)
    {
    }

    bool TestHarnessMembrane::enablePower(bool en) const
    {
        // Function not supported for this device
        (void)en;
        return false;
    }

    bool TestHarnessMembrane::isPowerEnabled(bool& en) const
    {
        // Function not supported for this device
        (void)en;
        return false;
    }

    bool TestHarnessMembrane::inputCurrent(uint16_t& milliamps) const
    {
        // Function not supported for this device
        (void)milliamps;
        return false;
    }

    bool TestHarnessMembrane::inputVoltage(uint16_t& millivolts) const
    {
        // Function not supported for this device
        (void)millivolts;
        return false;
    }
}
