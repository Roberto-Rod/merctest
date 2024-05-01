#ifndef RFSWITCHMATRIX_HPP
#define RFSWITCHMATRIX_HPP

#include "rfpath.hpp"
#include "registercontrol.hpp"

namespace testjig
{
    class RfSwitchMatrix
    {
    public:
        RfSwitchMatrix(comms::RegisterControl* pRegs);

        bool setTestEquipment(RfPathSourceAssy::TePort te) const;
        RfPathSourceAssy::TePort selectedTestEquipment() const;

        bool setDutRfPort(RfPathSourceAssy::DutRfPort port) const;
        RfPathSourceAssy::DutRfPort selectedDutRfPort() const;

    private:
        static const uint16_t kAddressRfSwCtl    = 0x0002;     //!< RF switch control register
        static const uint32_t kMaskTestEquipment = 0x00000001; //!< Bit mask for test equipment selection
        static const uint32_t kMaskDutRf         = 0x00000002; //!< Bit mask for DUT RF port selection
        static const uint32_t kValPowerMeter     = 0x00000000; //!< Power meter select value
        static const uint32_t kValSpecAn         = 0x00000001; //!< Spectrum analyser select value
        static const uint32_t kValDutRfLhs       = 0x00000000; //!< DUT RF LHS select value
        static const uint32_t kValDutRfRhs       = 0x00000002; //!< DUT RF RHS select value

        comms::RegisterControl* m_pRegs;                       //!< Pointer to the register control object to use
    };
}

#endif // RFSWITCHMATRIX_HPP
