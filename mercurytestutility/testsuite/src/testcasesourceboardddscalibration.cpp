#include "testcasesourceboardddscalibration.hpp"
#include "testequipmentstore.hpp"
#include "mercurytestjig1.hpp"
#include "logging.hpp"
#include "settings.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "ddscalfile.hpp"
#include "rfswitchmatrix.hpp"

#include <QProgressDialog>
#include <QApplication>

namespace testsuite
{
    TestCaseSourceBoardDdsCalibration::TestCaseSourceBoardDdsCalibration(QObject* parent) :
        TestCaseSourceBoard("DDS Calibration", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseSourceBoardDdsCalibration::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDdsCalibration::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

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

            // Get a pointer to the power meter object and reset the power meter
            te::IPowerMeter* pwrMtr = te::TestEquipmentStore::powerMeter();
            TEST_STEP(te::TestEquipment::resetTe(teList()),                                         "reset test equipment");

            // Create an RF switch matrix object and select the Power Meter port
            // LHS DUT port on the test jig
            testjig::RfSwitchMatrix jigRfSwitch(m_pSrcBrd->testHarness()->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::POWER_METER), "set jig TE port to: Power Meter");
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::LHS),          "set jig DUT port to: LHS");

            // Perform the test on Port 1 alone
            TEST_STEP(m_pSrcBrd->setRfOutputPort(dut::SourceBoard::PORT_1),       "set DUT output port to: Port 1");

            // Set Source Board attenuation to 0 dB and disable blanking
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::DB_0),        "set Source Board attenutation to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                               "disable Source Board blanking on DUT");
            TEST_STEP(pHarness->setBlankIn(false),                                "disable external blanking from Test Jig");

            // Open a new CSV file
            dut::DdsCalFile calFile(m_pSrcBrd->serialNumber());
            TEST_STEP(calFile.open(common::Settings::instance()->reportsDirectory()), "open calibration file");

            // Start at 30 dB attenuation
            uint8_t att = 30u * 4u;

            int steps = static_cast<int>((kTestStopFreq - kTestStartFreq) / kTestStepFreq);
            QProgressDialog progress("Calibrating DDS...", "Cancel", 1, steps);
            progress.setWindowModality(Qt::ApplicationModal);
            progress.setValue(0);
            progress.setFixedSize(progress.sizeHint());
            bool progressCanceled = false;

            // Loop around the cal points - leave this loop if test fails, even if "stop on fail" not set as otherwise
            // the software will just keep repeating the failure for every frequency
            for (uint64_t freq = kTestStartFreq; (freq <= kTestStopFreq) && retVal && testsPassed; freq += kTestStepFreq)
            {
                progress.setValue(progress.value() + 1);
                // Configure the power meter for this frequency
                TEST_STEP(pwrMtr->setFrequency(freq), QString("set power meter to %1 MHz").arg(freq / 1e6));

                // Prime last error at 0.0 on each frequency point - prevents direction change on first step
                double lastError = 0.0;

                // Prime step size at 0.25 dB on each frequency point - prevents large steps on first test
                uint8_t stepSize = 1u;

                uint8_t lastAtt  = att;
                bool decreaseAtt = true;
                int flipCount    = 0;
                bool calPointFound = false;
                double error = 0.0;

                while (!calPointFound && testsPassed && !progressCanceled)
                {
                    progressCanceled = progress.wasCanceled();
                    // Set the DDS to output CW at this frequency at required amplitude
                    TEST_STEP(m_pSrcBrd->setDdsCw(freq, att), QString("set DDS to %1 MHz, %2 dB attenuation").
                                                                 arg(freq / 1e6).
                                                                 arg(static_cast<double>(att) / 4.0));

                    // Read power from the power meter
                    double power;
                    TEST_STEP(pwrMtr->getPower(power), "read RF power from Power Meter");

                    // Correct for specific path on test jig.
                    testjig::RfPathSourceAssy path(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                                   testjig::RfPathSourceAssy::DutRfPort::LHS,
                                                   testjig::RfPathSourceAssy::TePort::POWER_METER);

                    power = testjig::CableCal::correctedPower(path, freq, power);

                    // Power target = 0.0 dBm
                    error = 0.0 - power;
                    double absError = std::abs(error);

                    // Is the error below the minimum threshold?
                    if (absError < 0.1)
                    {
                        calPointFound = true;
                    }
                    else
                    {
                        // If attenuation hits zero and power level has not been met then cal has failed
                        if (att == 0)
                        {
                            common::Logging::instance()->log("ERROR: Reached minimum attenuation");
                            testsPassed = false;
                        }
                        else if (absError < 2.0)
                        {
                            stepSize = 1u;      // 0.25 dB step
                        }
                        else if (absError < 6.0)
                        {
                            stepSize = 4u;      // 1 dB step
                        }
                        else
                        {
                            stepSize = 12u;     // 3dB step
                        }

                        // Determine direction
                        bool directionChanged = false;
                        if (error > 0)
                        {
                            if (!decreaseAtt) { directionChanged = true; }
                            decreaseAtt = true;
                        }
                        else
                        {
                            if (decreaseAtt) { directionChanged = true; }
                            decreaseAtt = false;
                        }

                        // Was direction changed?
                        if (directionChanged)
                        {
                            // If we've been oscillating between two values then pick the
                            // one with the lowest error
                            if (++flipCount > 5)
                            {
                                if (abs(lastError) < absError)
                                {
                                    att = lastAtt;
                                }
                                calPointFound = true;
                            }
                        }
                        else
                        {
                            flipCount = 0;
                        }

                        // Increment/decrement attenuation depending on sign of error
                        if (decreaseAtt)
                        {
                            // Not enough power - reduce attenuation
                            if (att >= stepSize) { att -= stepSize; }
                            else                 { att = 0u; }
                        }
                        else
                        {
                            // Too much power - increase attenuation
                            if ((255u - att) >= stepSize) { att += stepSize; }
                            else                          { att = 255u; }
                        }

                        lastError = error;
                        lastAtt = att;
                    }  // Testpoint not found

                    // Process events to keep progress dialog responsive
                    QApplication::processEvents();

                    // Check progress dialog for cancellation
                    retVal &= !progressCanceled;
                }  // iteration loop

                // Do we have a cal point to log?
                if (calPointFound && KEEP_GOING)
                {
                    uint32_t asf = dut::SourceBoard::quarterDbToAsf(att);

                    QString msg = QString("Freq: %1 MHz, Att: %2 dB, ASF: %3")
                                                                        .arg(freq / 1e6)
                                                                        .arg(QString::number(((double)att / 4), 'f', 2))
                                                                        .arg(asf);
                    common::Logging::instance()->log(msg);

                    TEST_STEP(calFile.addPoint(freq, asf), "add point to calibration file");
                }
            }

            // Close the cal file
            calFile.close();

            // Hide the progress dialog
            progress.hide();

            // If tests passed then transfer the calibration file to the DUT
            if (testsPassed &&   !progressCanceled)
            {
                TEST_STEP(m_pSrcBrd->copyFileToSdCard(calFile, "dds_cal.csv"), "copy calibration file to SD card");
            }

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
