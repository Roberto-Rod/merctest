#include "dutpowercontrol.hpp"
#include "registergpio.hpp"

namespace testjig
{
    DutPowerControl::DutPowerControl(comms::RegisterControl* pRegs) :
        m_pRegs(pRegs)
    {
    }

    DutPowerControl::~DutPowerControl()
    {
    }

    bool DutPowerControl::enablePower(PowerSupply supply, bool enable) const
    {
        bool retVal = false;

        comms::RegisterControl::RegDetails reg;

        if (getPwrEnDetails(supply, reg))
        {
            retVal = true;

            hardware::RegisterGpio gpio(m_pRegs);

            // Set the address & width in the GPIO unit
            retVal &= gpio.setBaseAddr(reg.addr);
            retVal &= gpio.setRegWidth(reg.width);

            // Set the enable bit for this supply
            retVal &= gpio.setData(reg.bitPos, enable);
        }

        return retVal;
    }

    bool DutPowerControl::isPowerEnabled(PowerSupply supply, bool& enabled) const
    {
        bool retVal = false;

        comms::RegisterControl::RegDetails reg;

        // Get the register details
        if (getPwrEnDetails(supply, reg))
        {
            // Create a GPIO unit
            hardware::RegisterGpio gpio(m_pRegs);

            // Set the address & width in the GPIO unit
            if (gpio.setBaseAddr(reg.addr) && gpio.setRegWidth(reg.width))
            {
                // Get the enable bit for this supply
                retVal = gpio.getData(reg.bitPos, enabled);
            }
        }

        return retVal;
    }

    bool DutPowerControl::isPowerGood(PowerSupply supply, bool& good) const
    {
        bool retVal = false;

        comms::RegisterControl::RegDetails reg;

        // Get the register details
        if (getPwrGdDetails(supply, reg))
        {
            // Create a GPIO unit
            hardware::RegisterGpio gpio(m_pRegs);

            // Set the address & width in the GPIO unit
            if (gpio.setBaseAddr(reg.addr) && gpio.setRegWidth(reg.width))
            {
                // Get the good bit for this supply
                retVal = gpio.getData(reg.bitPos, good);
            }
        }

        return retVal;
    }

    bool DutPowerControl::measureCurrent(PowerSupply supply, uint16_t& milliamps) const
    {
        (void)supply;
        (void)milliamps;
        return false;
    }

    bool DutPowerControl::getPwrEnDetails(PowerSupply supply, comms::RegisterControl::RegDetails& details)
    {
        bool retVal = false;

        // Default to the FPGA2 settings and change them to FPGA3 if necessary
        details.addr   = kBaseAddrDutPwrCtrlFpga2;
        details.width  = kGpioWidthPwrCtrlFpga2;
        details.bitPos = 0u;

        // If the Power Supply is one that we can control then set address and bit position
        // and return true, otherwise just return false
        if (supply == PowerSupply::DOCK_CONTROLLER)
        {
            details.bitPos = kBitPosDockPwrEn;
            retVal         = true;
        }
        else if (supply == PowerSupply::SOURCE_BOARD)
        {
            details.bitPos = kBitPosSrcPwrEn;
            retVal         = true;
        }
        else if (supply == PowerSupply::POWER_MONITOR)
        {
            details.addr   = kBaseAddrDutPwrCtrlFpga3;
            details.width  = kGpioWidthPwrCtrlFpga3;
            details.bitPos = kBitPosMonPwrEn;
            retVal         = true;
        }
        else if (supply == PowerSupply::PSU_BOARD)
        {
            details.bitPos = kBitPosPsuPwrEn;
            retVal         = true;
        }
        else if (supply == PowerSupply::TESTJIG_5V5)
        {
            details.bitPos = kBitPosJig5V5En;
            retVal         = true;
        }

        return retVal;
    }

    bool DutPowerControl::getPwrGdDetails(PowerSupply supply, comms::RegisterControl::RegDetails& details)
    {
        bool retVal = false;

        // Default to the FPGA2 settings and change them to FPGA3 if necessary
        details.addr   = kBaseAddrDutPwrCtrlFpga2;
        details.width  = kGpioWidthPwrCtrlFpga2;
        details.bitPos = 0u;

        if (supply == PowerSupply::TESTJIG_3V3)
        {
            details.bitPos = kBitPosJig3V3Gd;
            retVal         = true;
        }
        else if (supply == PowerSupply::TESTJIG_5V5)
        {
            details.bitPos = kBitPosJig5V5Gd;
            retVal         = true;
        }

        return retVal;
    }
}
