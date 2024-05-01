#include "testharnessctsboard.hpp"
#include "registergpio.hpp"
#include "dutpowercontrol.hpp"

#include <QThread>

namespace testjig
{
    TestHarnessCtsBoard::TestHarnessCtsBoard(comms::RegisterControl* pJigRegs, QObject* parent) :
        TestHarness(pJigRegs, parent)
    {
    }

    bool TestHarnessCtsBoard::enablePower(bool en) const
    {
        // Enable/disable the power, CTS is powered from Power Monitor port
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.enablePower(testjig::PowerSupply::POWER_MONITOR, en);
    }

    bool TestHarnessCtsBoard::isPowerEnabled(bool& en) const
    {
        // Get the power status, CTS is powered from Power Monitor port
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.isPowerEnabled(testjig::PowerSupply::POWER_MONITOR, en);
    }

    bool TestHarnessCtsBoard::inputCurrent(uint16_t& milliamps) const
    {
        return TestHarness::inputCurrent(milliamps, kRegAddrCurrAdc, kCurrTranscond);
    }

    bool TestHarnessCtsBoard::inputVoltage(uint16_t& millivolts) const
    {
        bool retVal(false);

        // May be able to return ADC reading in future version of hardware. For now:
        //   - If power is not enabled then return 0V
        //   - If power is enabled then just return 5.486V (nominal regulated voltage on Test Jig)
        bool enabled(false);

        if (isPowerEnabled(enabled))
        {
            retVal = true;

            // If the supply is not enabled then just return 0 millivolts
            if (!enabled)
            {
                millivolts = 0u;
            }
            else
            {
                millivolts = 5486u;
            }
        }

        return retVal;
    }
}
