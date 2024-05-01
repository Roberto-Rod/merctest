#ifndef TESTCASEPOWERMONITORDIRECTIVITY_HPP
#define TESTCASEPOWERMONITORDIRECTIVITY_HPP

#include "testcasepowermonitor.hpp"

namespace testsuite
{
    class TestCasePowerMonitorDirectivity : public TestCasePowerMonitor
    {
        Q_OBJECT

    public:
        TestCasePowerMonitorDirectivity(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
         static const  double KCouplerSlopeAdjustment;               // adjustment per Mhz for frequencies > 3.6GHz
                                                                     // 0.125 mV loss in power per MHz when > 3.6GHz

        static const uint64_t kLowBandStartFreq   =   20e6;          //!< Low band start frequency (Hz)
        static const uint64_t kLowBandStopFreq    =  520e6;          //!< Low band stop frequency (Hz)
        static const uint64_t kLowBandStepFreq    =    5e6;          //!< Low band step frequency (Hz)

        static const uint64_t kMidBandStartFreq   =  500e6;          //!< Mid band start frequency (Hz)
        static const uint64_t kMidBandStopFreq    = 2700e6;          //!< Mid band stop frequency (Hz)
        static const uint64_t kMidBandStepFreq    =   22e6;          //!< Mid band step frequency (Hz)

        static const uint64_t kHighBandStartFreq  = 2400e6;          //!< High band start frequency (Hz)
        //static const uint64_t kHighBandStopFreq   = 6000e6;          //!< High band stop frequency (Hz)
        //static const uint64_t kHighBandStopFreq   = 5800e6;          //!< High band stop frequency (Hz)
        static const uint64_t kHighBandStopFreq   = 5770e6;          //!< High band stop frequency (Hz)
        static const uint64_t kHighBandStepFreq   =   30e6;          //!< High band step frequency (Hz)

        /* Notes from Power Monitor hardware designer (LMP):
         * The ...DiffMin value reflects the hard lower limit for a Low Band (1000mV = ~20dB
         * return loss) and a Mid Band (750mV = ~15dB return loss) – these min values
         * should never be relaxed.  The ...GoodPctMin value is the limit that can flex if the
         * manufacturer experiences the odd failure.
         */

        static const uint16_t kLowBandTermFwdMin5V0 = 1300;          //!< Low band, terminated, forward reading, min (mV)
        static const uint16_t kLowBandTermFwdMax5V0 = 1600;          //!< Low band, terminated, forward reading, max (mV)
        static const uint16_t kLowBandTermRevMin5V0 = 0;             //!< Low band, terminated, reverse reading, min (mV)
        static const uint16_t kLowBandTermRevMax5V0 = 900;           //!< Low band, terminated, reverse reading, max (mV)
        static const uint16_t kLowBandOpenFwdMin5V0 = 1300;          //!< Low band, terminated, forward reading, min (mV)
        static const uint16_t kLowBandOpenFwdMax5V0 = 1600;          //!< Low band, terminated, forward reading, max (mV)
        static const uint16_t kLowBandOpenRevMin5V0 = 1300;          //!< Low band, terminated, reverse reading, min (mV)
        static const uint16_t kLowBandOpenRevMax5V0 = 1600;          //!< Low band, terminated, reverse reading, max (mV)
        static const int32_t  kLowBandDiffMin5V0    = 900;           //!< Low band, difference (forward-reverse), min for "good" point (mV)
        static const uint8_t  kLowBandGoodPctMin5V0 = 75;            //!< Low band, minium percentage of "good" difference points

        static const uint16_t kMidBandTermFwdMin5V0 = 1300;          //!< Mid band, terminated, forward reading, min (mV)
        static const uint16_t kMidBandTermFwdMax5V0 = 1700;          //!< Mid band, terminated, forward reading, max (mV)
        static const uint16_t kMidBandTermRevMin5V0 = 0;             //!< Mid band, terminated, reverse reading, min (mV)
        static const uint16_t kMidBandTermRevMax5V0 = 1200;          //!< Mid band, terminated, reverse reading, max (mV)
        static const uint16_t kMidBandOpenFwdMin5V0 = 1260;          //!< Mid band, terminated, forward reading, min (mV)
        static const uint16_t kMidBandOpenFwdMax5V0 = 1710;          //!< Mid band, terminated, forward reading, max (mV)
        static const uint16_t kMidBandOpenRevMin5V0 = 1200;          //!< Mid band, terminated, reverse reading, min (mV)
        static const uint16_t kMidBandOpenRevMax5V0 = 1700;          //!< Mid band, terminated, reverse reading, max (mV)
        static const int32_t  kMidBandDiffMin5V0    = 700;           //!< Mid band, difference (forward-reverse), min for "good" point (mV)
        static const uint8_t  kMidBandGoodPctMin5V0 = 75;            //!< Mid band, minium percentage of "good" difference points  */

        //  There is no High band 5V variant

        static const uint16_t kLowBandTermFwdMin3V3 = 800;           //!< Low band, terminated, forward reading, min (mV)
        static const uint16_t kLowBandTermFwdMax3V3 = 1100;          //!< Low band, terminated, forward reading, max (mV)
        static const uint16_t kLowBandTermRevMin3V3 = 50;            //!< Low band, terminated, reverse reading, min (mV)
        static const uint16_t kLowBandTermRevMax3V3 = 620;           //!< Low band, terminated, reverse reading, max (mV)
        static const uint16_t kLowBandOpenFwdMin3V3 = 800;           //!< Low band, terminated, forward reading, min (mV)
        static const uint16_t kLowBandOpenFwdMax3V3 = 1100;          //!< Low band, terminated, forward reading, max (mV)
        static const uint16_t kLowBandOpenRevMin3V3 = 800;           //!< Low band, terminated, reverse reading, min (mV)
        static const uint16_t kLowBandOpenRevMax3V3 = 1100;          //!< Low band, terminated, reverse reading, max (mV)
        static const int32_t  kLowBandDiffMin3V3    = 480;           //!< Low band, difference (forward-reverse), min for "good" point (mV)
        static const uint8_t  kLowBandGoodPctMin3V3 = 75;            //!< Low band, minium percentage of "good" difference points

        static const uint16_t kMidBandTermFwdMin3V3 = 680;           //!< Mid band, terminated, forward reading, min (mV)
        static const uint16_t kMidBandTermFwdMax3V3 = 1180;          //!< Mid band, terminated, forward reading, max (mV)
        static const uint16_t kMidBandTermRevMin3V3 = 50;            //!< Mid band, terminated, reverse reading, min (mV)
        static const uint16_t kMidBandTermRevMax3V3 = 792;           //!< Mid band, terminated, reverse reading, max (mV)
        static const uint16_t kMidBandOpenFwdMin3V3 = 680;           //!< Mid band, terminated, forward reading, min (mV)
        static const uint16_t kMidBandOpenFwdMax3V3 = 1180;          //!< Mid band, terminated, forward reading, max (mV)
        static const uint16_t kMidBandOpenRevMin3V3 = 680;           //!< Mid band, terminated, reverse reading, min (mV)
        static const uint16_t kMidBandOpenRevMax3V3 = 1180;          //!< Mid band, terminated, reverse reading, max (mV)
        static const int32_t  kMidBandDiffMin3V3    = 460;           //!< Mid band, difference (forward-reverse), min for "good" point (mV)
        static const uint8_t  kMidBandGoodPctMin3V3 = 75;            //!< Mid band, minium percentage of "good" difference points

        // Sig Gen requires high power option, capable of outputting 19dB @6GHz
        // Sig Gen cable reduced in length to 6" to give negligible loss even at 6GHz.
        // Rf power compensation for sig gen cable no longer required.
        // Max power output over all frequencies (19dB).
        // High Rf power ensures that power monitor  is tested over the linear part of the power monitor range.
        // The no Rf power value (forward or revers) is subtracted from all power measurements.

        static constexpr double kTestPowerLevel    = 15.0;           // Low & Mid band power level
        static constexpr double kTestPowerLevelHB  = 19.0;           // High Band power level

        static const uint16_t kFwdTermFwdRevOpenmV = kTestPowerLevelHB * 30;  // Slope of 30mV per dB (circuit design)
        static const uint16_t kRevTermmV = kTestPowerLevelHB * 12;            // Slope of 12mV per dB (measured from graph plot)

        // Limits are divided into frequency ranges (high frequency Rf!!)
        static const uint16_t kFwdTermTollLowRangemV  =  6.7 * 30;
        static const uint16_t kRevTermTollLowRangemV  =  9.0 * 12;
        static const uint16_t kFwdTermTollHighRangemV =  7.3 * 30;
        static const uint16_t kRevTermTollHighRangemV =  9.0 * 12;

        static const uint16_t kFwdOpenTollLowRangemV  =  6.3 * 30;
        static const uint16_t kRevOpenTollLowRangemV  =  5.8 * 30;
        static const uint16_t kFwdOpenTollMidRangemV  =  5.8 * 30;
        static const uint16_t kRevOpenTollMidRangemV  =  6.1 * 30;
        static const uint16_t kFwdOpenTollHighRangemV = 10.0 * 30;
        static const uint16_t kRevOpenTollHighRangemV =  7.5 * 30;

        // Terminated Power Meter limits

        // Low Frequency Range
        static const uint16_t kHighBandTermFwdMin3G6  = kFwdTermFwdRevOpenmV - kFwdTermTollLowRangemV;
        static const uint16_t kHighBandTermFwdMax3G6  = kFwdTermFwdRevOpenmV + kFwdTermTollLowRangemV;
        static const uint16_t kHighBandTermRevMin3G6  = 0;
        static const uint16_t kHighBandTermRevMax3G6  = kRevTermmV + kRevTermTollLowRangemV;

        // High Frequency Range
        static const uint16_t kHighBandTermFwdMin5G5  = kFwdTermFwdRevOpenmV - kFwdTermTollHighRangemV;
        static const uint16_t kHighBandTermFwdMax5G5  = kFwdTermFwdRevOpenmV + kFwdTermTollHighRangemV;
        static const uint16_t kHighBandTermRevMin5G5  = 0;
        static const uint16_t kHighBandTermRevMax5G5  = kRevTermmV + kRevTermTollHighRangemV;

        // Open Circuit Power Meter  limits

        //  Low Frequency range
        static const uint16_t kHighBandOpenFwdMin2G79 = kFwdTermFwdRevOpenmV - kFwdOpenTollLowRangemV;
        static const uint16_t kHighBandOpenFwdMax2G79 = kFwdTermFwdRevOpenmV + kFwdOpenTollLowRangemV;
        static const uint16_t kHighBandOpenRevMin2G79 = kFwdTermFwdRevOpenmV - kRevOpenTollLowRangemV;
        static const uint16_t kHighBandOpenRevMax2G79 = kFwdTermFwdRevOpenmV + kRevOpenTollLowRangemV;

        // Mid Frequency Range
        static const uint16_t kHighBandOpenFwdMin3G6  = kFwdTermFwdRevOpenmV - kFwdOpenTollMidRangemV;
        static const uint16_t kHighBandOpenFwdMax3G6  = kFwdTermFwdRevOpenmV + kFwdOpenTollMidRangemV;
        static const uint16_t kHighBandOpenRevMin3G6  = kFwdTermFwdRevOpenmV - kRevOpenTollMidRangemV;
        static const uint16_t kHighBandOpenRevMax3G6  = kFwdTermFwdRevOpenmV + kRevOpenTollMidRangemV;

        // High Frequency range
        static const uint16_t kHighBandOpenFwdMin4G95 = kFwdTermFwdRevOpenmV - (kFwdOpenTollHighRangemV + 40);
        static const uint16_t kHighBandOpenFwdMax4G95 = kFwdTermFwdRevOpenmV + kFwdOpenTollHighRangemV;
        static const uint16_t kHighBandOpenRevMin4G95 = kFwdTermFwdRevOpenmV - kRevOpenTollHighRangemV;
        static const uint16_t kHighBandOpenRevMax4G95 = kFwdTermFwdRevOpenmV + kRevOpenTollHighRangemV;

        static const int32_t  kHighBandDiffMin        = 300;         //!< Mid band, difference (forward-reverse), min for "good" point (mV)
        static const uint8_t  kHighBandGoodPctMin     = 75;          //!< Mid band, minium percentage of "good" difference points
    };
}

#endif // TESTCASEPOWERMONITORDIRECTIVITY_HPP
