#include "testharnesspsumodule.hpp"
#include "registergpio.hpp"
#include "registeradc.hpp"
#include "dutpowercontrol.hpp"

#include <QThread>

namespace testjig
{
    TestHarnessPsuModule::TestHarnessPsuModule(comms::RegisterControl* pJigRegs, QObject* parent) :
        TestHarness(pJigRegs, parent)
    {
    }

    bool TestHarnessPsuModule::enablePower(bool en) const
    {
        // Enable/disable the power
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.enablePower(testjig::PowerSupply::PSU_BOARD, en);
    }

    bool TestHarnessPsuModule::isPowerEnabled(bool& en) const
    {
        // Get the power  status
        testjig::DutPowerControl pwr(m_pJigRegs);
        return pwr.isPowerEnabled(testjig::PowerSupply::PSU_BOARD, en);
    }

    bool TestHarnessPsuModule::inputCurrent(uint16_t& milliamps) const
    {
        return TestHarness::inputCurrent(milliamps, kRegAddrPsuInCurrAdc, kPsuInCurrTranscond);
    }

    bool TestHarnessPsuModule::outputCurrent(uint16_t& milliamps) const
    {
        // Use the "inputCurrent" method - does the same job we need to do to measure output (load) current
        return TestHarness::inputCurrent(milliamps, kRegAddrPsuOutCurrAdc, kPsuOutCurrTranscond);
    }

    bool TestHarnessPsuModule::inputVoltage(uint16_t& millivolts) const
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

    bool TestHarnessPsuModule::outputVoltage(uint16_t& millivolts) const
    {
        bool retVal = false;

        // Get an ADC reading
        hardware::RegisterAdc adcP(m_pJigRegs, kRegAddrPsuOutVoltsPAdc);
        hardware::RegisterAdc adcN(m_pJigRegs, kRegAddrPsuOutVoltsNAdc);

        // But first wait for the settling time
        QThread::msleep(kSettleTime);

        int16_t millivoltsP, millivoltsN;

        if (adcP.getReading(millivoltsP))
        {
            // Scale the ADC reading up to represent the PSU output voltage
            millivoltsP = static_cast<int16_t >(round(static_cast<double>(millivoltsP) * kPsuOutVoltsScale));

            if (adcN.getReading(millivoltsN))
            {
                // Calculate total volts from PSU
                int16_t millivoltsTot = millivoltsP + millivoltsN;

                // If difference is less than zero then covnert to zero
                if (millivoltsTot < 0) { millivoltsTot = 0; }

                millivolts = static_cast<uint16_t>(millivoltsTot);

                retVal = true;
            }
        }

        return retVal;
    }

    bool TestHarnessPsuModule::setPaShutdownState(bool shdn) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPsuCtrlBase, kRegWidthPsuCtrl);

        // Set the shutdown bit
        return (gpio.setDirection(kPsuCtrlBitPosShdn, hardware::Gpio::Dir::OUT) &&
                 gpio.setData(kPsuCtrlBitPosShdn, shdn));
    }

    bool TestHarnessPsuModule::setPaMuteState(bool mute) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPsuCtrlBase, kRegWidthPsuCtrl);

        // Set the mute bit (invert as signal is active low)
        return (gpio.setDirection(kPsuCtrlBitPosMute, hardware::Gpio::Dir::OUT) &&
                 gpio.setData(kPsuCtrlBitPosMute, !mute));
    }

    bool TestHarnessPsuModule::getAlertState(bool& alert) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPsuCtrlBase, kRegWidthPsuCtrl);

        // Get the alert bit and invert as it is active low
        bool retVal = gpio.getData(kPsuCtrlBitPosAlert, alert);
        alert = !alert;

        return retVal;
    }

    bool TestHarnessPsuModule::getPaShutdownState(bool& shdn) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPaCtrlBase, kRegWidthPaCtrl);

        // Get the shutdown bit
        return gpio.getData(kPaCtrlBitPosShdn, shdn);
    }

    bool TestHarnessPsuModule::getPaMuteState(bool& mute) const
    {
        hardware::RegisterGpio gpio(m_pJigRegs, kRegAddrPaCtrlBase, kRegWidthPaCtrl);

        // Get the mute bit and invert it as mute is active low
        bool retVal = gpio.getData(kPaCtrlBitPosMute, mute);
        mute = !mute;

        return retVal;
    }

    bool TestHarnessPsuModule::getFanVoltage(dut::PsuModule::Fan fan, uint16_t& millivolts) const
    {
        bool retVal = false;

        uint16_t regAddr = 0u;

        if (fan == dut::PsuModule::Fan::FAN_1)      { regAddr = kRegAddrFan1VoltsAdc; }
        else if (fan == dut::PsuModule::Fan::FAN_2) { regAddr = kRegAddrFan2VoltsAdc; }

        if (regAddr != 0u)
        {
            // Get an ADC reading
            hardware::RegisterAdc adc(m_pJigRegs, regAddr);

            retVal = adc.getReading(millivolts);

            // Scale the ADC reading up to represent the fan voltage
            millivolts = static_cast<uint16_t >(round(static_cast<double>(millivolts) * kFanVoltsScale));
        }

        return retVal;
    }

    bool TestHarnessPsuModule::getFanPwmGnd(dut::PsuModule::Fan fan, uint16_t& lowMicroseconds, uint16_t& highMicroseconds) const
    {
        bool retVal = false;

        uint16_t regAddr = 0u;

        if (fan == dut::PsuModule::Fan::FAN_1)      { regAddr = kRegAddrFan1PwmGndLow; }
        else if (fan == dut::PsuModule::Fan::FAN_2) { regAddr = kRegAddrFan2PwmGndLow; }

        if (regAddr != 0u)
        {
            retVal = getFanPwm(regAddr, lowMicroseconds, highMicroseconds);
        }

        return retVal;
    }

    bool TestHarnessPsuModule::getFanPwmCtrl(dut::PsuModule::Fan fan, uint16_t& lowMicroseconds, uint16_t& highMicroseconds) const
    {
        bool retVal = false;

        uint16_t regAddr = 0u;

        if (fan == dut::PsuModule::Fan::FAN_1)      { regAddr = kRegAddrFan1PwmCtrlLow; }
        else if (fan == dut::PsuModule::Fan::FAN_2) { regAddr = kRegAddrFan2PwmCtrlLow; }

        if (regAddr != 0u)
        {
            retVal = getFanPwm(regAddr, lowMicroseconds, highMicroseconds);
        }

        return retVal;
    }

    bool TestHarnessPsuModule::setFanPwmSense(dut::PsuModule::Fan fan, uint16_t lowMicroseconds, uint16_t highMicroseconds) const
    {
        bool retVal = false;

        uint16_t regAddr = 0u;

        if (fan == dut::PsuModule::Fan::FAN_1)      { regAddr = kRegAddrFan1PwmSenseLow; }
        else if (fan == dut::PsuModule::Fan::FAN_2) { regAddr = kRegAddrFan2PwmSenseLow; }

        if (regAddr != 0u)
        {
            retVal = setFanPwm(regAddr, lowMicroseconds, highMicroseconds);
        }

        return retVal;
    }

    bool TestHarnessPsuModule::getFanPwm(uint16_t regAddr, uint16_t& lowMicroseconds, uint16_t& highMicroseconds) const
    {
        uint32_t lowCount, highCount;

        bool retVal = m_pJigRegs->readRegister(regAddr,     lowCount) &&
                      m_pJigRegs->readRegister(regAddr + 1, highCount);

        lowMicroseconds = static_cast<uint16_t >(std::round(static_cast<double>(lowCount) /
                                                               static_cast<double>(kPwmTicksPerMicrosecond)));
        highMicroseconds = static_cast<uint16_t >(std::round(static_cast<double>(highCount) /
                                                                static_cast<double>(kPwmTicksPerMicrosecond)));
        return retVal;
    }

    bool TestHarnessPsuModule::setFanPwm(uint16_t regAddr, uint16_t lowMicroseconds, uint16_t highMicroseconds) const
    {
        uint32_t lowCount  = kPwmTicksPerMicrosecond * lowMicroseconds;
        uint32_t highCount = kPwmTicksPerMicrosecond * highMicroseconds;

        return (m_pJigRegs->writeRegister(regAddr,     lowCount) &&
                 m_pJigRegs->writeRegister(regAddr + 1, highCount));
    }
}
