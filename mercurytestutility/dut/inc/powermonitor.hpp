#ifndef POWERMONITOR_HPP
#define POWERMONITOR_HPP

#include "deviceundertest.hpp"
#include "rfband.hpp"
#include "powermonitoradc.hpp"

namespace comms
{
    class RegisterControl;
    class RegisterComms;
}

namespace dut
{
    struct PowerMonitorVariant : public Variant
    {
        PowerMonitorVariant(QString name, QString number, hardware::RfBand::Band band) :
            Variant(name, number),
            m_band(band)
        {
        }

        ~PowerMonitorVariant() {}

        hardware::RfBand::Band m_band;
    };

    class PowerMonitor : public DeviceUnderTest
    {
        Q_OBJECT

    public:
        enum class Direction { FORWARD, REVERSE };

        PowerMonitor(testjig::TestJig* parentJig = 0);

        ~PowerMonitor();

        bool connect();

        void disconnect();

        bool isConnected() const;

        DutState state() const;

        bool waitReady() const;

        bool getReading(uint16_t& millivolts, Direction dir);

        hardware::RfBand::Band band();

        void setAdcVariant(double current, hardware::RfBand::Band band);

        double getADCscaling();

        uint16_t getMinStandbyCurrent(hardware::RfBand::Band band);

        uint16_t getMaxStandbyCurrent(hardware::RfBand::Band band);

        bool isPreRevBIssue3();

    private:
        static constexpr double kMillivoltsPerLsb = (5000.0 / 4096.0);
        hardware::powerMonitorAdc  m_adcVariant;
    };
}

#endif // POWERMONITOR_HPP
