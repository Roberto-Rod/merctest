#include "testcasesourceboardddsfrequency.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"
#include "sourceboardrfcontrol.hpp"
#include "rfswitchmatrix.hpp"
#include "mercurytestjig1.hpp"
#include "testequipmentstore.hpp"
#include "settings.hpp"

#include <cmath>
#include <climits>

namespace testsuite
{
    TestCaseSourceBoardDdsFrequency::TestCaseSourceBoardDdsFrequency(QObject* parent) :
        TestCaseSourceBoard(QString("DDS Frequency Accuracy"), parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SpectrumAnalyser));
    }

    QString TestCaseSourceBoardDdsFrequency::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDdsFrequency::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

        // Store the port as a DutRfPort type because it will be used later for
        // cable correction
        testjig::RfPathSourceAssy::DutRfPort rfPort;

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // If in Individual test mode re-initialise synthesiser.
            if(!isTopLevel())
            {
                TEST_STEP(m_pSrcBrd->initialiseSynth(), "initialise synthesizer");
            }

            // Get a pointer to the test harness
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(m_pSrcBrd->testHarness());

            // Get a pointer to the spectrum analyser object and attempt to reset it
            te::ISpectrumAnalyser* specAn = te::TestEquipmentStore::spectrumAnalyser();
            TEST_STEP(te::TestEquipment::resetTe(teList()), "reset test equipment");

            // Centre analyser on test frequency and set span/RBW
            TEST_STEP(specAn->setFrequency(kTestFreq), QString("set Spectrum Analyser centre frequency to %1 MHz").arg (kTestFreq / 1e6));
            TEST_STEP(specAn->setSpan(kSpecAnSpan),    QString("set Spectrum Analyser span to %1 kHz").arg (kSpecAnSpan / 1e3));
            TEST_STEP(specAn->setResBw(kSpecAnRbw),    QString("set Spectrum Analyser RBW to %1 kHz").arg (kSpecAnRbw / 1e3));

            // Create an RF switch matrix object and select the Power Meter port
            // LHS DUT port on the test jig
            testjig::RfSwitchMatrix jigRfSwitch(pHarness->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::SPECTRUM_ANALYSER), "set jig TE port to: Spectrum Analyser");
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::LHS),          "set jig DUT port to: LHS");

            // Perform the test on Port 1 alone
            TEST_STEP(m_pSrcBrd->setRfOutputPort(dut::SourceBoard::PORT_1),       "set DUT output port to: Port 1");

            rfPort = testjig::RfPathSourceAssy::DutRfPort::LHS;

            // Set Source Board attenuation to 0 dB and disable blanking
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::DB_0),        "set Source Board attenutation to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                               "disable Source Board blanking on DUT");
            TEST_STEP(pHarness->setBlankIn(false),                                "disable external blanking from Test Jig");

            // Set DDS to test frequency with 10 dB attenuation (param is quarters of a dB)
            TEST_STEP(m_pSrcBrd->setDdsCw(kTestFreq, 40), QString("set DDS to %1 MHz, 10 dB attenuation").arg(kTestFreq / 1e6));

            // Store the minimum and maximum frequency measurements
            uint64_t minMeasFreq = ULLONG_MAX;
            uint64_t maxMeasFreq = 0ull;

            for (int i = 0; (i < kNumberTestReps) && KEEP_GOING; i++)
            {
                // Get the frequency measurement
                uint64_t measFreq;
                TEST_STEP(specAn->getPeakFrequency(measFreq), QString("get frequency measurement #%1 from spectrum analyser").arg(i + 1));

                // Is this the new min or max measurement?
                if (measFreq < minMeasFreq) { minMeasFreq = measFreq; }
                if (measFreq > maxMeasFreq) { maxMeasFreq = measFreq; }
            }

           double peakPower;
           TEST_STEP (specAn->getPeakPower(peakPower), QString("get frequency power from spectrum analyser"));

           // Correct for specific path on test jig. This test is always performed using the
           // short SMA-SMP cable (i.e. on the Source Board not the Source Board + Doubler)
           testjig::RfPathSourceAssy path(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                          rfPort,
                                          testjig::RfPathSourceAssy::TePort::SPECTRUM_ANALYSER);

            peakPower = testjig::CableCal::correctedPower(path, kTestFreq, peakPower);

            // Test and log result
            TEST_RESULT("Frequency Power", peakPower, KMinFreqPower, KMaxFreqPower, "dBm");

            // Calculate the frqeuency errors at the lowest and highest measured frequency
            // min/max measured could be above or below expected frequency
            int64_t loError = static_cast<int64_t>(minMeasFreq) - static_cast<int64_t>(kTestFreq);
            int64_t hiError = static_cast<int64_t>(maxMeasFreq) - static_cast<int64_t>(kTestFreq);

            // Which error (lo/hi) has the highest absolute value?
            int64_t maxError = loError;
            if (std::abs(hiError) > std::abs(loError)) { maxError = hiError; }

            // Calculate achieved ppm
            double ppm = static_cast<double>(maxError) / (static_cast<double>(kTestFreq) / 1e6);

            // Round to 1 d.p.
            ppm = std::round(ppm * 10.0) / 10.0;

            // Test and log result
            TEST_RESULT("Frequency Accuracy", ppm, -1.0 * kMaxFreqErrorPpm, 1.0 * kMaxFreqErrorPpm, "ppm");

            // Clean up - attempt these regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Set DDS to zero (DC) output
                (void)m_pSrcBrd->setDdsCw(0, 0u);

                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
