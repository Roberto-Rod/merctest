#ifndef DUTPOWERCONTROL_HPP
#define DUTPOWERCONTROL_HPP

#include "registercontrol.hpp"

#include <QObject>
#include <QMap>

namespace testjig
{
    enum class PowerSupply
    {
        TESTJIG_3V3,
        TESTJIG_5V5,
        SOURCE_BOARD,
        DOCK_CONTROLLER,
        POWER_MONITOR,
        PSU_BOARD
    };

    class DutPowerControl : public QObject
    {
        Q_OBJECT

    public:
        DutPowerControl(comms::RegisterControl* pRegs);
        ~DutPowerControl();

        bool enablePower(PowerSupply supply, bool enable) const;

        bool isPowerEnabled(PowerSupply supply, bool& enabled) const;

        bool isPowerGood(PowerSupply supply, bool& good) const;

        bool measureCurrent(PowerSupply supply, uint16_t& milliamps) const;

    private:
        // GPIO Base Addresses
        static const uint16_t kBaseAddrDutPwrCtrlFpga2  = 0x4002;   //!< Base address of FPGA2 DUT Power Control unit
        static const uint16_t kBaseAddrDutPwrCtrlFpga3  = 0x8002;   //!< Base address of FPGA3 DUT Power Control unit

        // GPIO Width
        static const uint8_t kGpioWidthPwrCtrlFpga2 = 6u;           //!< Width of DUT Power Control unit GPIO register in FPGA2
        static const uint8_t kGpioWidthPwrCtrlFpga3 = 6u;           //!< Width of DUT Power Control unit GPIO register in FPGA3

        // FPGA 2 Power Control Bit positions
        static const uint8_t kBitPosJig5V5En  = 0u;                 //!< Bit position of VAA_5V5_EN bit
        static const uint8_t kBitPosDockPwrEn = 1u;                 //!< Bit position of DOCK_PWR_EN bit
        static const uint8_t kBitPosSrcPwrEn  = 2u;                 //!< Bit position of SRC_PWR_EN bit
        static const uint8_t kBitPosPsuPwrEn  = 3u;                 //!< Bit position of PSU_PWR_EN bit
        static const uint8_t kBitPosJig3V3Gd  = 4u;                 //!< Bit position of VAA_5V5_EN bit
        static const uint8_t kBitPosJig5V5Gd  = 5u;                 //!< Bit position of VAA_5V5_EN bit

        // FPGA 3 Power Control Bit positions
        static const uint8_t kBitPosMonPwrEn  = 0u;                 //!< Bit position of MON_PWR_EN bit

        comms::RegisterControl* m_pRegs;                            //!< Pointer to the register control object to use

        static bool getPwrEnDetails(PowerSupply supply, comms::RegisterControl::RegDetails& details);
        static bool getPwrGdDetails(PowerSupply supply, comms::RegisterControl::RegDetails& details);
    };
}

#endif // DUTPOWERCONTROL_HPP
