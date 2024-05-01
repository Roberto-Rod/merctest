#include "testharnesspowermonitor.hpp"
#include "registergpio.hpp"
#include "dutpowercontrol.hpp"

#include <QThread>

namespace testjig
{
    TestHarnessPowerMonitor::TestHarnessPowerMonitor(comms::RegisterControl* pJigRegs, QObject* parent) :
        TestHarness(pJigRegs, parent)
    {
    }

    bool TestHarnessPowerMonitor::enablePower(bool en) const
    {
        // Enable/disable the power
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.enablePower(testjig::PowerSupply::POWER_MONITOR, en);
    }

    bool TestHarnessPowerMonitor::isPowerEnabled(bool& en) const
    {
        // Get the power status
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.isPowerEnabled(testjig::PowerSupply::POWER_MONITOR, en);
    }

    bool TestHarnessPowerMonitor::inputCurrent(uint16_t& milliamps) const
    {
        return TestHarness::inputCurrent(milliamps, kRegAddrPwrMonCurrAdc, kPwrMonCurrTranscond);
    }

    bool TestHarnessPowerMonitor::inputVoltage(uint16_t& millivolts) const
    {
        bool retVal = false;

        // May be able to return ADC reading in future version of hardware. For now:
        //   - If power is not enabled then return 0V
        //   - If power is enabled then just return 5.486V (nominal regulated voltage on Test Jig)
        bool enabled = false;

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

    bool TestHarnessPowerMonitor::getAdcReadings(uint16_t& fwd, uint16_t& rev) const
    {
        bool retVal = false;

        uint32_t data;

        if (m_pJigRegs->readRegister(kRegAddrPwrMonAdc, data))
        {
            rev = static_cast<uint16_t>((data >> 16) & 0xFFFF);
            fwd = static_cast<uint16_t>((data)       & 0xFFFF);

            retVal = true;
        }

        return retVal;
    }

    bool TestHarnessPowerMonitor::driveEnablePin(bool en) const
    {
        // Drive the enable pin
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPwrMonCtrlBase, kRegWidthPwrMonCtrl);

        return (gpio.setDirection(0u, hardware::Gpio::Dir::OUT) &&
                gpio.setData(0u, en));
    }
}
