#ifndef TESTCASEMIDBANDSOURCEMODULERFPATH_HPP
#define TESTCASEMIDBANDSOURCEMODULERFPATH_HPP

#include "testcasesourceboard.hpp"
#include "rfpath.hpp"

namespace testsuite
{
// Original MB test point.
    struct SourceModuleTestPointMB
    {
        uint8_t path;           //!< Source module path selection
        uint64_t freq;          //!< Test frequency, Hz
        double powerIn;         //!< Power in, dBm
        double powerOutMin;     //!< Minimum output power, dBm
        double F0;
        double F2;
        double F3;
        double F4;
    };

    // This is really the HB test point
    /*struct SourceModuleTestPointMB
    {
        uint8_t path;           //!< Source module path selection
        uint64_t freq;          //!< Test frequency, Hz
        double powerIn;         //!< Power in, dBm
        double powerOutMin;     //!< Minimum output power, dBm
        uint64_t  startFreq;          //!< Analyser start frequency
        uint64_t  stopFreq;          //!<Analyser stop frequency
        uint64_t  rbw;                      //!< Analyser resolution band width
    }; */

    struct SourceModuleTestPointHB
    {
        uint8_t path;           //!< Source module path selection
        uint64_t freq;          //!< Test frequency, Hz
        double powerIn;         //!< Power in, dBm
        double powerOutMin;     //!< Minimum output power, dBm
        double  npeak1Rejection;          //!< Minimum Rejection, dB for Next Peak
        double  npeak2Rejection;         //!< Minimum Rejection, dB for Next, Next Peak
        double  npeak3Rejection;          //!< Minimum Rejection, dB for Next, Next, Next Peak
    };

    class TestCaseMidBandSourceModuleRfPath : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:

       struct bandTestParams
        {
            bool testsPassed;
            bool manualSteps;
            double freqWanted;
            double powerWanted;
            double sigGenFreq;
            uint16_t  pathdivisor;
            testjig::RfPath  * specAnPath;
        };

        TestCaseMidBandSourceModuleRfPath(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const SourceModuleTestPointMB kTestPointMidBandRevDiss2[];
        static const SourceModuleTestPointMB kTestPointMidBandRevCIss2[];
        static const SourceModuleTestPointMB kTestPointMidBandRevBtoC[];
        static const SourceModuleTestPointHB kTestPointHighBand[];

        static constexpr double kF0RejectionMin   = 40.0;    //!< Minimum fundamental rejection, dB. Only applies to doubled paths
        static constexpr double k2F0RejectionMin  = 29.0;    //!< Minimum 2x fundamental rejection, dB. Only applies to bypass path
        static constexpr double k3F0RejectionMin  = 17.0;    //!< Minimum 3x fundamental rejection, dB. Only applies to doubled paths
        static constexpr double k4F0RejectionMin  = 18.3;    //!< Minimum 4x fundamental rejection, dB. Only applies to doubled paths

        static const uint64_t kAnalyserSpan       = 10000;   //!< Spectrum analyser span
        static const uint64_t kAnalyserRbw        = 10;      //!< Spectrum analyser resolution bandwidth
        static const uint64_t kAnalyserRbwHB      = 3000000; //!< Spectrum analyser resolution bandwidth
        static constexpr double kAnalyserRefLev   = 20.0;    //!< Spectrum analyser reference level
        static constexpr double kAnalyserRefLevHB = 0.0;     //!< Spectrum analyser reference level

        const SourceModuleTestPointMB * getTestPointMidBand(int revision, int index);

        bool testRejection(const QString& testName, uint64_t freq, double powerWanted, double rejectionMin);
        bool testHighBandPathHarmonics(bandTestParams prm, SourceModuleTestPointHB  hbtp, bool& retVal);
        bool testMidBandPathHarmonicsX(bandTestParams prm,  SourceModuleTestPointMB mbtp,  bool& retVal);
        bool testMidBandPathHarmonics(bandTestParams prm, SourceModuleTestPointMB  mbtp, bool& retVal);
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULERFPATH_HPP
