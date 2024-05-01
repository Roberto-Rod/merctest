#include "testcasectsboarddynamicrange.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"

namespace testsuite
{
    const QList<CtsDynamicRangePoint> TestCaseCtsBoardDynamicRange::kTestPoints = QList<CtsDynamicRangePoint>() <<
        // Field Order: RF Path, Frequency (kHz)
        CtsDynamicRangePoint {0, 20000, QList<CtsPowerSpec>() <<
            // Field Order: RF Input Power, ADC Reading Min. (mV), ADC Reading Max. (mV)
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   70,  390} <<
            CtsPowerSpec {-50.0,  190,  520} <<
            CtsPowerSpec {-40.0,  560,  880} <<
            CtsPowerSpec {-30.0,  950, 1280} <<
            CtsPowerSpec {-20.0, 1350, 1680} <<
            CtsPowerSpec {-10.0, 1750, 2070} <<
            CtsPowerSpec {  0.0, 2120, 2450} <<
            CtsPowerSpec { 10.0, 2200, 2530} <<
            CtsPowerSpec { 20.0, 2100, 2430}} <<
        CtsDynamicRangePoint {0, 50000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,  100,  430} <<
            CtsPowerSpec {-40.0,  370,  700} <<
            CtsPowerSpec {-30.0,  770, 1100} <<
            CtsPowerSpec {-20.0, 1170, 1490} <<
            CtsPowerSpec {-10.0, 1560, 1890} <<
            CtsPowerSpec {  0.0, 1970, 2290} <<
            CtsPowerSpec { 10.0, 2210, 2540} <<
            CtsPowerSpec { 20.0, 2150, 2480}} <<
        CtsDynamicRangePoint {0, 75000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   60,  390} <<
            CtsPowerSpec {-40.0,  130,  460} <<
            CtsPowerSpec {-30.0,  430,  760} <<
            CtsPowerSpec {-20.0,  820, 1150} <<
            CtsPowerSpec {-10.0, 1220, 1550} <<
            CtsPowerSpec {  0.0, 1620, 1950} <<
            CtsPowerSpec { 10.0, 2010, 2340} <<
            CtsPowerSpec { 20.0, 2210, 2540}} <<
        CtsDynamicRangePoint {0, 100000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   80,  410} <<
            CtsPowerSpec {-30.0,  260,  590} <<
            CtsPowerSpec {-20.0,  640,  970} <<
            CtsPowerSpec {-10.0, 1030, 1360} <<
            CtsPowerSpec {  0.0, 1420, 1750} <<
            CtsPowerSpec { 10.0, 1820, 2150} <<
            CtsPowerSpec { 20.0, 2160, 2490}} <<
        CtsDynamicRangePoint {0, 200000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   70,  400} <<
            CtsPowerSpec {-30.0,  230,  560} <<
            CtsPowerSpec {-20.0,  600,  930} <<
            CtsPowerSpec {-10.0,  990, 1320} <<
            CtsPowerSpec {  0.0, 1390, 1720} <<
            CtsPowerSpec { 10.0, 1780, 2110} <<
            CtsPowerSpec { 20.0, 2140, 2470}} <<
        CtsDynamicRangePoint {0, 400000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   70,  400} <<
            CtsPowerSpec {-30.0,  200,  530} <<
            CtsPowerSpec {-20.0,  570,  890} <<
            CtsPowerSpec {-10.0,  960, 1290} <<
            CtsPowerSpec {  0.0, 1350, 1680} <<
            CtsPowerSpec { 10.0, 1750, 2080} <<
            CtsPowerSpec { 20.0, 2130, 2460}} <<
        CtsDynamicRangePoint {1, 400001, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   70,  400} <<
            CtsPowerSpec {-30.0,  200,  530} <<
            CtsPowerSpec {-20.0,  560,  890} <<
            CtsPowerSpec {-10.0,  960, 1290} <<
            CtsPowerSpec {  0.0, 1350, 1680} <<
            CtsPowerSpec { 10.0, 1750, 2080} <<
            CtsPowerSpec { 20.0, 2130, 2460}} <<
        CtsDynamicRangePoint {1, 450000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   60,  390} <<
            CtsPowerSpec {-30.0,  180,  510} <<
            CtsPowerSpec {-20.0,  530,  860} <<
            CtsPowerSpec {-10.0,  920, 1250} <<
            CtsPowerSpec {  0.0, 1330, 1660} <<
            CtsPowerSpec { 10.0, 1730, 2050} <<
            CtsPowerSpec { 20.0, 2110, 2440}} <<
        CtsDynamicRangePoint {1, 500000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   60,  390} <<
            CtsPowerSpec {-30.0,  150,  480} <<
            CtsPowerSpec {-20.0,  480,  810} <<
            CtsPowerSpec {-10.0,  880, 1200} <<
            CtsPowerSpec {  0.0, 1270, 1600} <<
            CtsPowerSpec { 10.0, 1670, 2000} <<
            CtsPowerSpec { 20.0, 2070, 2400}} <<
        CtsDynamicRangePoint {2, 500001, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   60,  390} <<
            CtsPowerSpec {-30.0,  180,  510} <<
            CtsPowerSpec {-20.0,  540,  870} <<
            CtsPowerSpec {-10.0,  930, 1260} <<
            CtsPowerSpec {  0.0, 1330, 1660} <<
            CtsPowerSpec { 10.0, 1730, 2060} <<
            CtsPowerSpec { 20.0, 2120, 2450}} <<
        CtsDynamicRangePoint {2, 650000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   70,  400} <<
            CtsPowerSpec {-30.0,  200,  520} <<
            CtsPowerSpec {-20.0,  550,  880} <<
            CtsPowerSpec {-10.0,  940, 1270} <<
            CtsPowerSpec {  0.0, 1340, 1670} <<
            CtsPowerSpec { 10.0, 1740, 2070} <<
            CtsPowerSpec { 20.0, 2130, 2460}} <<
        CtsDynamicRangePoint {2, 800000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   50,  380} <<
            CtsPowerSpec {-60.0,   50,  380} <<
            CtsPowerSpec {-50.0,   50,  380} <<
            CtsPowerSpec {-40.0,   70,  400} <<
            CtsPowerSpec {-30.0,  210,  540} <<
            CtsPowerSpec {-20.0,  580,  900} <<
            CtsPowerSpec {-10.0,  970, 1300} <<
            CtsPowerSpec {  0.0, 1360, 1690} <<
            CtsPowerSpec { 10.0, 1760, 2090} <<
            CtsPowerSpec { 20.0, 2140, 2470}} <<
        CtsDynamicRangePoint {3, 800001, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1430} <<
            CtsPowerSpec {-60.0, 1230, 1440} <<
            CtsPowerSpec {-50.0, 1230, 1440} <<
            CtsPowerSpec {-40.0, 1240, 1450} <<
            CtsPowerSpec {-30.0, 1320, 1530} <<
            CtsPowerSpec {-20.0, 1490, 1700} <<
            CtsPowerSpec {-10.0, 1710, 1920} <<
            CtsPowerSpec {  0.0, 1950, 2160} <<
            CtsPowerSpec { 10.0, 2220, 2430} <<
            CtsPowerSpec { 20.0, 2460, 2670}} <<
        CtsDynamicRangePoint {3, 1100000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1440} <<
            CtsPowerSpec {-60.0, 1230, 1440} <<
            CtsPowerSpec {-50.0, 1230, 1440} <<
            CtsPowerSpec {-40.0, 1270, 1480} <<
            CtsPowerSpec {-30.0, 1390, 1600} <<
            CtsPowerSpec {-20.0, 1590, 1790} <<
            CtsPowerSpec {-10.0, 1840, 2050} <<
            CtsPowerSpec {  0.0, 2080, 2290} <<
            CtsPowerSpec { 10.0, 2320, 2530} <<
            CtsPowerSpec { 20.0, 2560, 2770}} <<
        CtsDynamicRangePoint {3, 1400000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1430} <<
            CtsPowerSpec {-60.0, 1230, 1430} <<
            CtsPowerSpec {-50.0, 1230, 1440} <<
            CtsPowerSpec {-40.0, 1270, 1480} <<
            CtsPowerSpec {-30.0, 1390, 1600} <<
            CtsPowerSpec {-20.0, 1580, 1790} <<
            CtsPowerSpec {-10.0, 1840, 2050} <<
            CtsPowerSpec {  0.0, 2080, 2290} <<
            CtsPowerSpec { 10.0, 2330, 2540} <<
            CtsPowerSpec { 20.0, 2560, 2770}} <<
        CtsDynamicRangePoint {3, 1700000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1440} <<
            CtsPowerSpec {-60.0, 1230, 1430} <<
            CtsPowerSpec {-50.0, 1230, 1440} <<
            CtsPowerSpec {-40.0, 1270, 1480} <<
            CtsPowerSpec {-30.0, 1400, 1610} <<
            CtsPowerSpec {-20.0, 1590, 1800} <<
            CtsPowerSpec {-10.0, 1850, 2060} <<
            CtsPowerSpec {  0.0, 2100, 2310} <<
            CtsPowerSpec { 10.0, 2340, 2550} <<
            CtsPowerSpec { 20.0, 2570, 2780}} <<
        CtsDynamicRangePoint {3, 2000000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1430} <<
            CtsPowerSpec {-60.0, 1230, 1440} <<
            CtsPowerSpec {-50.0, 1230, 1440} <<
            CtsPowerSpec {-40.0, 1270, 1480} <<
            CtsPowerSpec {-30.0, 1390, 1600} <<
            CtsPowerSpec {-20.0, 1590, 1800} <<
            CtsPowerSpec {-10.0, 1850, 2050} <<
            CtsPowerSpec {  0.0, 2090, 2300} <<
            CtsPowerSpec { 10.0, 2330, 2540} <<
            CtsPowerSpec { 20.0, 2560, 2770}} <<
        CtsDynamicRangePoint {4, 2000001, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1440} <<
            CtsPowerSpec {-60.0, 1230, 1440} <<
            CtsPowerSpec {-50.0, 1240, 1450} <<
            CtsPowerSpec {-40.0, 1320, 1530} <<
            CtsPowerSpec {-30.0, 1490, 1700} <<
            CtsPowerSpec {-20.0, 1710, 1920} <<
            CtsPowerSpec {-10.0, 1960, 2170} <<
            CtsPowerSpec {  0.0, 2230, 2440} <<
            CtsPowerSpec { 10.0, 2480, 2680} <<
            CtsPowerSpec { 20.0, 2750, 2960}} <<
        CtsDynamicRangePoint {4, 2550000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1440} <<
            CtsPowerSpec {-60.0, 1230, 1440} <<
            CtsPowerSpec {-50.0, 1260, 1470} <<
            CtsPowerSpec {-40.0, 1370, 1580} <<
            CtsPowerSpec {-30.0, 1560, 1770} <<
            CtsPowerSpec {-20.0, 1810, 2020} <<
            CtsPowerSpec {-10.0, 2050, 2260} <<
            CtsPowerSpec {  0.0, 2300, 2510} <<
            CtsPowerSpec { 10.0, 2540, 2750} <<
            CtsPowerSpec { 20.0, 2750, 2960}} <<
        CtsDynamicRangePoint {4, 3100000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1230, 1440} <<
            CtsPowerSpec {-60.0, 1230, 1440} <<
            CtsPowerSpec {-50.0, 1260, 1470} <<
            CtsPowerSpec {-40.0, 1380, 1590} <<
            CtsPowerSpec {-30.0, 1570, 1780} <<
            CtsPowerSpec {-20.0, 1820, 2030} <<
            CtsPowerSpec {-10.0, 2060, 2270} <<
            CtsPowerSpec {  0.0, 2310, 2520} <<
            CtsPowerSpec { 10.0, 2540, 2750} <<
            CtsPowerSpec { 20.0, 2760, 2970}} <<
        CtsDynamicRangePoint {4, 3650000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1240, 1450} <<
            CtsPowerSpec {-60.0, 1240, 1440} <<
            CtsPowerSpec {-50.0, 1250, 1460} <<
            CtsPowerSpec {-40.0, 1340, 1550} <<
            CtsPowerSpec {-30.0, 1510, 1720} <<
            CtsPowerSpec {-20.0, 1750, 1960} <<
            CtsPowerSpec {-10.0, 1990, 2200} <<
            CtsPowerSpec {  0.0, 2250, 2450} <<
            CtsPowerSpec { 10.0, 2460, 2670} <<
            CtsPowerSpec { 20.0, 2610, 2820}} <<
        CtsDynamicRangePoint {4, 4200000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0, 1240, 1450} <<
            CtsPowerSpec {-60.0, 1240, 1450} <<
            CtsPowerSpec {-50.0, 1250, 1460} <<
            CtsPowerSpec {-40.0, 1300, 1510} <<
            CtsPowerSpec {-30.0, 1450, 1660} <<
            CtsPowerSpec {-20.0, 1660, 1870} <<
            CtsPowerSpec {-10.0, 1920, 2130} <<
            CtsPowerSpec {  0.0, 2180, 2390} <<
            CtsPowerSpec { 10.0, 2400, 2610} <<
            CtsPowerSpec { 20.0, 2610, 2820}} <<
        CtsDynamicRangePoint {5, 4200001, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,  120,  440} <<
            CtsPowerSpec {-60.0,  120,  450} <<
            CtsPowerSpec {-50.0,  170,  500} <<
            CtsPowerSpec {-40.0,  420,  750} <<
            CtsPowerSpec {-30.0,  790, 1120} <<
            CtsPowerSpec {-20.0, 1150, 1480} <<
            CtsPowerSpec {-10.0, 1520, 1840} <<
            CtsPowerSpec {  0.0, 1830, 2160} <<
            CtsPowerSpec { 10.0, 2010, 2340} <<
            CtsPowerSpec { 20.0, 2030, 2360}} <<
        CtsDynamicRangePoint {5, 4650000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   60,  390} <<
            CtsPowerSpec {-60.0,   60,  390} <<
            CtsPowerSpec {-50.0,  120,  450} <<
            CtsPowerSpec {-40.0,  390,  720} <<
            CtsPowerSpec {-30.0,  770, 1090} <<
            CtsPowerSpec {-20.0, 1120, 1450} <<
            CtsPowerSpec {-10.0, 1490, 1820} <<
            CtsPowerSpec {  0.0, 1810, 2140} <<
            CtsPowerSpec { 10.0, 2010, 2330} <<
            CtsPowerSpec { 20.0, 2030, 2360}} <<
        CtsDynamicRangePoint {5, 5100000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   60,  380} <<
            CtsPowerSpec {-60.0,   60,  390} <<
            CtsPowerSpec {-50.0,  100,  430} <<
            CtsPowerSpec {-40.0,  320,  650} <<
            CtsPowerSpec {-30.0,  710, 1040} <<
            CtsPowerSpec {-20.0, 1060, 1390} <<
            CtsPowerSpec {-10.0, 1410, 1740} <<
            CtsPowerSpec {  0.0, 1750, 2080} <<
            CtsPowerSpec { 10.0, 1990, 2320} <<
            CtsPowerSpec { 20.0, 2030, 2360}} <<
        CtsDynamicRangePoint {5, 5550000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   60,  390} <<
            CtsPowerSpec {-60.0,   60,  390} <<
            CtsPowerSpec {-50.0,   90,  420} <<
            CtsPowerSpec {-40.0,  280,  610} <<
            CtsPowerSpec {-30.0,  660,  990} <<
            CtsPowerSpec {-20.0, 1020, 1350} <<
            CtsPowerSpec {-10.0, 1380, 1710} <<
            CtsPowerSpec {  0.0, 1730, 2060} <<
            CtsPowerSpec { 10.0, 2000, 2330} <<
            CtsPowerSpec { 20.0, 2030, 2360}} <<
        CtsDynamicRangePoint {5, 6000000, QList<CtsPowerSpec>() <<
            CtsPowerSpec {-70.0,   60,  380} <<
            CtsPowerSpec {-60.0,   60,  390} <<
            CtsPowerSpec {-50.0,   90,  420} <<
            CtsPowerSpec {-40.0,  290,  620} <<
            CtsPowerSpec {-30.0,  660,  990} <<
            CtsPowerSpec {-20.0, 1020, 1350} <<
            CtsPowerSpec {-10.0, 1370, 1700} <<
            CtsPowerSpec {  0.0, 1710, 2040} <<
            CtsPowerSpec { 10.0, 1990, 2320} <<
            CtsPowerSpec { 20.0, 2030, 2360}};

    TestCaseCtsBoardDynamicRange::TestCaseCtsBoardDynamicRange(QObject* parent) :
        TestCaseCtsBoard("Dynamic Range", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
    }

    QString TestCaseCtsBoardDynamicRange::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardDynamicRange::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed(true);

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal(TestCase::execute());

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "initialise DUT");

            // Get a pointer to the signal generator object and attempt to reset it
            te::ISignalGenerator* pSigGen(te::TestEquipmentStore::signalGenerator());
            TEST_STEP(te::TestEquipment::resetTe(teList()), "reset test equipment");

            // Enable DUT RF section
            TEST_STEP(m_pCtsBrd->enableRfPower(true), "enable DUT RF section");
            QThread::msleep(1000);

            // Loop over test points
            CtsDynamicRangePoint point;
            foreach (point, kTestPoints)
            {
                CtsPowerSpec powerSpec;
                uint64_t rfFreq_Hz(uint64_t(point.m_frequency_kHz) * 1000);

                // Set the DUT to this path
                TEST_STEP(m_pCtsBrd->setRfPath(static_cast<dut::CtsBoard::RfPath>(point.m_rfPath)), "set DUT RF path");

                // Set DUT to this frequency
                TEST_STEP(m_pCtsBrd->setRfFrequency(static_cast<uint32_t>(point.m_frequency_kHz)), "set DUT RF frequency");

                // Set signal generator to this frequency
                TEST_STEP(pSigGen->setFrequency(rfFreq_Hz), "set signal generator frequency");

                // Set signal generator to -120 dBm
                TEST_STEP(pSigGen->setOutputPower(-120.00), "set signal generator to -120 dBm")

                // Enable signal generator RF output
                TEST_STEP(pSigGen->setRfEnabled(true), "enable signal generator RF output");

                // Loop over power levels
                foreach (powerSpec, point.m_powerSpec)
                {
                    uint16_t power;

                    // Get cable calibration correction. The cable calibration includes two cables, the one normally
                    // connected from signal generator to the Power Monitor assembly and the one normally connected
                    // from the Power Monitor assembly to the power meter. Assume that the loss is equally distributed
                    // between the two cables and use half of that loss for this case where only one cable is used.
                    testjig::RfPathPowerMonitor path(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);
                    double offset_dB(testjig::CableCal::correctedPower(path, rfFreq_Hz, 0));

                    // Set signal generator to this power
                    TEST_STEP(pSigGen->setOutputPower(double(powerSpec.m_inputPower) + offset_dB), "set signal generator output power");

                    // Read power from the DUT
                    TEST_STEP(m_pCtsBrd->getRfPowerReading(power), "read power from DUT");

                    // Test against limits
                    QString desc(QString("Path %1, %2 MHz, %3 dBm").arg(point.m_rfPath).arg(QString::number(double(rfFreq_Hz)/1e6, 'f', 3)).arg(QString::number(powerSpec.m_inputPower, 'f', 1)));
                    TEST_RESULT(desc, power, powerSpec.m_adcMin_mV, powerSpec.m_adcMax_mV, "mV");
                }

                // Disable signal generator RF output
                TEST_STEP(pSigGen->setRfEnabled(false), "disable signal generator RF output");
            }

            // Disable DUT RF section
            TEST_STEP(m_pCtsBrd->enableRfPower(false), "disable DUT RF section");

            // Clean up - attempt regardless of test state
            if (m_pCtsBrd)
            {
                // Disconnect DUT serial port
                m_pCtsBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
