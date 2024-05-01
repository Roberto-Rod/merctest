#include "rfswitchmatrix.hpp"

namespace testjig
{
    RfSwitchMatrix::RfSwitchMatrix(comms::RegisterControl* pRegs) :
        m_pRegs(pRegs)
    {
    }

    bool RfSwitchMatrix::setTestEquipment(RfPathSourceAssy::TePort te) const
    {
        bool retVal = false;
        if (m_pRegs != 0)
        {
            // Initialise the data with an invalid value
            uint32_t data = 0xFFFFFFFF;

            // Determine the value to write into the register bitfield
            if (te == RfPathSourceAssy::TePort::POWER_METER)
            {
                data = kValPowerMeter;
            }
            else if (te == RfPathSourceAssy::TePort::SPECTRUM_ANALYSER)
            {
                data = kValSpecAn;
            }

            // Check that we have got a valid value to write
            if (data != 0xFFFFFFFF)
            {
                // Read-modify-write the register to set the appropriate bits
                retVal = m_pRegs->setBitField(kAddressRfSwCtl, kMaskTestEquipment, data);
            }
        }

        return retVal;
    }

    RfPathSourceAssy::TePort RfSwitchMatrix::selectedTestEquipment() const
    {
        RfPathSourceAssy::TePort retVal = RfPathSourceAssy::TePort::UNKNOWN;

        if (m_pRegs != 0)
        {
            uint32_t data;
            if (m_pRegs->getBitField(kAddressRfSwCtl, kMaskTestEquipment, data))
            {
                if (data == kValPowerMeter)
                {
                    retVal = RfPathSourceAssy::TePort::POWER_METER;
                }
                else if (data == kValSpecAn)
                {
                    retVal = RfPathSourceAssy::TePort::SPECTRUM_ANALYSER;
                }
            }
        }

        return retVal;
    }

    bool RfSwitchMatrix::setDutRfPort(RfPathSourceAssy::DutRfPort port) const
    {
        bool retVal = false;

        if (m_pRegs != 0)
        {
            // Initialise the data with an invalid value
            uint32_t data = 0xFFFFFFFF;

            // Determine the value to write into the register bitfield
            if (port == RfPathSourceAssy::DutRfPort::LHS)
            {
                data = kValDutRfLhs;
            }
            else if (port == RfPathSourceAssy::DutRfPort::RHS)
            {
                data = kValDutRfRhs;
            }

            // Check that we have got a valid value to write
            if (data != 0xFFFFFFFF)
            {
                // Read-modify-write the register to set the appropriate bits
                retVal = m_pRegs->setBitField(kAddressRfSwCtl, kMaskDutRf, data);
            }
        }

        return retVal;
    }

    RfPathSourceAssy::DutRfPort RfSwitchMatrix::selectedDutRfPort() const
    {
        RfPathSourceAssy::DutRfPort retVal = RfPathSourceAssy::DutRfPort::UNKNOWN;

        if (m_pRegs != 0)
        {
            uint32_t data;
            if (m_pRegs->getBitField(kAddressRfSwCtl, kMaskDutRf, data))
            {
                if (data == kValDutRfLhs)
                {
                    retVal = RfPathSourceAssy::DutRfPort::LHS;
                }
                else if (data == kValDutRfRhs)
                {
                    retVal = RfPathSourceAssy::DutRfPort::RHS;
                }
            }
        }

        return retVal;
    }
}
