#include "testcasectsboardrfbandwidth.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"

namespace testsuite
{
    TestCaseCtsBoardRfBandwidth::TestCaseCtsBoardRfBandwidth(QString name, QObject* parent) :
        TestCaseCtsBoard(name, parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
    }

    QString TestCaseCtsBoardRfBandwidth::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardRfBandwidth::execute(bool manualSteps)
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

            // Enable signal generator RF output
            TEST_STEP(pSigGen->setRfEnabled(true), "enable signal generator RF output");

            // Enable DUT RF section
            TEST_STEP(m_pCtsBrd->enableRfPower(true), "enable DUT RF section");
            QThread::msleep(1000);

            // Loop over test bands
            CtsTestBand band;
            foreach (band, m_testBands)
            {
                // Set the DUT to this path
                TEST_STEP(m_pCtsBrd->setRfPath(static_cast<dut::CtsBoard::RfPath>(band.m_rfPath)), "set DUT RF path");
                if (band.m_ifPath != kIfNz)
                {
                    TEST_STEP(m_pCtsBrd->setIfPath(static_cast<dut::CtsBoard::IfPath>(band.m_ifPath)), "set DUT IF path");
                }

                // Loop over frequencies in this test band
                for (uint32_t rfFreq_kHz(band.m_rfStart_kHz); rfFreq_kHz <= band.m_rfEnd_kHz; rfFreq_kHz += band.m_rfStep_kHz)
                {
                    uint16_t power;

                    // Get cable calibration correction. The cable calibration includes two cables, the one normally
                    // connected from signal generator to the Power Monitor assembly and the one normally connected
                    // from the Power Monitor assembly to the power meter. Assume that the loss is equally distributed
                    // between the two cables and use half of that loss for this case where only one cable is used.
                    uint64_t rfFreq_Hz(uint64_t(rfFreq_kHz) * 1000);
                    testjig::RfPathPowerMonitor path(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);
                    double offset_dB(testjig::CableCal::correctedPower(path, rfFreq_Hz, 0));

                    // Set signal generator to this frequency
                    TEST_STEP(pSigGen->setFrequency(rfFreq_Hz), "set signal generator frequency");
                    TEST_STEP(pSigGen->setOutputPower(double(band.m_testPower_dBm) + offset_dB), "set signal generator output power");

                    // Set the DUT to this frequency
                    int32_t loFreq_kHz(band.m_loOffset_kHz);
                    if (!band.m_fixedLo)
                    {
                        if (band.m_invertRf)
                        {
                            loFreq_kHz -= rfFreq_kHz;
                        }
                        else
                        {
                            loFreq_kHz += rfFreq_kHz;
                        }
                    }
                    TEST_STEP(m_pCtsBrd->setLoFrequency(static_cast<uint32_t>(loFreq_kHz)), "set DUT LO frequency");

                    // Read power from the DUT
                    TEST_STEP(m_pCtsBrd->getRfPowerReading(power), "read power from DUT");

                    // Test against limits
                    QString desc(QString("Path %1, %2 MHz [LO %3 MHz] (%4 Band)").arg(band.m_rfPath).arg(QString::number(double(rfFreq_kHz)/1000.0, 'f', 1)).arg(QString::number(double(loFreq_kHz)/1000.0, 'f', 1)).arg(band.typeString()));
                    TEST_RESULT(desc, power, band.m_minPowerReading_mV, band.m_maxPowerReading_mV, "mV");
                }
            }

            // Disable signal generator RF output
            TEST_STEP(pSigGen->setRfEnabled(false), "disable signal generator RF output");

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
