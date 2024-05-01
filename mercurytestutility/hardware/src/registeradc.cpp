#include "registeradc.hpp"
#include "registercomms.hpp"

namespace hardware
{
    RegisterAdc::RegisterAdc(comms::RegisterControl* pRegs, uint16_t addr) :
        m_pRegs(pRegs),
        m_addr(addr),
        m_averages(kAveragesDefault)
    {
    }

    bool RegisterAdc::getReading(int16_t& millivolts, bool zeroNegativeResults)
    {
        bool retVal = true;

        double accum = 0u;

        for (uint8_t i = 0; (i < m_averages) && retVal; i++)
        {
            // Get a new reading
            uint32_t data;
            retVal &= m_pRegs->readRegister(m_addr, data);

            int16_t reading = static_cast<int16_t>(data);

            // The LTC2991 wraps around when negative voltages are read and with an LSB weight
            // of 305.18uV it can easily wrap around to 16383 (F.S.) when an input pin is grounded.
            // To prevent this, assume results within top 250mV are negative. If caller has asked
            // for positive only readings then zero out negative voltages.
            if (reading >= 15565)
            {
                if (zeroNegativeResults)
                {
                    reading = 0;
                }
                else
                {
                     reading -= 16384;
                }
            }

            // Add to accumulator
            accum += static_cast<double>(reading);
        }

        if (retVal)
        {
            // LTC2991: Voltage = D[13:0] * 305.18uV
            double result = std::round((accum / m_averages) * (30518.0 / 1e5));

            millivolts = static_cast<int16_t>(result);
        }

        return retVal;
    }

    bool RegisterAdc::getReading(uint16_t& millivolts)
    {
        int16_t reading;

        // Get ADC reading and zero out negative results
        bool retVal = getReading(reading, true);

        millivolts = static_cast<uint16_t>(reading);

        return retVal;
    }

}
