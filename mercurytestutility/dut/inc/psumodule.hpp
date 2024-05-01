#ifndef PSUMODULE_HPP
#define PSUMODULE_HPP

#include "deviceundertest.hpp"
#include "psucomms.hpp"

namespace comms
{
    class RegisterControl;
    class RegisterComms;
}

namespace dut
{
    enum class PsuModuleType
    {
        UNKNOWN,
        MANPACK,
        VEHICLE
    };

    struct PsuModuleVariant : public Variant
    {
        PsuModuleVariant(QString name, QString number, PsuModuleType type) :
            Variant(name, number),
            m_type(type)
        {
        }

        ~PsuModuleVariant() {}

        PsuModuleType m_type;
    };

    class PsuModule : public DeviceUnderTest
    {
        Q_OBJECT

    public:
        enum VoltageSource
        {
            INPUT,
            OUTPUT,
            RAIL_3V3,
            RAIL_12V
        };

        enum Fan
        {
            FAN_1,
            FAN_2
        };

        enum TemperatureSource
        {
            LOCAL = 0,
            REMOTE,
            THERMISTOR
        };

        enum ShutdownMode
        {
            PA_SHDN_1 = 0,
            PA_SHDN_0,
            HOST_PA_SHDN
        };

        PsuModule(testjig::TestJig* parentJig = 0);

        ~PsuModule();

        bool connect();

        void disconnect();

        bool isConnected() const;

        DutState state() const;

        bool waitReady() const;

        PsuModuleType type() const;

        bool getSwVersion(uint8_t& major, uint8_t& minor) const;

        bool getType(PsuModuleType& type) const;

        bool setPsuEnable(bool en) const;

        bool setVoltage(uint16_t millivolts) const;

        bool setFanEnable(Fan fan, bool en) const;

        bool setFanEnable(bool en) const;

        bool setFanManual(Fan fan, bool manual) const;

        bool setFanManual(bool manual) const;

        bool setShutdownMode(ShutdownMode mode) const;

        bool readVoltage(VoltageSource source, uint16_t& millivolts) const;

        bool readCurrent(uint16_t& milliamps) const;

        bool readTemperature(TemperatureSource source, int8_t& degreesC) const;

        bool readFanTacho(Fan fan, uint8_t& tachoReading) const;

        bool enableFan1Alarm(bool en) const;

        bool clearAlarms() const;

    private:
        // Register Addresses
        static const uint8_t kRegAddrControl     = 0x00;
        static const uint8_t kRegAddrFan1Ctrl    = 0x01;
        static const uint8_t kRegAddrFan2Ctrl    = 0x02;
        static const uint8_t kRegAddrAlertCtrl2  = 0x05;
        static const uint8_t kRegAddrVOutSetMsb  = 0x06;
        static const uint8_t kRegAddrVOutSetLsb  = 0x07;
        static const uint8_t kRegAddrAlarm1      = 0x40;
        static const uint8_t kRegAddrVInMeasMsb  = 0x44;
        static const uint8_t kRegAddrVInMeasLsb  = 0x45;
        static const uint8_t kRegAddrVOutMeasMsb = 0x46;
        static const uint8_t kRegAddrVOutMeasLsb = 0x47;
        static const uint8_t kRegAddr3V3Meas     = 0x48;
        static const uint8_t kRegAddr12VMeas     = 0x49;
        static const uint8_t kRegAddrCurrMeas    = 0x4A;
        static const uint8_t kRegAddrTempLocal   = 0x4B;
        static const uint8_t kRegAddrTempRemote  = 0x4C;
        static const uint8_t kRegAddrThermistor  = 0x4D;
        static const uint8_t kRegAddrTacho1      = 0x4E;
        static const uint8_t kRegAddrTacho2      = 0x4F;
        static const uint8_t kRegAddrSwVersion   = 0x60;
        static const uint8_t kRegAddrReset       = 0x7F;

        // Control Register Bit Definitions
        static const uint8_t kControlPsuEnBit    = 0u;          //!< PSU Enable bit position
        static const uint8_t kControlShdnLsb     = 2u;          //!< SHDN Control bit position

        static const uint8_t kControlShdnWidth   = 2u;          //!< SHDN Control width

        // Fan Control Register Bit Definitions
        static const uint8_t kFanCtrlFanEnBit    = 0u;          //!< Fan Enable bit position
        static const uint8_t kFanCtrlManualBit   = 1u;          //!< Fan Manual/Auto bit position
        static const uint8_t kFanCtrlTachoEnBit  = 2u;          //!< Tacho Enable bit position
        static const uint8_t kFanCtrlSensSelLsb  = 3u;          //!< Sensor Select bit position

        static const uint8_t kFanCtrlSensSelWidth = 2u;          //!< Sensor Select width

        // Alarm register bit definitions
        static const uint8_t kAlertCtrl2Fan1     = 0u;          //!< Fan 1 fail bit

        // Software Version Register Bit Definitions
        static const uint8_t kSwVersMinLsb       = 0u;          //!< Software Version minor digit LSB position
        static const uint8_t kSwVersMajLsb       = 4u;          //!< Software Version minor digit MSB position
        static const uint8_t kSwVersTypeBit      = 7u;          //!< Board Type bit position

        static const uint8_t kSwVersMinWidth     = 4u;          //!< Software Version minor digit width
        static const uint8_t kSwVersMajWidth     = 3u;          //!< Software Version minor digit width

        // Other Register Defs
        static const uint8_t kResetMagicNumber   = 0x34;        //!< Magic number used to reset PSU controller registers
        static const uint8_t kPsuRegsWidth       = 8u;          //!< Width of PSU registers

        // Voltage/Current Scaling
        static const uint16_t kVoltsOutSetScale  = 50u;         //!< Output voltage setting scale: 1 LSB = 50 mV
        static const uint16_t kVoltsOutMeasScale = 50u;         //!< Output voltage measurement scale: 1 LSB = 50 mV
        static const uint16_t kVoltsInMeasScale  = 50u;         //!< Input voltage measurement scale: 1 LSB = 50 mV
        static const uint16_t kRail3V3MeasScale  = 20u;         //!< 3V3 Rail voltage measurement scale: 1 LSB = 20 mV
        static const uint16_t kRail12VMeasScale  = 100u;        //!< 12V Rail voltage measurement scale: 1 LSB = 100 mV
        static const uint16_t kCurrOutMeasScale  = 50u;         //!< Output current measurement scale: 1 LSB = 50 mA

        // Other settings
        static const uint32_t kResetPostDelay    = 1ul;         //!< Delay after sending register reset command, seconds

        comms::PsuComms* m_pPsuComms;                           //!< Pointer to PSU comms object

        static uint16_t millivoltsToRegister(uint16_t millivolts, uint16_t millivoltsPerLsb);
        static uint16_t registerToMillivolts(uint16_t regVal, uint16_t millivoltsPerLsb);
    };
}

#endif // PSUMODULE_HPP
