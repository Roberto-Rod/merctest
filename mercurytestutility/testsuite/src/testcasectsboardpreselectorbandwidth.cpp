#include "testcasectsboardpreselectorbandwidth.hpp"

namespace testsuite
{
    TestCaseCtsBoardPreselectorBandwidth::TestCaseCtsBoardPreselectorBandwidth(QObject* parent) :
        TestCaseCtsBoardRfBandwidth("Preselector Bandwidth", parent)
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
        // Path 0/1: 20 MHz to 500 MHz
            // Pass Band: 20 MHz to 60 MHz, step 20 MHz, LO = 916.5 MHz - RF
            CtsTestBand {1,  kIf900,   20000,   60000, 20000,   916500,  true, false, 1840, 2450,  0.0, CtsTestBandType::Pass} <<
            // Pass Band: 80 MHz to 120 MHz, step 20 MHz, LO = 916.5 MHz - RF
            CtsTestBand {1,  kIf900,   80000,  120000, 20000,   916500,  true, false, 1410, 1890,  0.0, CtsTestBandType::Pass} <<
            // Pass Band: 150 MHz to 400 MHz, step 50 MHz, LO = 916.5 MHz - RF
            CtsTestBand {1,  kIf900,  140000,  400000, 50000,   916500,  true, false, 1360, 1730,  0.0, CtsTestBandType::Pass} <<
            // Transition Band: 450 MHz to 650 MHz, step 50 MHz, LO = RF + 916.5 MHz
            CtsTestBand {1,  kIf900,  450000,  650000, 50000,   916500, false, false,  200, 1730,  0.0, CtsTestBandType::Transition} <<
            // Stop Band: 700 MHz to 3200 MHz, step 50 MHz, LO = RF + 916.5 MHz
            CtsTestBand {1,  kIf900,  700000, 3200000, 50000,   916500, false, false,  200,  470,  0.0, CtsTestBandType::Stop} <<
            // Stop Band: 3250 MHz to 3750 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {1, kIf1900, 3250000, 3750000, 50000, -1910000, false, false,  200,  620,  0.0, CtsTestBandType::Stop} <<
            // Skip RF band 3800 to 3900 MHz as LO ends up within/too close to 1910 MHz IF
            // Stop Band: 3950 MHz to 6000 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {1, kIf1900, 3950000, 6000000, 50000, -1910000, false, false,  200,  620,  0.0, CtsTestBandType::Stop} <<

        // Path 2: 500 MHz to 800 MHz
            // Stop Band: 20 MHz to 100 MHz, step 20 MHz, LO = 916.5 MHz - RF
            CtsTestBand {2,  kIf900,   20000,  100000, 20000,   916500,  true, false,  200,  470,  0.0, CtsTestBandType::Stop} <<
            // Transition Band: 150 MHz to 450 MHz, step 50 MHz, LO = 916.5 MHz - RF
            CtsTestBand {2,  kIf900,  150000,  450000, 50000,   916500,  true, false,  200, 1650,  0.0, CtsTestBandType::Transition} <<
            // Pass Band: 500 MHz to 800 MHz, step 50 MHz, LO = 916.5 MHz - RF
            CtsTestBand {2,  kIf900,  500000,  800000, 50000,   916500,  true, false, 1330, 1700,  0.0, CtsTestBandType::Pass} <<
            // Transition Band: 820 MHz to 1700 MHz, step 50 MHz, LO = RF + 916.5 MHz
            CtsTestBand {2,  kIf900,  820000, 1700000, 50000,   916500, false, false,  200, 1700,  0.0, CtsTestBandType::Transition} <<
            // Stop Band: 1750 MHz to 3200 MHz, step 50 MHz, LO = RF + 916.5 MHz
            CtsTestBand {2,  kIf900, 1750000, 3200000, 50000,   916500, false, false,  200,  430,  0.0, CtsTestBandType::Stop} <<
            // Stop Band: 3250 MHz to 3750 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {2, kIf1900, 3250000, 3750000, 50000, -1910000, false, false,  200,  670,  0.0, CtsTestBandType::Stop} <<
            // Skip RF band 3800 to 3900 MHz as LO ends up within/too close to 1910 MHz IF
            // Stop Band: 3950 MHz to 6000 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {2, kIf1900, 3950000, 6000000, 50000, -1910000, false, false,  200,  670,  0.0, CtsTestBandType::Stop} <<

        // Path 3: 800 MHz to 2000 MHz, LO = RF - 1 MHz
            // Note: can only test in range 86 MHz to 4201 MHz due to LO frequency limitation.
            // Stop Band: 100 MHz to 500 MHz, step 50 MHz
            CtsTestBand {3,  kIfNz,  100000,  500000, 50000,     -1000, false, false, 1200, 1530,  0.0, CtsTestBandType::Stop} <<
            // Transition Band: 550 MHz to 750 MHz, step 50 MHz
            CtsTestBand {3,  kIfNz,  550000,  750000, 50000,     -1000, false, false, 1200, 2300,  0.0, CtsTestBandType::Transition} <<
            // Pass Band: 800 MHz to 2000 MHz, step 50 MHz
            CtsTestBand {3,  kIfNz,  800000, 2000000, 50000,     -1000, false, false, 1950, 2300,  0.0, CtsTestBandType::Pass} <<
            // Transition Band: 2050 MHz to 2950 MHz, step 50 MHz
            CtsTestBand {3,  kIfNz, 2050000, 2950000, 50000,     -1000, false, false, 1200, 2300,  0.0, CtsTestBandType::Transition} <<
            // Stop Band: 3000 MHz to 4200 MHz, step 20 MHz
            CtsTestBand {3,  kIfNz, 3000000, 4200000, 50000,     -1000, false, false, 1200, 1530,  0.0, CtsTestBandType::Stop} <<

        // Path 4: 2000 MHz to 4200 MHz, LO = RF - 1 MHz
            // Note: can only test in range 86 MHz to 4201 MHz due to LO frequency limitation.
            // Stop Band: 100 MHz to 1300 MHz, step 50 MHz
            CtsTestBand {4,  kIfNz,  100000, 1300000, 50000,     -1000, false, false, 1200, 1530,  0.0, CtsTestBandType::Stop} <<
            // Transition Band: 1350 MHz to 1950 MHz, step 50 MHz
            CtsTestBand {4,  kIfNz, 1350000, 1950000, 50000,     -1000, false, false, 1200, 2520,  0.0, CtsTestBandType::Transition} <<
            // Pass Band: 2000 MHz to 4200 MHz, step 50 MHz
            CtsTestBand {4,  kIfNz, 2000000, 4200000, 50000,     -1000, false, false, 2180, 2520,  0.0, CtsTestBandType::Pass} <<

        // Path 5: 4200 MHz to 6000 MHz
            // Stop Band: 50 MHz to 500 MHz, step 50 MHz, LO = 916.5 MHz - RF
            CtsTestBand {5,  kIf900,   50000,  500000, 50000,   916500,  true, false,  200, 1100,  0.0, CtsTestBandType::Stop} <<
            // Stop Band: 550 MHz to 800 MHz, step 50 MHz, LO = 916.5 MHz - RF
            CtsTestBand {5,  kIf900,  550000,  800000, 50000,   916500,  true, false,  200, 1260,  0.0, CtsTestBandType::Stop} <<
            // Stop Band: 850 MHz to 3200 MHz, step 50 MHz, LO = RF + 916.5 MHz
            CtsTestBand {5,  kIf900,  850000, 3200000, 50000,   916500, false, false,  200, 1350,  0.0, CtsTestBandType::Stop} <<
            // Stop Band: 3250 MHz to 3400 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {5, kIf1900, 3250000, 3400000, 50000, -1910000, false, false,  200, 1350,  0.0, CtsTestBandType::Stop} <<
            // TransitionBand: 3450 MHz to 3750 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {5, kIf1900, 3450000, 3750000, 50000, -1910000, false, false,  200, 2060,  0.0, CtsTestBandType::Transition} <<
            // Skip RF band 3800 to 3900 MHz as LO ends up within/too close to 1910 MHz IF
            // Transition Band: 3950 MHz to 4150 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {5, kIf1900, 3950000, 4150000, 50000, -1910000, false, false,  200, 2150,  0.0, CtsTestBandType::Transition} <<
            // Pass Band: 4200 MHz to 6000 MHz, step 50 MHz, LO = RF - 1910 MHz
            CtsTestBand {5, kIf1900, 4200000, 6000000, 50000, -1910000, false, false, 1720, 2165,  0.0, CtsTestBandType::Pass};
    }
}
