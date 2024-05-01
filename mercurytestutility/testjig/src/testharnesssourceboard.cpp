#include "testharnesssourceboard.hpp"
#include "mercurytestjig1.hpp"
#include "registercomms.hpp"
#include "registergpio.hpp"
#include "directserialcomms.hpp"
#include "dutpowercontrol.hpp"
#include "pacontrolmonitortest.hpp"
#include "settings.hpp"

#include <QThread>

namespace testjig
{
    // Voltage gain = x20, current sense resistor = 0.33 Ohm
    const double TestHarnessSourceBoard::kSourceCurrTranscond = (1.0 / (20.0 * 0.33));

    TestHarnessSourceBoard::TestHarnessSourceBoard(comms::RegisterControl* pJigRegs, QObject* parent) :
        TestHarness(pJigRegs, parent)
    {
        // Create PA control/monitor test objects
        m_pPaCtrlMonTest[0] = new PaControlMonitorTest(pJigRegs, kRegAddrPaGpioABase, this);
        m_pPaCtrlMonTest[1] = new PaControlMonitorTest(pJigRegs, kRegAddrPaGpioBBase, this);
    }

    bool TestHarnessSourceBoard::enablePower(bool en) const
    {
        // Enable/disable the power
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.enablePower(testjig::PowerSupply::SOURCE_BOARD, en);
    }

    bool TestHarnessSourceBoard::isPowerEnabled(bool& en) const
    {
        // Get the power  status
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.isPowerEnabled(testjig::PowerSupply::SOURCE_BOARD, en);
    }

    bool TestHarnessSourceBoard::inputCurrent(uint16_t& milliamps) const
    {
        return TestHarness::inputCurrent(milliamps, kRegAddrSourceCurrAdc, kSourceCurrTranscond);
    }

    bool TestHarnessSourceBoard::inputVoltage(uint16_t& millivolts) const
    {
        bool retVal = false;

        // May be able to return ADC reading in future version of hardware. For now:
        //   - If power is not enabled then return 0V
        //   - If power is enabled then just return 24.0V (user will be instructed to set bench PSU to this value)
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

    bool TestHarnessSourceBoard::driveShutdown(bool shdn) const
    {
        // Drive the shutdown control
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrSrcCtrlBase, kRegWidthSrcCtrl);

        return (gpio.setDirection(kSrcCtrlBitShdn, hardware::Gpio::Dir::OUT) &&
                 gpio.setData(kSrcCtrlBitShdn, shdn));
    }

    bool TestHarnessSourceBoard::setBlankIn(bool blank) const
    {
        // Drive the blank control
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrSrcCtrlBase, kRegWidthSrcCtrl);

        // Set to output and drive the inverted state (blank is active low)
        return (gpio.setDirection(kSrcCtrlBitBlankIn, hardware::Gpio::Dir::OUT) &&
                 gpio.setData(kSrcCtrlBitBlankIn, !blank));
    }

    bool TestHarnessSourceBoard::getExtBlankOutState(bool& blank) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrSrcCtrlBase, kRegWidthSrcCtrl);

        bool retVal = gpio.getData(kSrcCtrlBitBlankOut, blank);

        // Invert state (blank is active low)
        blank = !blank;

        return retVal;
    }

    bool TestHarnessSourceBoard::setPpsOutEnable(bool en) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPpsCtrlStat, kRegWidthPpsCtrlStat);

        return gpio.setData(kPpsCtrlBitEnablePpsOut, en);
    }

    bool TestHarnessSourceBoard::getPpsInPresent(bool& present) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPpsCtrlStat, kRegWidthPpsCtrlStat);

        return gpio.getData(kPpsCtrlBitPpsInPresent, present);
    }

    bool TestHarnessSourceBoard::deassertTamperInputs() const
    {
        // Create a register GPIO object to control the external tamper switch signals
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrTamperBase, kRegWidthTamper);

        // De-assert all the tamper pins by setting the entire bus to inputs
        return gpio.setDirection(hardware::Gpio::Dir::IN);
    }

    bool TestHarnessSourceBoard::assertTamperInput(uint8_t pin) const
    {
        // Create a register GPIO object to control the external tamper switch signals
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrTamperBase, kRegWidthTamper);

        // Assert this single tamper pin by setting to output and driving low
        return (gpio.setData(pin, false) &&
                 gpio.setDirection(pin, hardware::Gpio::Dir::OUT));
    }

    bool TestHarnessSourceBoard::setSlotId(uint8_t id) const
    {
        // Drive the slot ID bits
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrSrcCtrlBase, kRegWidthSrcCtrl);

        bool retVal = true;
        retVal &= gpio.setDirection(kSrcCtrlBitGpio0, 4u, hardware::Gpio::Dir::OUT);
        retVal &= gpio.setData(kSrcCtrlBitGpio0, 4u, (id & 0xf));

        return retVal;
    }

    bool TestHarnessSourceBoard::sendCanTestPacket(bool& ackReceived) const
    {
        bool retVal = false;
        ackReceived = false;

        if (m_pJigRegs->writeRegister(kRegCanCtrlStat, 0))
        {
            uint32_t data;
            retVal = m_pJigRegs->readRegister(kRegCanCtrlStat, data);
            ackReceived = (data == 0u);
        }

        return retVal;
    }

    bool TestHarnessSourceBoard::getDockBlankState(hardware::Port port, bool& blank) const
    {
        bool retVal = false;
        uint16_t regAddr = 0u;

        // Get the register address for the specific port
        if      (port == hardware::Port::A) { regAddr = kRegAddrDockCtrlABase; }
        else if (port == hardware::Port::B) { regAddr = kRegAddrDockCtrlBBase; }

        if (regAddr != 0u)
        {
            // Create a GPIO object
            hardware::RegisterGpio gpio(m_pJigRegs, regAddr, kRegWidthDockCtrl);

            retVal = true;
            // Enable receiver output (active low)
            retVal &= gpio.setData(kDockCtrlBitBlankReN, false);

            // Disable transmitter output (active high)
            retVal &= gpio.setData(kDockCtrlBitBlankDe, false);

            // Set directions
            retVal &= gpio.setDirection(kDockCtrlBitBlankRo,  hardware::Gpio::Dir::IN);
            retVal &= gpio.setDirection(kDockCtrlBitBlankReN, hardware::Gpio::Dir::OUT);
            retVal &= gpio.setDirection(kDockCtrlBitBlankDe,  hardware::Gpio::Dir::OUT);
            retVal &= gpio.setDirection(kDockCtrlBitBlankDi,  hardware::Gpio::Dir::OUT);

            // Get receiver output value
            retVal &= gpio.getData(kDockCtrlBitBlankRo, blank);

            // Invert blank as the signal is active low
            blank = !blank;
        }

        return retVal;
    }

    PaControlMonitorTest* TestHarnessSourceBoard::paCtrlMonTest(hardware::Port port) const
    {
        PaControlMonitorTest* retVal = 0;

        if      (port == hardware::Port::A) { retVal = m_pPaCtrlMonTest[0]; }
        else if (port == hardware::Port::B) { retVal = m_pPaCtrlMonTest[1]; }

        return retVal;
    }
}
