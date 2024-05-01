#include "testcasesourceboardddscalcheck.hpp"
#include "testequipmentstore.hpp"
#include "mercurytestjig1.hpp"
#include "logging.hpp"
#include "settings.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "ddscalfile.hpp"
#include "rfswitchmatrix.hpp"
#include "assyrevision.hpp"

#include <QProgressDialog>
#include <QApplication>

namespace testsuite
{
static const uint16_t  limits_step = 20;
static const uint16_t  dds_high = 1500;
static const uint16_t dds_size  = (dds_high /  limits_step);

// Limits derived from ~400 dds cal files from STI
// (Low gain amplifier).
//   1st entry = frequency
//   2nd entry = mean - 4 * SD
//   3rd entry = mean + 4 * SD
ddsLimits m_ddslimits[dds_size] = {
        { 20,  325,  568,  },
        { 40,  322,  567,  },
        { 60,  321,  566,  },
        { 80,  322,  561,  },
        { 100,  325,  562,  },
        { 120,  330,  565,  },
        { 140,  332,  563,  },
        { 160,  335,  566,  },
        { 180,  337,  572,  },
        { 200,  335,  566,  },
        { 220,  328,  561,  },
        { 240,  324,  561,  },
        { 260,  325,  562,  },
        { 280,  327,  568,  },
        { 300,  326,  569,  },
        { 320,  329,  574,  },
        { 340,  340,  591,  },
        { 360,  345,  602,  },
        { 380,  347,  602,  },
        { 400,  353,  608,  },
        { 420,  360,  615,  },
        { 440,  358,  613,  },
        { 460,  356,  611,  },
        { 480,  362,  613,  },
        { 500,  365,  616,  },
        { 520,  363,  620,  },
        { 540,  366,  627,  },
        { 560,  374,  637,  },
        { 580,  377,  644,  },
        { 600,  378,  655,  },
        { 620,  382,  671,  },
        { 640,  379,  680,  },
        { 660,  378,  685,  },
        { 680,  377,  694,  },
        { 700,  377,  696,  },
        { 720,  376,  697,  },
        { 740,  378,  703,  },
        { 760,  376,  701,  },
        { 780,  376,  699,  },
        { 800,  388,  715,  },
        { 820,  394,  713,  },
        { 840,  391,  688,  },
        { 860,  402,  703,  },
        { 880,  421,  726,  },
        { 900,  415,  704,  },
        { 920,  416,  701,  },
        { 940,  444,  741,  },
        { 960,  455,  754,  },
        { 980,  452,  749,  },
        { 1000,  482,  797,  },
        { 1020,  507,  844,  },
        { 1040,  493,  830,  },
        { 1060,  505,  862,  },
        { 1080,  534,  917,  },
        { 1100,  503,  888,  },
        { 1120,  468,  847,  },
        { 1140,  480,  891,  },
        { 1160,  472,  909,  },
        { 1180,  432,  851,  },
        { 1200,  431,  852,  },
        { 1220,  468,  915,  },
        { 1240,  493,  950,  },
        { 1260,  498,  921,  },
        { 1280,  515,  930,  },
        { 1300,  559,  976,  },
        { 1320,  582,  993,  },
        { 1340,  580,  977,  },
        { 1360,  604,  1009,  },
        { 1380,  650,  1077,  },
        { 1400,  676,  1115,  },
        { 1420,  689,  1136,  },
        { 1440,  700,  1175,  },
        { 1460,  682,  1167,  },
        { 1480,  647,  1134,  },
        { 1500,  641,  1138,  }
       };  // end of structure

ddsLimits m_ddslimitsHitite[dds_size] = {
       { 20,  277,  331,  },
       { 40,  273,  332,  },
       { 60,  273,  337,  },
       { 80,  277,  342,  },
       { 100,  282,  347,  },
       { 120,  289,  352,  },
       { 140,  292,  356,  },
       { 160,  289,  348,  },
       { 180,  282,  343,  },
       { 200,  266,  334,  },
       { 220,  251,  332,  },
       { 240,  255,  340,  },
       { 260,  270,  365,  },
       { 280,  291,  388,  },
       { 300,  301,  393,  },
       { 320,  305,  387,  },
       { 340,  308,  388,  },
       { 360,  316,  398,  },
       { 380,  317,  407,  },
       { 400,  310,  405,  },
       { 420,  306,  406,  },
       { 440,  315,  422,  },
       { 460,  332,  442,  },
       { 480,  342,  450,  },
       { 500,  340,  442,  },
       { 520,  337,  431,  },
       { 540,  337,  428,  },
       { 560,  340,  430,  },
       { 580,  339,  431,  },
       { 600,  340,  441,  },
       { 620,  348,  459,  },
       { 640,  362,  479,  },
       { 660,  373,  495,  },
       { 680,  380,  505,  },
       { 700,  386,  513,  },
       { 720,  393,  520,  },
       { 740,  399,  532,  },
       { 760,  408,  540,  },
       { 780,  413,  546,  },
       { 800,  418,  548,  },
       { 820,  426,  561,  },
       { 840,  442,  582,  },
       { 860,  456,  599,  },
       { 880,  460,  607,  },
       { 900,  468,  617,  },
       { 920,  487,  644,  },
       { 940,  509,  672,  },
       { 960,  504,  665,  },
       { 980,  473,  633,  },
       { 1000,  480,  600,  },
       { 1020,  487,  608,  },
       { 1040,  486,  612,  },
       { 1060,  456,  596,  },
       { 1080,  443,  605,  },
       { 1100,  476,  665,  },
       { 1120,  551,  778,  },
       { 1140,  617,  865,  },
       { 1160,  622,  843,  },
       { 1180,  605,  790,  },
       { 1200,  623,  799,  },
       { 1220,  680,  856,  },
       { 1240,  722,  932,  },
       { 1260,  722,  961,  },
       { 1280,  721,  964,  },
       { 1300,  737,  984,  },
       { 1320,  759,  1024,  },
       { 1340,  781,  1057,  },
       { 1360,  775,  1049,  },
       { 1380,  721,  1002,  },
       { 1400,  669,  961,  },
       { 1420,  657,  968,  },
       { 1440,  675,  1026,  },
       { 1460,  705,  1064,  },
       { 1480,  727,  1054,  },
       { 1500,  744,  1036,  }
};

    TestCaseSourceBoardDdsCalCheck::TestCaseSourceBoardDdsCalCheck(QObject* parent) :
        TestCaseSourceBoard("DDS Calibration Check", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseSourceBoardDdsCalCheck::setupMessage()
    {
        return QString();
    }

    bool TestCaseSourceBoardDdsCalCheck::execute(bool manualSteps)
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

            this->m_hititeAmp = false;
            int revision = m_pSrcBrd->assyRevision()->getUUTRevInt();
            if(revision == static_cast<int>(common::AssyRevision::PCB0044Rev::REVBtoGIss2))
            {
                this->m_hititeAmp = true;
            }

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
            uint16_t fx, asf;
            TEST_STEP(calFile.openRo(common::Settings::instance()->reportsDirectory()), "open calibration file");
            calFile.removeHeader();

            int steps = static_cast<int>((kTestStopFreq - kTestStartFreq) / kTestStepFreq);
            QProgressDialog progress("Calibrating DDS...", "Cancel", 1, steps);
            progress.setWindowModality(Qt::ApplicationModal);
            progress.setValue(0);
            progress.setFixedSize(progress.sizeHint());
            bool progressCanceled = false;

            // Read DDS Cal file from DDS calibration test case.
            // For each frequency read the ASF value and check its within limits.
            // Use ASF value to set Rf power from DDS.
            // Check power reading is 0dBm after cable loss correction.

            for (uint64_t freq = kTestStartFreq; (freq <= kTestStopFreq) && retVal && testsPassed; freq += kTestStepFreq)
            {
                if (!calFile.atEnd()== 0)
                {
                    calFile.getFreqAndAsf(fx, asf);
                }

                this->limitcheck(fx, asf);
                QString  desc = QString("ASF  at %1 MHz :  ").arg(freq/ 1e6);

                TEST_RESULT(desc,  asf,  m_flimitLo,  m_flimitHi, " ");

                progress.setValue(progress.value() + 1);
                // Configure the power meter for this frequency
                TEST_STEP(pwrMtr->setFrequency(freq), QString("set power meter to %1 MHz").arg(freq / 1e6));

                double error = 0.0;

                progressCanceled = progress.wasCanceled();
                if(progressCanceled)
                {
                    break;
                }
                if(!testsPassed)
                {
                    break;
                }

                TEST_STEP(m_pSrcBrd-> setDdsCwUsingAsf(freq, asf), QString("set DDS to %1 MHz, %2 ASF").
                                                                 arg(freq / 1e6).
                                                                 arg(asf));

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

                QString val, min, max;
                desc = QString("Rf 0 dBm Output at %1 MHz : (dBm)").arg(freq/ 1e6);
                val = QString::number(absError,'f',2);
                min = QString::number(kDDSCalMin,'f',2);
                max = QString::number(kDDSCalMax, 'f',2);

                TEST_RESULT(desc, val, min, max, " ");

                // Process events to keep progress dialog responsive
                QApplication::processEvents();

                // Check progress dialog for cancellation
                retVal &= !progressCanceled;
             }  // iteration loop

             // Close the cal file
             calFile.close();

              // Hide the progress dialog
             progress.hide();

            // Clean up - attempt these regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Set DDS to zero (DC) output
                (void)m_pSrcBrd->setDdsCw(0, 0u);

                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }  // if retVal

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    bool TestCaseSourceBoardDdsCalCheck::limitcheck(uint16_t freq, uint16_t val)
    {
        ddsLimits * ddsPointPtr;
        bool result = false;
        for(uint16_t i = 0; i <= dds_size; i ++)
        {
            ddsPointPtr = getLimitPoint(i);

            if(freq > ddsPointPtr->fr)
            {
                continue;
            }
            else
            {
                m_flimitLo = ddsPointPtr->lo;
                m_flimitHi = ddsPointPtr->hi;

                if(val >= ddsPointPtr->lo && val <= ddsPointPtr->hi)
                {
                    result = true;
                }
                else
                {
                    result = false;
                }
                break;
            }
        }
        return result;
    }

     ddsLimits * TestCaseSourceBoardDdsCalCheck::getLimitPoint(uint16_t freq)
     {
         if (this->m_hititeAmp == true)
         {
             return &m_ddslimitsHitite[freq];
         }
         else
         {
             return &m_ddslimits[freq];
         }
     }

}
