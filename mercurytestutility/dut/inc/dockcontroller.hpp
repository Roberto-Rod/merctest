#ifndef DOCKCONTROLLER_HPP
#define DOCKCONTROLLER_HPP

#include "deviceundertest.hpp"
#include "port.hpp"

#include <QMap>

namespace comms
{
    class DockControllerComms;
}

namespace hardware
{
    class PaControlMonitor;
}

namespace dut
{
    class DockController : public DeviceUnderTest
    {
        Q_OBJECT

    public:
        enum BitVoltage
        {
            VOLTS_IN,
            GPS_3V0,
            INT_3V3,
            INT_5V5
        };

        DockController(testjig::TestJig* parentJig = 0);

        ~DockController();

        bool connect();

        void disconnect();

        bool isConnected() const;

        DutState state() const;

        bool waitReady() const;

        bool getFwVersion(QString& vers, hardware::Port port = hardware::Port::A) const;

        bool getHwVersion(QString& vers, hardware::Port port = hardware::Port::A) const;

        bool getDrawingNumber(uint16_t& num, hardware::Port port = hardware::Port::A) const;

        bool getClockCount1Pps(uint32_t& count, hardware::Port port = hardware::Port::A) const;

        bool getEepromMfrDevId(uint16_t& id, hardware::Port port = hardware::Port::A) const;

        bool getEepromUniqueId(uint32_t& id, hardware::Port port = hardware::Port::A) const;

        bool getBitStatus(uint16_t& status, hardware::Port port = hardware::Port::A) const;

        bool getBitVoltage(BitVoltage src, uint16_t& millivolts, hardware::Port port = hardware::Port::A) const;

        bool getBitCurrent(uint16_t& milliamps, hardware::Port port = hardware::Port::A) const;

        bool getBitTemperature(int8_t& degC, hardware::Port port = hardware::Port::A) const;

        hardware::PaControlMonitor* paCtrlMonitor(hardware::Port port) const;

    private:
        static const uint16_t kDockCh1Offset         = 0x5000;       //!< Dock Channel 1 offset within test jig register space
        static const uint16_t kDockCh2Offset         = 0x6000;       //!< Dock Channel 2 offset within test jig register space

        static const uint16_t kRegAddrFwVers         = 0x0000;       //!< Firmware version register address
        static const uint16_t kRegAddrHwVers         = 0x0001;       //!< Hardware version register address
        static const uint16_t kRegAddrGpioData       = 0x0002;       //!< GPIO data register address
        static const uint16_t kRegAddrGpioDir        = 0x0003;       //!< GPIO direction register address
        static const uint16_t kRegAddrGpoFlashRate   = 0x0004;       //!< GPO flash rate register address
        static const uint16_t kRegAddrClkCnt1Pps     = 0x0005;       //!< Clock count 1PPS register address
        static const uint16_t kRegAddrEepromMfrId    = 0x000D;       //!< EEPROM Manufacturer/Device ID register address
        static const uint16_t kRegAddrEepromUId      = 0x000E;       //!< EEPROM Unique ID register address
        static const uint16_t kRegAddrBitStatus      = 0x000F;       //!< BIT status register address
        static const uint16_t kRegAddrAdcVIn         = 0x0010;       //!< ADC: Input Voltage register address
        static const uint16_t kRegAddrAdcCurrIn      = 0x0011;       //!< ADC: Input Current register address
        static const uint16_t kRegAddrAdc3V0Gps      = 0x0012;       //!< ADC: 3V0 GPS supply register address
        static const uint16_t kRegAddrAdc3V3Int      = 0x0013;       //!< ADC: 3V3 internal rail register address
        static const uint16_t kRegAddrAdc5V5Int      = 0x0014;       //!< ADC: 5V5 internal rail register address
        static const uint16_t kRegAddrAdcTemp        = 0x0015;       //!< ADC: temperature register address
        static const uint16_t kRegAddrPaCtrl         = 0x0170;       //!< PA control register address
        static const uint16_t kRegAddrPwrMon         = 0x0174;       //!< Power monitor register address
        static const uint16_t kRegAddrI2cCtrl        = 0x01F0;       //!< I2C control register address
        static const uint16_t kRegAddrI2cBase        = 0x0200;       //!< I2C registers base address

        static const uint8_t kWidthDockRegs          = 32u;          //!< Width of dock registers

        static const uint8_t kHwMajLsb               = 24u;          //!< Hardware major version field LSB
        static const uint8_t kHwMinLsb               = 16u;          //!< Hardware minor version field LSB
        static const uint8_t kHwDwgNumLsb            = 0u;           //!< FPGA drawing number field LSB
        static const uint8_t kHwMajWidth             = 8u;           //!< Hardware major version field width
        static const uint8_t kHwMinWidth             = 8u;           //!< Hardware minor version field width
        static const uint8_t kHwDwgNumWidth          = 16u;          //!< FPGA drawing number field width

        static const uint8_t kFwMajLsb               = 24u;          //!< Firmware major version field LSB
        static const uint8_t kFwMinLsb               = 16u;          //!< Firmware minor version field LSB
        static const uint8_t kFwBuildLsb             = 0u;           //!< Firmware build field LSB
        static const uint8_t kFwMajWidth             = 8u;           //!< Firmware major version field width
        static const uint8_t kFwMinWidth             = 8u;           //!< Firmware minor version field width
        static const uint8_t kFwBuildWidth           = 16u;          //!< Firmware build field width

        static constexpr double kScalingVIn          = (302.0 / 22.0) * 305.18e-3;    //!< Scaling from ADC value to millivolts, input voltage
        static constexpr double kScalingVInt         = (2.0)          * 305.18e-3;    //!< Scaling from ADC value to millivolts, internal rails
        static constexpr double kScalingCurr         = (1.0 / 1.25)   * 305.18e-3;    //!< Scaling from ADC value to milliamps
        static constexpr double kScalingTemp         = (1.0 / 16.0);                  //!< Scaling from ADC value to degrees C

        comms::DockControllerComms* m_pDockComms[2];                 //!< Dock controller comms (2 channels)

        hardware::PaControlMonitor* m_pPaCtrlMon[2];                 //!< Pointer to PA Control/Monitor objects

        QMap<hardware::Port, int> m_portMap;                       //!< Map enumerated port to comms object index
    };
}

#endif // DOCKCONTROLLER_HPP
