#ifndef SOURCEBOARDRFCONTROL_HPP
#define SOURCEBOARDRFCONTROL_HPP

#include "stdint.h"

namespace hardware
{
    class SourceBoardRfControl
    {
    public:
        enum RfBand { LOW, MID, HIGH, INVALID };

        SourceBoardRfControl(uint64_t outputFreq = kLowBandFreqMin , RfBand band = LOW);

        bool setOutputFreq(uint64_t freq);

        bool setSynthFreq(uint64_t freq);

        void setBand(RfBand band);

        bool isValidBand() const;

        uint32_t getDdsFtw() const;

        uint64_t getDdsFreq() const;

        uint8_t getDoublerPath() const;

        static RfBand getBand(uint8_t daughterId);

        static uint32_t quarterDbToAsf(uint8_t quarterDb);

    private:
        // Constants
        static const uint64_t kLowBandFreqMin    = 20000000;
        static const uint64_t kLowBandFreqMax    = 520000000;
        static const uint64_t kMidBandFreqMin    = 500000000;
        static const uint64_t kMidBandFreqMax    = 2700000000;
        static const uint64_t kHighBandFreqMin   = 2500000000;
        static const uint64_t kHighBandFreqMax   = 6000000000;
        static const uint64_t kSynthFreqMin      = 2500000000;
        static const uint64_t kSynthFreqMax      = 3500000000;
        static const uint64_t kSynthFreqDefault  = 3240000000;
        static const uint64_t kDoublerPath1Min   = 400000000;
        static const uint64_t kDoublerPath1Max   = 1500000000;
        static const uint64_t kDoublerPath2Min   = 1490000000;
        static const uint64_t kDoublerPath2Max   = 1880000000;
        static const uint64_t kDoublerPath3Min   = 1850000000;
        static const uint64_t kDoublerPath3Max   = 2250000000;
        static const uint64_t kDoublerPath4Min   = 2250000000;
        static const uint64_t kDoublerPath4Max   = 2500000000;
        static const uint64_t kDoublerPath5Min   = 2500000000;
        static const uint64_t kDoublerPath5Max   = 2700000000;
        static const uint64_t kDoublerPath6Min   = 2700000000;
        static const uint64_t kDoublerPath6Max   = 3000000000;

        // Define 2^32 as 64-bit unsigned to avoid needing math library
        static const uint64_t k2pow32            = 4294967296;

        // Member variables
        uint64_t m_outputFreq;
        uint64_t m_synthFreq;
        RfBand m_rfBand;
    };
}

#endif // SOURCEBOARDRFCONTROL_HPP
