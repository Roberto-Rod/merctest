#include "powermonitor.hpp"
#include "testcasepowermonitor.hpp"
#include "testharnesspowermonitor.hpp"
#include "testjig.hpp"
#include "registercomms.hpp"
#include "testequipmentstore.hpp"
#include "powermonitoradc.hpp"
#include "assyrevision.hpp"

namespace dut
{
    PowerMonitor::PowerMonitor(testjig::TestJig* parentJig) :
        DeviceUnderTest(parentJig)
    {
        m_variants.append(new Variant("Power Monitor", "ASY0020/ASY0021/ASY0130"));  // The generic variant
        m_variants.append(new PowerMonitorVariant("Low-Band Power Monitor", "ASY0020", hardware::RfBand::LOW));
        m_variants.append(new PowerMonitorVariant("Mid-Band Power Monitor", "ASY0021", hardware::RfBand::MID));
        m_variants.append(new PowerMonitorVariant("High-Band Power Monitor", "ASY0130", hardware::RfBand::HIGH));
        m_pRevision = new common::AssyRevision("Null Assembly","");

        m_pTopLevelTestCase = new testsuite::TestCasePowerMonitor(this);
        m_pTestHarness = new testjig::TestHarnessPowerMonitor(parentJig->registerComms(), this);
    }

    PowerMonitor::~PowerMonitor()
    {
    }

    bool PowerMonitor::connect()
    {
        return true;
    }

    void PowerMonitor::disconnect()
    {
    }

    bool PowerMonitor::isConnected() const
    {
        return true;
    }

    DutState PowerMonitor::state() const
    {
        // Only two possible states for this DUT - either No Comms (with jig)
        // or Ready
        DutState retVal = DutState::NO_COMMS;

        if (isConnected())
        {
            retVal = DutState::READY;
        }

        return retVal;
    }

    bool PowerMonitor::waitReady() const
    {
        // No waiting to do - it's ready or not (here I come)
        return (state() == DutState::READY);
    }

    bool PowerMonitor::getReading(uint16_t& millivolts, Direction dir)
    {
        bool retVal = false;

        uint16_t fwd, rev;
        if (static_cast<testjig::TestHarnessPowerMonitor*>(m_pTestHarness)->getAdcReadings(fwd, rev))
        {
            double reading;

            if (dir == Direction::FORWARD) { reading = static_cast<double>(fwd); }
            else                           { reading = static_cast<double>(rev); }

            double mvPerBit = m_adcVariant.getKMillivoltsPerLsb();
            millivolts = static_cast<uint16_t>(mvPerBit * reading);

            retVal = true;
        }

        return retVal;
    }

    void PowerMonitor::setAdcVariant(double current,  hardware::RfBand::Band band)
    {
        bool isHighBand = false;
        if (band == hardware::RfBand::RfBand::HIGH)
        {
            isHighBand = true;
        }
         m_adcVariant.setVoltageRail(current,   isHighBand);
    }

    bool PowerMonitor::isPreRevBIssue3()
    {
        bool result = false;
         if(m_adcVariant.is5Vref())
         {
             result = true;
         }
         return result;
    }

    double PowerMonitor::getADCscaling()
    {
        double mVperBit;
        mVperBit = m_adcVariant.getKMillivoltsPerLsb();
        return mVperBit;
    }

    uint16_t  PowerMonitor::getMinStandbyCurrent(hardware::RfBand::Band band)
    {
        uint16_t minCurrent;
        bool isHighBand = false;
        if(band == hardware::RfBand::Band::HIGH)
        {
             isHighBand = true;
        }
        minCurrent = m_adcVariant.getMinStandbyCurrent(isHighBand);
        return minCurrent;
    }

     uint16_t PowerMonitor::getMaxStandbyCurrent(hardware::RfBand::Band band)
    {
         uint16_t maxCurrent;
         bool isHighBand = false;
         if(band == hardware::RfBand::Band::HIGH)
         {
             isHighBand = true;
         }
        maxCurrent = m_adcVariant.getMaxStandbyCurrent(isHighBand);
        return maxCurrent;
    }

    hardware::RfBand::Band PowerMonitor::band()
    {
        // Default to low band
        hardware::RfBand::Band band = hardware::RfBand::LOW;

        const PowerMonitorVariant* variant = dynamic_cast<const PowerMonitorVariant*>(selectedVariant());

        if (variant != 0)
        {
            band = variant->m_band;
        }

        return band;
    }
}
