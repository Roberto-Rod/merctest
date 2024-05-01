#ifndef CTSBOARD_HPP
#define CTSBOARD_HPP

#include "deviceundertest.hpp"

// Forward declaration
namespace comms { class CtsBoardComms; }

namespace dut
{
    class CtsBoard : public DeviceUnderTest
    {
        Q_OBJECT
    public:
        struct BitStatus
        {
            bool allOk() const;
            void clear();

            bool m_procInitOk { false };
            bool m_bspOk { false };
            bool m_synthOk { false };
            bool m_flashMissionFileOk { false };
            bool m_missionOk { false };
            bool m_tamperDeviceOk { false };
            bool m_internalFlashOk { false };
        };

        enum class ChargeStatus : uint8_t
        {
            Unknown = 0,
            Charging,
            NearlyComplete,
            Complete
        };

        enum class Button : uint8_t
        {
            Power = 0,
            Scan,
            Exclamation,
            Cross
        };

        enum class Led : uint8_t
        {
            Power = 0,
            Battery,
            ScanRed,
            ScanGreen,
            ExclamationRed,
            ExclamationGreen,
            CrossRed,
            CrossGreen,
        };

        enum class RfPath : uint8_t
        {
            Path0 = 0,
            Path1,
            Path2,
            Path3,
            Path4,
            Path5
        };

        enum class IfPath : uint8_t
        {
            If900 = 0,
            If1900,
            IfNearZero
        };

        enum class TamperChannel : uint8_t
        {
            Microswitch = 1,
            LightSensor = 2,
            Both = 3
        };

        CtsBoard(testjig::TestJig* parentJig = 0);

        ~CtsBoard();

        bool connect();

        void disconnect();

        bool isConnected() const;

        DutState state() const;

        bool waitReady() const;

        bool getBitStatus(BitStatus& status) const;

        bool getVersion(QString& version) const;

        bool getBatteryVoltage(uint16_t& millivolts) const;

        bool getTamperVoltage(uint16_t& millivolts) const;

        bool getProcessorTemperature(int16_t& degreesC) const;

        bool getChargeStatus(ChargeStatus& status) const;

        bool getChargerFaultStatus(bool& fault) const;

        bool getRfPowerReading(uint16_t& millivolts) const;

        bool getRfPowerReading(float& dBm) const;

        bool getTamperStatus(TamperChannel ch, bool& tampered) const;

        bool getUsbPowerStatus(bool& present) const;

        bool setInterfaceOverride(bool override) const;

        bool setInterfaceLed(Led led) const;

        bool isButtonPressed(Button button) const;

        bool resetTamperDevice() const;

        bool setRfPath(RfPath path) const;

        bool setIfPath(IfPath path) const;

        bool setRfFrequency(uint32_t freq_kHz) const;

        bool setLoFrequency(uint32_t freq_kHz) const;

        bool enableRfPower(bool en) const;

        bool writeSerialNumber(uint32_t sn) const;

        bool readSerialNumber(uint32_t& sn) const;

        bool killDcPower() const;

    private:
        comms::CtsBoardComms* m_pDutComms;
    };
}

#endif // CTSBOARD_HPP
