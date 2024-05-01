#include "testcasectsboardifbandwidth.hpp"

namespace testsuite
{
    TestCaseCtsBoardIfBandwidth::TestCaseCtsBoardIfBandwidth(QObject* parent) :
        TestCaseCtsBoardRfBandwidth("IF Bandwidth", parent)
    {

        // Note that in some cases there are two or more consecutive bands of the same type, this caters for filter specifications,
        // for example the path 0 low-pass filter is re-generative above 3500 MHz and so a second, looser specified stop band is used
        // between 3500 MHz and 6000 MHz.
        //
        // Tests are performed outside normal operating frequencies so that we can test the out-of-band filter rejection. The LO is
        // manually tuned by the Test Utility to allow us to operated outside normal limits. Whilst operating outside normal limits care
        // must be taken not to test at spots where RF harmonics or LO harmonics will fall into the IF band. The IF bands and their sub-harmonics are:
        // 0 to 10 MHz
        // 916 to 917 MHz, 458 to 458.5 MHz
        // 1900 to 1920 MHz, 950 to 960 MHz, 475 to 480 MHz
        m_testBands = QList<CtsTestBand>() <<
        // Field Order: RF Path, IF Path, Start (kHz), End (kHz), Step (kHz), LO Offset (kHz), min (mV), max (mV), Input Power (dBm), Test Band Type
        //
        // Near-Zero IF, test at RF centre 1000 MHz, LO = 999 MHz, path 3
            CtsTestBand {3,   kIfNz,  980000,  986000,  2000,   999000, false, true, 1200, 1550,  0.0, CtsTestBandType::Stop} <<
            CtsTestBand {3,   kIfNz,  988000,  994000,  2000,   999000, false, true, 1200, 2140,  0.0, CtsTestBandType::Transition} <<
            CtsTestBand {3,   kIfNz,  996000, 1002000,  2000,   999000, false, true, 2080, 2290,  0.0, CtsTestBandType::Pass} <<
            CtsTestBand {3,   kIfNz, 1004000, 1012000,  2000,   999000, false, true, 1200, 2140,  0.0, CtsTestBandType::Transition} <<
            CtsTestBand {3,   kIfNz, 1014000, 1020000,  2000,   999000, false, true, 1200, 1550,  0.0, CtsTestBandType::Stop} <<
        // 916.5 MHz IF, test at RF centre 450 MHz, LO = 1366.5 MHz, path 1
            CtsTestBand {1,  kIf900,  420000,  446000,  2000,  1366500, false, true,  200,  550,  0.0, CtsTestBandType::Stop} <<
            CtsTestBand {1,  kIf900,  444000,  448000,  2000,  1366500, false, true,  200,  880,  0.0, CtsTestBandType::Transition} <<
            CtsTestBand {1,  kIf900,  450000,  450000,  2000,  1366500, false, true, 1330, 1650,  0.0, CtsTestBandType::Pass} <<
            CtsTestBand {1,  kIf900,  452000,  456000,  2000,  1366500, false, true,  200,  880,  0.0, CtsTestBandType::Transition} <<
            CtsTestBand {1,  kIf900,  458000,  480000,  2000,  1366500, false, true,  200,  550,  0.0, CtsTestBandType::Stop} <<
        // 1910 MHz IF, test at RF centre 5000 MHz, LO = 3090 MHz, path 5
            CtsTestBand {5, kIf1900, 4950000, 4960000,  5000,  3090000, false, true,  200, 1690,  0.0, CtsTestBandType::Stop} <<
            CtsTestBand {5, kIf1900, 4965000, 4985000,  5000,  3090000, false, true,  200, 2070,  0.0, CtsTestBandType::Transition} <<
            CtsTestBand {5, kIf1900, 4990000, 5010000,  5000,  3090000, false, true, 1790, 2120,  0.0, CtsTestBandType::Pass} <<
            CtsTestBand {5, kIf1900, 5015000, 5035000,  5000,  3090000, false, true,  200, 2070,  0.0, CtsTestBandType::Transition} <<
            CtsTestBand {5, kIf1900, 5040000, 5050000,  5000,  3090000, false, true,  200, 1690,  0.0, CtsTestBandType::Stop};
    }
}
