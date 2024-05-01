#include "pacontrolmonitortest.hpp"
#include "registergpio.hpp"

namespace testjig
{
    PaControlMonitorTest::PaControlMonitorTest(comms::RegisterControl* pJigRegs, uint16_t addr, QObject* parent) :
        QObject(parent),
        m_pJigRegs(pJigRegs),
        m_addr(addr)
    {
    }

    bool PaControlMonitorTest::getPwrMon5V5State(uint8_t pin, bool& en) const
    {
        bool retVal = false;
        uint8_t bit = 255u;

        if      (pin == 0u) { bit = kPaCtrlBitPwrMon5V5_0; }
        else if (pin == 1u) { bit = kPaCtrlBitPwrMon5V5_1; }

        if (bit != 255u)
        {
            // Create a GPIO object
            hardware::RegisterGpio gpio(m_pJigRegs, m_addr, kRegWidthPaGpio);

            // Get 5V5 state
            retVal = gpio.getData(bit, en);
        }

        return retVal;
    }

    bool PaControlMonitorTest::getPwrMonEnState(bool& en) const
    {
        // Create a GPIO object
        hardware::RegisterGpio gpio(m_pJigRegs, m_addr, kRegWidthPaGpio);

        // Get enable state
        return gpio.getData(kPaCtrlBitPwrMonEn, en);
    }

    bool PaControlMonitorTest::getPaShdnState(bool& shdn) const
    {
        // Create a GPIO object
        hardware::RegisterGpio gpio(m_pJigRegs, m_addr, kRegWidthPaGpio);

        // Get shutdown state
        return gpio.getData(kPaCtrlBitPaShdn, shdn);
    }

    bool PaControlMonitorTest::getPaBlankState(bool& blank) const
    {
        // Create a GPIO object
        hardware::RegisterGpio gpio(m_pJigRegs, m_addr, kRegWidthPaGpio);

        // Get blank state
        bool retVal = gpio.getData(kPaCtrlBitPaBlank, blank);

        // Invert blank as the signal is active low
        blank = !blank;

        return retVal;
    }

    bool PaControlMonitorTest::setPaAlertState(bool alert) const
    {
        // Create a GPIO object
        hardware::RegisterGpio gpio(m_pJigRegs, m_addr, kRegWidthPaGpio);

        // Set alert state - invert the boolean as alert is active low
        // and set direction to output
        return (gpio.setData(kPaCtrlBitPaAlert, !alert) &&
                 gpio.setDirection(kPaCtrlBitPaAlert, hardware::RegisterGpio::Dir::OUT));
    }
}
