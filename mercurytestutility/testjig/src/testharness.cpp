#include "testharness.hpp"
#include "registercontrol.hpp"
#include "registercomms.hpp"
#include "registeradc.hpp"
#include "testjig.hpp"

#include <QThread>

namespace testjig
{
    TestHarness::TestHarness(comms::RegisterControl* pJigRegs, QObject* parent) :
        QObject(parent),
        m_pJigRegs(pJigRegs)
    {
    }

    void TestHarness::powerOff() const
    {
        enablePower(false);
    }

    void TestHarness::powerOn() const
    {
        enablePower(true);
    }

    bool TestHarness::powerCycle(uint8_t delaySecs) const
    {
        bool retVal = false;

        // Disable power (even if it's already disabled - just run through method again
        if (enablePower(false))
        {
            // Sleep for number of seconds
            QThread::sleep(static_cast<unsigned long>(delaySecs));

            // Enable power
            if (enablePower(true))
            {
                retVal = true;
            }
        }

        return retVal;
    }

    bool TestHarness::inputCurrent(uint16_t& milliamps, uint16_t regAddr, double transcond) const
    {
        bool retVal = false;

        // Check if the power supply is enabled. If it is not enabled then a reliable
        // current reading cannot be gained as turning the PFET off leaves the current
        // sense resistor floating and the current sense amplifier can swing up to full-scale
        bool enabled = false;

        if (isPowerEnabled(enabled))
        {
            // If the supply is not enabled then just return 0 milliamps
            if (!enabled)
            {
                milliamps = 0u;
                retVal = true;
            }
            else
            {
                // Power supply is enabled - get a reading
                hardware::RegisterAdc adc(m_pJigRegs, regAddr);

                // But first wait for the settling time
                QThread::msleep(kSettleTime);

                uint16_t millivolts;
                if (adc.getReading(millivolts))
                {
                    milliamps = static_cast<uint16_t >(round(static_cast<double>(millivolts) * transcond));
                    retVal = true;
                }
            }
        }

        return retVal;
    }
}
