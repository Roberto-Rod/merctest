#include "testcasepowermonitordirectivity.hpp"
#include "powermonitor.hpp"
#include "testharnesspowermonitor.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"
#include "rfpath.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCasePowerMonitorDirectivity::TestCasePowerMonitorDirectivity(QObject* parent) :
        TestCasePowerMonitor("Directivity/Sensitivity", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
    }

    QString TestCasePowerMonitorDirectivity::setupMessage()
    {
        return QString();
    }

    bool TestCasePowerMonitorDirectivity::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();
       // const  double KCouplerSlopeAdjustment = 0.125;

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Get pointers to the DUT and the Test Harness
            dut::PowerMonitor* pwrMon = static_cast<dut::PowerMonitor*>(dut());
            testjig::TestHarnessPowerMonitor* harness = static_cast<testjig::TestHarnessPowerMonitor*>(pwrMon->testHarness());

            // Enable jig power & DUT
            TEST_STEP(harness->enablePower(true),            "enable 5.5V rail on test jig");
            TEST_STEP(harness->driveEnablePin(true),         "assert enable pin to DUT");

            TEST_STEP(te::TestEquipment::resetTe(teList()),  "reset test equipment");

            // Get a pointer to the power monitor object
            dut::PowerMonitor* powerMonitor = static_cast<dut::PowerMonitor*>(dut());

            // Get the start/stop/step frequencies and the test limit for the given band
            uint64_t startFreq = 0ull;
            uint64_t stopFreq  = 0ull;
            uint64_t stepFreq  = 0ull;
            uint16_t testLimitTermFwdMin = 0;
            uint16_t testLimitTermFwdMax = 0;
            uint16_t testLimitTermRevMin = 0;
            uint16_t testLimitTermRevMax = 0;
            uint16_t testLimitOpenFwdMin = 0;
            uint16_t testLimitOpenFwdMax = 0;
            uint16_t testLimitOpenRevMin = 0;
            uint16_t testLimitOpenRevMax = 0;
            int32_t  testLimitDiffMin = 0;
            uint8_t  testLimitGoodPctMin = 0;
            double sigGenPowerInit = kTestPowerLevel;

            if (powerMonitor->band() == hardware::RfBand::LOW)
            {
                startFreq = kLowBandStartFreq;
                stopFreq  = kLowBandStopFreq;
                stepFreq  = kLowBandStepFreq;

                if (pwrMon->isPreRevBIssue3())
                {
                    testLimitTermFwdMin = kLowBandTermFwdMin5V0;
                    testLimitTermFwdMax = kLowBandTermFwdMax5V0;
                    testLimitTermRevMin = kLowBandTermRevMin5V0;
                    testLimitTermRevMax = kLowBandTermRevMax5V0;
                    testLimitOpenFwdMin = kLowBandOpenFwdMin5V0;
                    testLimitOpenFwdMax = kLowBandOpenFwdMax5V0;
                    testLimitOpenRevMin = kLowBandOpenRevMin5V0;
                    testLimitOpenRevMax = kLowBandOpenRevMax5V0;
                    testLimitDiffMin    = kLowBandDiffMin5V0;
                    testLimitGoodPctMin = kLowBandGoodPctMin5V0;
                }
                else     // PCB0055 Rev C or later or High Band PCB0080 Rev B or later
                {
                    testLimitTermFwdMin = kLowBandTermFwdMin3V3;
                    testLimitTermFwdMax = kLowBandTermFwdMax3V3;
                    testLimitTermRevMin = kLowBandTermRevMin3V3;
                    testLimitTermRevMax = kLowBandTermRevMax3V3;
                    testLimitOpenFwdMin = kLowBandOpenFwdMin3V3;
                    testLimitOpenFwdMax = kLowBandOpenFwdMax3V3;
                    testLimitOpenRevMin = kLowBandOpenRevMin3V3;
                    testLimitOpenRevMax = kLowBandOpenRevMax3V3;
                    testLimitDiffMin    = kLowBandDiffMin3V3;
                    testLimitGoodPctMin = kLowBandGoodPctMin3V3;
                }
            }
            else if (powerMonitor->band() == hardware::RfBand::MID)
            {
                startFreq = kMidBandStartFreq;
                stopFreq  = kMidBandStopFreq;
                stepFreq  = kMidBandStepFreq;

                if (pwrMon->isPreRevBIssue3())
                {
                    testLimitTermFwdMin = kMidBandTermFwdMin5V0;
                    testLimitTermFwdMax = kMidBandTermFwdMax5V0;
                    testLimitTermRevMin = kMidBandTermRevMin5V0;
                    testLimitTermRevMax = kMidBandTermRevMax5V0;
                    testLimitOpenFwdMin = kMidBandOpenFwdMin5V0;
                    testLimitOpenFwdMax = kMidBandOpenFwdMax5V0;
                    testLimitOpenRevMin = kMidBandOpenRevMin5V0;
                    testLimitOpenRevMax = kMidBandOpenRevMax5V0;
                    testLimitDiffMin    = kMidBandDiffMin5V0;
                    testLimitGoodPctMin = kMidBandGoodPctMin5V0;
                }
                else  // PCB0055 Rev C or later or High Band PCB0080 Rev B or later
                {
                    testLimitTermFwdMin = kMidBandTermFwdMin3V3;
                    testLimitTermFwdMax = kMidBandTermFwdMax3V3;
                    testLimitTermRevMin = kMidBandTermRevMin3V3;
                    testLimitTermRevMax = kMidBandTermRevMax3V3;
                    testLimitOpenFwdMin = kMidBandOpenFwdMin3V3;
                    testLimitOpenFwdMax = kMidBandOpenFwdMax3V3;
                    testLimitOpenRevMin = kMidBandOpenRevMin3V3;
                    testLimitOpenRevMax = kMidBandOpenRevMax3V3;
                    testLimitDiffMin    = kMidBandDiffMin3V3;
                    testLimitGoodPctMin = kMidBandGoodPctMin3V3;
                }
            }
            else if (powerMonitor->band() == hardware::RfBand::HIGH)
            {
                startFreq = kHighBandStartFreq;
                stopFreq  = kHighBandStopFreq;
                stepFreq  = kHighBandStepFreq;

                testLimitDiffMin    = kHighBandDiffMin;
                testLimitGoodPctMin = kHighBandGoodPctMin;
                sigGenPowerInit     = kTestPowerLevelHB;
            }

            else
            {
                common::Logging::instance()->log("ERROR: No valid Power Monitor band set");
                retVal = false;
            }

            uint16_t noRfFwd, noRfRev;

            // Perform the active RF, "terminated" tests
            if (KEEP_GOING)
            {
                common::Logging::instance()->log("Active RF Input, Terminated Output Port:");
                common::LogIndent indent;

                TEST_STEP(pwrMon->getReading(noRfFwd, dut::PowerMonitor::Direction::FORWARD), "get power monitor forward reading");
                TEST_STEP(pwrMon->getReading(noRfRev, dut::PowerMonitor::Direction::REVERSE), "get power monitor reverse reading");

                TEST_STEP(te::TestEquipmentStore::signalGenerator()->setRfEnabled(true), "enable signal generator RF output");

                uint16_t pointCount    = 0;
                uint16_t goodDiffCount = 0;

                // Loop around all of the frequencies
                for (uint64_t freq = startFreq; (freq <= stopFreq) && KEEP_GOING; freq += stepFreq)
                {
                    TEST_STEP(te::TestEquipmentStore::signalGenerator()->setFrequency(freq),
                              QString("set signal generator frequency (%1 MHz)").arg(freq / 1e6));

                    // Get signal generator power to use to compensate for cable loss & measured signal generator inaccuracy
                    testjig::RfPathPowerMonitor path(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);
                    //double sigGenPower = sigGenPowerInit + testjig::CableCal::correctedPower(path, freq, 0.0);
                    double sigGenPower = sigGenPowerInit;

                    TEST_STEP(te::TestEquipmentStore::signalGenerator()->setOutputPower(sigGenPower),
                              QString("set signal generator power (%1 dBm)").arg(sigGenPower));

                    uint16_t fwd, rev;
                    TEST_STEP(pwrMon->getReading(fwd, dut::PowerMonitor::Direction::FORWARD), "get power monitor forward reading");
                    TEST_STEP(pwrMon->getReading(rev, dut::PowerMonitor::Direction::REVERSE), "get power monitor reverse reading");

                    fwd -= noRfFwd;
                    rev -= noRfRev;

                    if (KEEP_GOING) { common::Logging::instance()->log(QString("Frequency: %1 MHz").arg(freq / 1e6)); }

                    if(powerMonitor->band() == hardware::RfBand::HIGH)
                    {
                        if(freq < 5.50e9)
                        {
                            testLimitTermFwdMin = kHighBandTermFwdMin3G6;
                            testLimitTermFwdMax = kHighBandTermFwdMax3G6;
                            testLimitTermRevMin = kHighBandTermRevMin3G6;
                            testLimitTermRevMax = kHighBandTermRevMax3G6;
                        }
                        else
                        {
                            testLimitTermFwdMin = kHighBandTermFwdMin5G5;
                            testLimitTermFwdMax = kHighBandTermFwdMax5G5;
                            testLimitTermRevMin = kHighBandTermRevMin5G5;
                            testLimitTermRevMax = kHighBandTermRevMax5G5;
                        }
                    }

                    TEST_RESULT("Forward Reading", fwd, testLimitTermFwdMin, testLimitTermFwdMax, "mV");
                    TEST_RESULT("Reverse Reading", rev, testLimitTermRevMin, testLimitTermRevMax, "mV");

                    if (KEEP_GOING)
                    {
                        // Calculate difference in millivolts
                        int32_t diff = static_cast<int32_t >(fwd) - static_cast<int32_t>(rev);

                        // Is this a "good" difference reading?
                        pointCount++;
                        QString goodBad = "bad";
                        if (diff > testLimitDiffMin) { goodDiffCount++; goodBad = "good"; }

                        common::Logging::instance()->log(QString("Difference: %1mV (%2)").arg(diff).arg(goodBad));
                    }

                    common::Logging::instance()->insertBreak();
                }

                // Calculate percentage of "good" difference points
                uint8_t goodPct = static_cast<uint8_t >(round(100.0 * static_cast<double>(goodDiffCount) / pointCount));

                TEST_RESULT("\"Good\" difference points", goodPct, testLimitGoodPctMin, static_cast<uint8_t>(UCHAR_MAX), "%");
                if (KEEP_GOING) { common::Logging::instance()->insertBreak(); }
            }

            // Perform the active RF, "open port" tests
            if (KEEP_GOING)
            {
                gui::MessageDialog msg("Please remove the RF cable from the<BR>"
                                        "Power Monitor \"OUT\" Port and leave the port open",
                                        gui::MessageDialogType::INFO, true, false);
                msg.exec();

                common::Logging::instance()->log("Active RF Input, Open Output Port:");
                common::LogIndent indent;

                // Loop around all of the frequencies
                for (uint64_t freq = startFreq; (freq <= stopFreq) && KEEP_GOING; freq += stepFreq)
                {
                    TEST_STEP(te::TestEquipmentStore::signalGenerator()->setFrequency(freq),
                               QString("set signal generator frequency (%1MHz)").arg(freq / 1e6));

                    // Get signal generator power to use to compensate for cable loss & measured signal generator inaccuracy
                    testjig::RfPathPowerMonitor path(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);
                    double sigGenPower =  sigGenPowerInit  + testjig::CableCal::correctedPower(path, freq, 0.0);

                    TEST_STEP(te::TestEquipmentStore::signalGenerator()->setOutputPower(sigGenPower),
                               QString("set signal generator power (%1 dBm)").arg(sigGenPower));

                    uint16_t fwd, rev;
                    TEST_STEP(pwrMon->getReading(fwd, dut::PowerMonitor::Direction::FORWARD), "get power monitor forward reading");
                    TEST_STEP(pwrMon->getReading(rev, dut::PowerMonitor::Direction::REVERSE), "get power monitor reverse reading");

                    fwd -= noRfFwd;
                    rev -= noRfRev;

                    if (KEEP_GOING) { common::Logging::instance()->log(QString("Frequency: %1MHz").arg(freq / 1e6)); }

                    if(powerMonitor->band() == hardware::RfBand::HIGH)
                    {
                        if(freq < 2.79e9)
                        {
                            testLimitOpenFwdMin = kHighBandOpenFwdMin2G79;
                            testLimitOpenFwdMax = kHighBandOpenFwdMax2G79;
                            testLimitOpenRevMin = kHighBandOpenRevMin2G79;
                            testLimitOpenRevMax = kHighBandOpenRevMax2G79;
                        }
                        else if(freq < 4.95e9)
                        {
                            testLimitOpenFwdMin = kHighBandOpenFwdMin3G6;
                            testLimitOpenFwdMax = kHighBandOpenFwdMax3G6;
                            testLimitOpenRevMin = kHighBandOpenRevMin3G6;
                            testLimitOpenRevMax = kHighBandOpenRevMax3G6;
                        }
                        else
                        {
                            testLimitOpenFwdMin = kHighBandOpenFwdMin4G95;
                            testLimitOpenFwdMax = kHighBandOpenFwdMax4G95;
                            testLimitOpenRevMin = kHighBandOpenRevMin4G95;
                            testLimitOpenRevMax = kHighBandOpenRevMax4G95;
                        }
                    }

                    TEST_RESULT("Forward Reading", fwd, testLimitOpenFwdMin, testLimitOpenFwdMax, "mV");
                    TEST_RESULT("Reverse Reading", rev, testLimitOpenRevMin, testLimitOpenRevMax, "mV");

                    common::Logging::instance()->insertBreak();
                }
            }

            TEST_STEP(harness->enablePower(false), "disable 5.5V rail on test jig");

            // Disable signal generator RF output. Ignore any failures here, wouldn't affect test if this failed
            // when everything else passed
            (void)te::TestEquipmentStore::signalGenerator()->setRfEnabled(false);
        }

        setStatus(retVal, testsPassed);

        return retVal;
    }
}
