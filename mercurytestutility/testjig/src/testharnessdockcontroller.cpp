#include "testharnessdockcontroller.hpp"
#include "registergpio.hpp"
#include "dutpowercontrol.hpp"
#include "pacontrolmonitortest.hpp"

#include <QThread>

namespace testjig
{
    TestHarnessDockController::TestHarnessDockController(comms::RegisterControl* pJigRegs, QObject* parent) :
        TestHarness(pJigRegs, parent)
    {
        // Create PA control/monitor test objects
        m_pPaCtrlMonTest[0] = new PaControlMonitorTest(pJigRegs, kRegAddrPaGpioABase, this);
        m_pPaCtrlMonTest[1] = new PaControlMonitorTest(pJigRegs, kRegAddrPaGpioBBase, this);
    }

    bool TestHarnessDockController::enablePower(bool en) const
    {
        // Enable/disable the power
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.enablePower(testjig::PowerSupply::DOCK_CONTROLLER, en);
    }

    bool TestHarnessDockController::isPowerEnabled(bool& en) const
    {
        // Get the power  status
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.isPowerEnabled(testjig::PowerSupply::DOCK_CONTROLLER, en);
    }

    bool TestHarnessDockController::inputCurrent(uint16_t& milliamps) const
    {
        return TestHarness::inputCurrent(milliamps, kRegAddrDockCurrAdc, kDockCurrTranscond);
    }

    bool TestHarnessDockController::inputVoltage(uint16_t& millivolts) const
    {
        bool retVal = false;

        // May be able to return ADC reading in future version of hardware. For now:
        //   - If power is not enabled then return 0V
        //   - If power is enabled then just return 24.0V (nominal input voltage to Test Jig)
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
                millivolts = 24000u;
            }
        }

        return retVal;
    }

    bool TestHarnessDockController::setBlankIn(hardware::Port port, bool blank) const
    {
        bool retVal = false;
        uint16_t addr = 0;

        if      (port == hardware::Port::A) { addr = kRegAddrDockBlankA; }
        else if (port == hardware::Port::B) { addr = kRegAddrDockBlankB; }

        if (addr != 0)
        {
            // Drive the blank control
            hardware::RegisterGpio gpio(m_pJigRegs, addr, kRegWidthDockBlank);

            // Set to output and drive the inverted state (blank is active low)
            retVal = gpio.setData(kDockBlankBitBlankN, !blank);
        }

        return retVal;
    }

    bool TestHarnessDockController::getPpsInPresent(bool& present) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrDockCommon, kRegWidthDockCommon);

        return gpio.getData(kDockCommonBitPpsPresent, present);
    }

    bool TestHarnessDockController::getInterfaceId(uint8_t& id) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrDockCommon, kRegWidthDockCommon);

        uint32_t data;
        bool retVal = gpio.getData(kDockCommonLsbId, kDockCommonLsbWidth, data);

        id = static_cast<uint8_t>(data);

        return retVal;
    }

    PaControlMonitorTest* TestHarnessDockController::paCtrlMonTest(hardware::Port port) const
    {
        PaControlMonitorTest* retVal = 0;

        if      (port == hardware::Port::A) { retVal = m_pPaCtrlMonTest[0]; }
        else if (port == hardware::Port::B) { retVal = m_pPaCtrlMonTest[1]; }

        return retVal;
    }
}
