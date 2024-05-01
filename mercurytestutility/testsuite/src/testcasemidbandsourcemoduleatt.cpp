#include "testcasemidbandsourcemoduleatt.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "daughtermodule.hpp"
#include "rfswitchmatrix.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"

namespace testsuite
{
    const TestCaseMidBandSourceModuleAtt::AttError TestCaseMidBandSourceModuleAtt::kAttErrorPoints[] =
    {
        {  1.0, 0.2 },
        { 12.0, 0.3 },
        { 24.0, 0.7 },
        { 33.0, 1.0 },
        { 60.0, 3.0 }
    };

    const TestCaseMidBandSourceModuleAtt::AttError TestCaseMidBandSourceModuleAtt::kAttErrorPointsHB[] =
    {
        { 1.0,  0.2 },
        { 12.0, 0.5 },        //24
        { 33.0, 1.1 },
        { 42.0, 2.5 },
        { 60.0, 3.5 }
    };

    const TestCaseMidBandSourceModuleAtt::AttError TestCaseMidBandSourceModuleAtt::kAttErrorPointsHB5G8[] =
    {
        { 18.0, 1.3 },
        { 33.0, 2.0 },
        { 39.0, 3.0 },
        { 60.0, 5.0}
    };

// With fine attenuator bit D5 pulled low permanently, fine attenuator is set to 8dB.
//  Set first location to 8db and repeat for next 25 locations, then  increase attenuation in 0.25dB steps.
//  location [3,0] = 9dB. The coarse tune goes up by 6dB steps every 2 rows from here onwards.
//  Fine attenuator

    // (8 db offset, minimum attenuation is 8 even though 0 db indexed through Rf register).

/*

     const uint32_t attLUT[] =
           {
               0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,     //  8dB (first col, includinmg offset)
               0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,     //  8
               0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111111,0b111111110,0b111111101,0b111111100,     //  8 to 8.75

               0b111111011,0b111111010,0b111111001,0b111111000,0b111110111,0b111110110,0b111110101,0b111110100,0b111110011,0b111110010,0b111110001,0b111110000,     //  9  (c = 0, f 1.0 to 6.75)
               0b111101111,0b111101110,0b111101101,0b111101100,0b111101011,0b111101010,0b111101001,0b111101000,0b111100111,0b111100110,0b111100101,0b111100100,      // 12

               0b110111011,0b110111010,0b110111001,0b110111000,0b110110111,0b110110110,0b110110101,0b110110100,0b110110011,0b110110010,0b110110001,0b110110000,     //  15
               0b110101111,0b110101110,0b110101101,0b110101100,0b110101011,0b110101010,0b110101001,0b110101000,0b110100111,0b110100110,0b110100101,0b110100100,      // 18

               0b101111011,0b101111010,0b101111001,0b101111000,0b101110111,0b101110110,0b101110101,0b101110100,0b101110011,0b101110010,0b101110001,0b101110000,     //  21
               0b101101111,0b101101110,0b101101101,0b101101100,0b101101011,0b101101010,0b101101001,0b101101000,0b101100111,0b101100110,0b101100101,0b101100100,      // 24

               0b100111011,0b100111010,0b100111001,0b100111000,0b100110111,0b100110110,0b100110101,0b100110100,0b100110011,0b100110010,0b100110001,0b100110000,     //  27
               0b100101111,0b100101110,0b100101101,0b100101100,0b100101011,0b100101010,0b100101001,0b100101000,0b100100111,0b100100110,0b100100101,0b100100100,      // 30

               0b011111011,0b011111010,0b011111001,0b011111000,0b011110111,0b011110110,0b011110101,0b011110100,0b011110011,0b011110010,0b011110001,0b011110000,     //  33
               0b011101111,0b011101110,0b011101101,0b011101100,0b011101011,0b011101010,0b011101001,0b011101000,0b011100111,0b011100110,0b011100101,0b011100100,      // 36

               0b010111011,0b010111010,0b010111001,0b010111000,0b010110111,0b010110110,0b010110101,0b010110100,0b010110011,0b010110010,0b010110001,0b010110000,     //  39
               0b010101111,0b010101110,0b010101101,0b010101100,0b010101011,0b010101010,0b010101001,0b010101000,0b010100111,0b010100110,0b010100101,0b010100100,      // 42

               0b001111011,0b001111010,0b001111001,0b001111000,0b001110111,0b001110110,0b001110101,0b001110100,0b001110011,0b001110010,0b001110001,0b001110000,     //  45
               0b001101111,0b001101110,0b001101101,0b001101100,0b001101011,0b001101010,0b001101001,0b001101000,0b001100111,0b001100110,0b001100101,0b001100100,      // 48

               0b000111011,0b000111010,0b000111001,0b000111000,0b000110111,0b000110110,0b000110101,0b000110100,0b000110011,0b000110010,0b000110001,0b000110000,     //  51 (last 4 values don't fit in table!)
               0b000101111,0b000101110,0b000101101,0b000101100,0b000101011,0b000101010,0b000101001,0b000101000,0b000100111,0b000100110,0b000100101,0b000100100,      // 54 (don't fit in table)
           };

           */

const uint32_t attLUT[] =
  {
      0b111111111,0b111111110,0b111111101,0b111111100,0b111111011,0b111111010,0b111111001,0b111111000,0b111110111,0b111110110,0b111110101,0b111110100,
      0b111110011,0b111110010,0b111110001,0b111110000,0b111101111,0b111101110,0b111101101,0b111101100,0b111101011,0b111101010,0b111101001,0b111101000,      //  0 + 3
      0b110111111,0b110111110,0b110111101,0b110111100,0b110111011,0b110111010,0b110111001,0b110111000,0b110110111,0b110110110,0b110110101,0b110110100,
      0b110110011,0b110110010,0b110110001,0b110110000,0b110101111,0b110101110,0b110101101,0b110101100,0b110101011,0b110101010,0b110101001,0b110101000,      //  6 + 3
      0b101111111,0b101111110,0b101111101,0b101111100,0b101111011,0b101111010,0b101111001,0b101111000,0b101110111,0b101110110,0b101110101,0b101110100,
      0b101110011,0b101110010,0b101110001,0b101110000,0b101101111,0b101101110,0b101101101,0b101101100,0b101101011,0b101101010,0b101101001,0b101101000,      //  12+3
      0b100111111,0b100111110,0b100111101,0b100111100,0b100111011,0b100111010,0b100111001,0b100111000,0b100110111,0b100110110,0b100110101,0b100110100,
      0b100110011,0b100110010,0b100110001,0b100110000,0b100101111,0b100101110,0b100101101,0b100101100,0b100101011,0b100101010,0b100101001,0b100101000,      //  18+3
      0b011111111,0b011111110,0b011111101,0b011111100,0b011111011,0b011111010,0b011111001,0b011111000,0b011110111,0b011110110,0b011110101,0b011110100,
      0b011110011,0b011110010,0b011110001,0b011110000,0b011101111,0b011101110,0b011101101,0b011101100,0b011101011,0b011101010,0b011101001,0b011101000,      //  24+3
      0b010111111,0b010111110,0b010111101,0b010111100,0b010111011,0b010111010,0b010111001,0b010111000,0b010110111,0b010110110,0b010110101,0b010110100,
      0b010110011,0b010110010,0b010110001,0b010110000,0b010101111,0b010101110,0b010101101,0b010101100,0b010101011,0b010101010,0b010101001,0b010101000,      //  30+3
      0b001111111,0b001111110,0b001111101,0b001111100,0b001111011,0b001111010,0b001111001,0b001111000,0b001110111,0b001110110,0b001110101,0b001110100,
      0b001110011,0b001110010,0b001110001,0b001110000,0b001101111,0b001101110,0b001101101,0b001101100,0b001101011,0b001101010,0b001101001,0b001101000,      //  36+3
      0b000111111,0b000111110,0b000111101,0b000111100,0b000111011,0b000111010,0b000111001,0b000111000,0b000110111,0b000110110,0b000110101,0b000110100,
      0b000110011,0b000110010,0b000110001,0b000110000,0b000101111,0b000101110,0b000101101,0b000101100,0b000101011,0b000101010,0b000101001,0b000101000,      //  42+3
      0b000100111,0b000100110,0b000100101,0b000100100,0b000100011,0b000100010,0b000100001,0b000100000,0b000001011,0b000001010,0b000001001,0b000001000,      //  42+6
      0b000000111,0b000000110,0b000000101,0b000000100,0b000000011,0b000000010,0b000000001,0b000000000,
      000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,
      000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,
      000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,
      000000000,000000000,000000000,000000000,000000000,000000000,000000000,000000000,
  };

    TestCaseMidBandSourceModuleAtt::TestCaseMidBandSourceModuleAtt(QObject* parent) :
        TestCaseSourceBoard("Attenuator Test", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseMidBandSourceModuleAtt::setupMessage()
    {
        return QString();
    }

    bool TestCaseMidBandSourceModuleAtt::execute(bool manualSteps)
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

            // Initialise the Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // Get a pointer to the Test Harness object
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            // Note disable the power from, the source board before zeroizing the power meter
            // this is to ensure the background noise is very low (about -70dBm  as opposed to about -45dBm).
           TEST_STEP(m_pSrcBrd->enableDaughter5V5(false),                                                    "disable daughter board 5V5 supply");
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::RfAtt::DB_0),                            "set source board attenuaton to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                                                          "disable source board blanking");
            TEST_STEP(pHarness->setBlankIn(false),                                                           "disable external blanking");
            TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_1),                    "set doubler output port to 1");
            //TEST_STEP(m_pSrcBrd->setDaughterRfPath(kTestPath),                                               "set doubler RF path");

            // Create an RF switch matrix object and select the LHS DUT port on the test jig,
            // TE port will be switched throughout test
            testjig::RfSwitchMatrix jigRfSwitch(pHarness->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::RHS),                   "set jig DUT port to: RHS");
            TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::POWER_METER),          "set jig TE port to: Power Meter");

            TEST_STEP(te::TestEquipment::resetTe(teList()),                                                  "reset test equipment");
              TEST_STEP(m_pSrcBrd->enableDaughter5V5(true),                                                    "enable daughter board 5V5 supply");
             dut::DaughterModule* pDut = static_cast<dut::DaughterModule*>(dut());

            uint64_t  testFreq = 0;
            uint64_t  loopCntMax = 0;

            if(pDut->isMidBand())
                loopCntMax = 1;
            else
                loopCntMax = 2;

              if(pDut->isHighBand())
              {
                  setAttLUT();
              }

            // If High band DUT, loop twice at different frequencies to ensure
            // can is operating correctly.
            for(uint16_t  loopCnt = 0;   loopCnt < loopCntMax; loopCnt++)
            {
                uint16_t testPath;
                testFreq = getTestFreq(pDut->isMidBand(), (loopCnt == 0));

                testPath = getTestPath(pDut->isMidBand(), (loopCnt == 0));

                TEST_STEP(m_pSrcBrd->setDaughterRfPath(testPath), "set doubler RF path");

                uint64_t sigGenFreq = testFreq;
                uint16_t divisor = pDut->getRfPathDivisor(testPath);

                sigGenFreq /= divisor;

                 double sigGenPower;

                // Get signal generator power to use to compensate for cable loss & measured signal generator inaccuracy
               //testjig::RfPathPowerMonitor sigGenPath(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);

                testjig::RfPathHighBandModule   sigGenPath(testjig::RfPathHighBandModule::RfCableSet::SIG_GEN_AND_CAB0255);
                 sigGenPower = kTestPower + testjig::CableCal::correctedPower(sigGenPath, sigGenFreq, 0.0);

                TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setFrequency(sigGenFreq),    "set signal generator frequency");
                TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setOutputPower(sigGenPower), "set signal generator output power");
                TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setRfEnabled(true),          "enable signal generator RF output");

                TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->setFrequency(testFreq),           "set power meter test frequency");

                double refPower = 0.0;
                uint8_t attinit;

                attinit = 0;

                uint8_t trueAtt;
                uint16_t attPointsLimit;
                if (pDut->isMidBand())
                {
                    attPointsLimit = 211u;
                }
                else
                {
                    attPointsLimit = 180u;
                }

                for (uint8_t att = attinit; att <  attPointsLimit;)
                {
                    trueAtt = att;
                    TEST_STEP(m_pSrcBrd->setDaughterAttenuation(att), QString("set doubler attenuation to %1 dB").arg(static_cast<double>(trueAtt) / 4.0));

                    double power;
                    TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->getPower(power), "get power reading from power meter");

                    // Correct power reading for specific path on test jig
                    if(pDut->isMidBand())
                    {
                        testjig::RfPathSourceAssy pwrMeterpath(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                                        testjig::RfPathSourceAssy::DutRfPort::RHS,
                                                        testjig::RfPathSourceAssy::TePort::POWER_METER);

                        power = testjig::CableCal::correctedPower(pwrMeterpath, testFreq, power);
                    }
                    else
                    {
                        testjig::RfPathHighBandModule  hbPathPowerMeter(testjig::RfPathHighBandModule::RfCableSet::POWER_METER_AND_CAB0255);
                        power = testjig::CableCal::correctedPower(hbPathPowerMeter, testFreq, power);
                    }
                    // If this is the 0 dB point then store reference power

                    if (att == 0u) { refPower = power; }

                    double attMeasured = refPower - power;

                    TEST_BOOL(QString("Attenuation Setting %1 (%2dB)").arg(trueAtt).arg(QString::number((static_cast<double>(trueAtt) / 4.0), 'f', 2)),
                           attOk(trueAtt, attMeasured, !pDut->isMidBand(),  (loopCnt == 1)), QString("%1dB").arg(QString::number(attMeasured, 'f', 2)));

                    // Just test each step so increase by 0.25 up to 3.00dB then increase by 3.00dB

                   // If 5.8GHz loop attenuation step is 0.25dB, always.

                    if(loopCnt == 1)
                    {
                           att += 1;
                     }
                     else
                     {
                          if (att < 12u) { att += 1; }
                          else        { att += 12; }
                     }
                }  // attenuation loop
            }  // Rf Path loop

            // Clean up - attempt regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Disable signal generator output
                te::TestEquipmentStore::instance()->signalGenerator()->setRfEnabled(false);

                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    // The following (commented out) are debug routines for the high band module.

  bool  TestCaseMidBandSourceModuleAtt::outputAttPoint(uint16_t att)
    {
            uint32_t attn;
            uint16_t luatt;
            uint16_t i;
            bool retVal = true;
            bool testsPassed = true;
            i = static_cast<uint16_t>(att);
            m_pSrcBrd->getDaughterAttenuation(attn);
            m_pSrcBrd->getDaughterAttLUT(i ,luatt);
            TEST_BOOL (QString("Attenuation Reg %1").arg(attn)  , true, "Read from FPGA");
            TEST_BOOL (QString("Attenuation LUT %1").arg  (luatt), true,  "Read from FPGA LUT");
            TEST_BOOL(QString(" U 11 pins 7,5,2 = Hi.  U12 pins 7, 5 = Hi"), true, QString("FineTune"));
            TEST_BOOL(QString(" U 12 pin 2 = Hi.  U34 pin 7 = lo, pin 5 = hi, pin 2 = lo"), true, QString("Coarse Tune"));
           return retVal;
    }

   uint64_t  TestCaseMidBandSourceModuleAtt::getTestFreq(bool isMidband, bool  isFirstloop)
   {
       uint64_t testFreq;
       if(isMidband)
       {
           testFreq = kTestFreqMB * 1e6;
      }
      else if(isFirstloop)
      {
           testFreq = kTestFreqHBfirstLoop * 1e6;
       }
       else
       {
             testFreq = kTestFreqHBsecondLoop * 1e6;
       }
       return testFreq;
   }

   uint16_t  TestCaseMidBandSourceModuleAtt::getTestPath(bool isMidband, bool  isFirstloop)
   {
       uint16_t  testPath;

       if(isMidband)
       {
           testPath = 0;
       }
      else  if(isFirstloop)
      {
           testPath = 0;
       }
       else
       {
          testPath = 3;
       }
       return testPath;
   }

    bool  TestCaseMidBandSourceModuleAtt::outputLUT()
    {
        bool retVal = true;
        bool testsPassed = true;
         volatile uint16_t atta[512];
          for(uint16_t i = 0;  i < 255; i++)
          {
              uint16_t att;
               m_pSrcBrd->getDaughterAttLUT(i,att);
               atta[i]= att;
               TEST_RESULT("Att lookup table", (double)atta[i], 0.0, 255.0, "binary");
          }
          return  retVal;
    }

  bool  TestCaseMidBandSourceModuleAtt::setAttLUT()
  {
    uint16_t luti,att;
     uint32_t lutatt;
     for(luti = 0; luti <= 211; luti++)
     {
            lutatt = attLUT[luti];
            m_pSrcBrd->setDaughterAttLUT(luti,  lutatt);
            m_pSrcBrd->getDaughterAttLUT(luti, att);
     }
     return true;
  }

    bool TestCaseMidBandSourceModuleAtt::attOk(uint8_t attSetting, double attMeasured, bool isHighBand, bool isSecondLoop)
    {
        double attExpected = static_cast<double>(attSetting) / 4.0;
        double attError = std::abs(attMeasured - attExpected);
        double maxError = 0.0;

        // Max Error is dependant on whether it is Mid Band or High Band
        // and if High Band, whether it is on 5.8GHz or not
         if (!isSecondLoop)
         {
             // Midband
             if(!isHighBand)
             {
                 // Find allowed error
                 int numPoints = static_cast<int>(sizeof(kAttErrorPoints) / sizeof(kAttErrorPoints[0]));
                 for (int i = 0; i < numPoints; i++)
                 {
                     if (attExpected <= kAttErrorPoints[i].rangeMaxAtt)
                     {
                         maxError = kAttErrorPoints[i].maxError;
                         break;
                     }
                 }
             }
             // HighBand 2.4 GHz
             else
             {
                 // Find allowed error
                 int numPoints = static_cast<int>(sizeof(kAttErrorPointsHB) / sizeof(kAttErrorPointsHB[0]));
                 for (int i = 0; i < numPoints; i++)
                 {
                     if (attExpected <= kAttErrorPointsHB[i].rangeMaxAtt)
                     {
                         maxError = kAttErrorPointsHB[i].maxError;
                         break;
                     }
                 }
             }
         }
         // Highband 5.8GHz
         else
         {
             // Find allowed error
             int numPoints = static_cast<int>(sizeof(kAttErrorPointsHB5G8) / sizeof(kAttErrorPointsHB5G8[0]));
             for (int i = 0; i < numPoints; i++)
             {
                 if (attExpected <= kAttErrorPointsHB5G8[i].rangeMaxAtt)
                 {
                     maxError = kAttErrorPointsHB5G8[i].maxError;
                     break;
                 }
             }
         }

        // Now test actual error against max allowed
        return (attError <= maxError);
    }
}
