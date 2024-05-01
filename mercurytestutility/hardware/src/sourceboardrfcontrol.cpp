#include "sourceboardrfcontrol.hpp"
#include <cmath>
#include <climits>

namespace hardware
{
    SourceBoardRfControl::SourceBoardRfControl(uint64_t outputFreq, RfBand band) :
        m_outputFreq(outputFreq),
        m_synthFreq(kSynthFreqDefault),
        m_rfBand(band)
    {
    }

    bool SourceBoardRfControl::setOutputFreq(uint64_t freq)
    {
        bool retVal = false;

        if ((freq >= kLowBandFreqMin) && (freq <= kHighBandFreqMax))
        {
            m_outputFreq = freq;
            retVal = true;
        }

        return retVal;
    }

    bool SourceBoardRfControl::setSynthFreq(uint64_t freq)
    {
        bool retVal = false;

        if ((freq >= kSynthFreqMin) && (freq <= kSynthFreqMax))
        {
            m_synthFreq = freq;
            retVal = true;
        }

        return retVal;
    }

    void SourceBoardRfControl::setBand(RfBand band)
    {
        m_rfBand = band;
    }

    bool SourceBoardRfControl::isValidBand() const
    {
        bool retVal = false;

        if ((m_rfBand == LOW) &&
             (m_outputFreq >= kLowBandFreqMin) &&
             (m_outputFreq <= kLowBandFreqMax))
        {
            retVal = true;
        }
        else if ((m_rfBand == MID) &&
                  (m_outputFreq >= kMidBandFreqMin) &&
                  (m_outputFreq <= kMidBandFreqMax))
        {
            retVal = true;
        }
        else if ((m_rfBand == HIGH) &&
                  (m_outputFreq >= kHighBandFreqMin) &&
                  (m_outputFreq <= kHighBandFreqMax))
        {
            retVal = true;
        }

        return retVal;
    }

    uint32_t SourceBoardRfControl::getDdsFtw() const
    {
        // DDS frequency tuning word = f_out * 2^32 / f_dac
        // Perform multiplication in 64-bit uint first to maintain precision
        uint64_t retVal = getDdsFreq() * (1ull + ULONG_MAX);
        retVal /= m_synthFreq;

        // Test that the output fits into 32-bits. Reset to zero if it doesn't to
        // indicate error
        uint64_t max = ULONG_MAX;
        if (retVal > max)
        {
            retVal = 0;
        }

        return static_cast<uint32_t>(retVal);
    }

    uint64_t SourceBoardRfControl::getDdsFreq() const
    {
        // Initialise DDS frequency to output frequency
        uint64_t ddsFreq = m_outputFreq;

        // If this is mid-band module and we are not using bypass path
        // then halve the DDS frequency as the doubler is used
        if ((m_rfBand == MID) && (getDoublerPath() > 0))
        {
            ddsFreq /= 2;
        }

        return ddsFreq;
    }

    uint8_t SourceBoardRfControl::getDoublerPath() const
    {
        uint8_t retVal = 0u;

        // Select the lowest path which passes the output frequency
        if (m_outputFreq >= kDoublerPath1Min)
        {
            if (m_outputFreq <= kDoublerPath1Max)
            {
                retVal = 0u;    // Path 1
            }
            else if (m_outputFreq <= kDoublerPath2Max)
            {
                retVal = 1u;    // Path 2
            }
            else if (m_outputFreq <= kDoublerPath3Max)
            {
                retVal = 2u;    // Path 3
            }
            else if (m_outputFreq <= kDoublerPath4Max)
            {
                retVal = 3u;    // Path 4
            }
            else if (m_outputFreq <= kDoublerPath5Max)
            {
                retVal = 4u;    // Path 5
            }
        }

        return retVal;
    }

    SourceBoardRfControl::RfBand SourceBoardRfControl::getBand(uint8_t daughterId)
    {
        RfBand retVal = INVALID;

        if (daughterId == 0xf)
        {
            retVal = LOW;
        }
        else if (daughterId == 0x0)
        {
            retVal = MID;
        }
        else if (daughterId == 0x1)
        {
            retVal = HIGH;
        }

        return retVal;
    }

    uint32_t SourceBoardRfControl::quarterDbToAsf(uint8_t quarterDb)
    {
        // ratio = 10^((quarterdB/-4) / 20))
        double ratio = pow(10.0, (static_cast<double>(quarterDb) / -4.0 / 20.0));

        // Amplitide scale factor = 4095 * ratio
        uint32_t asf = static_cast<uint32_t>((ratio * 4095.0) + 0.5);

        return asf;
    }
}
