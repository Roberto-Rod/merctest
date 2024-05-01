#include "testcasemidbandsourcemodulerfpath.hpp"
#include "daughtermodule.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"
#include "rfswitchmatrix.hpp"
#include "assyrevision.hpp"

namespace testsuite
{
    // Initialise the test points array
  /* const SourceModuleTestPointMB TestCaseMidBandSourceModuleRfPath::kTestPointMidBandRevDiss2[] =
    {
      // Path, Frequency,  Power In,  Min Power Out, F0, F2, F3, F4
      //       (KHz)        (dBm)      (dBm)
        {0,     400,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {0,     950,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {0,    1500,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },

        {1,    1490,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {1,    1685,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {1,    1880,  0.0,       6.4,   40.0,    29.0,     17.0,   18.3   },

        {2,    1850,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {2,    2050,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {2,    2250,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },

        {3,    2250,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {3,    2375,   0.0,      5.8,   40.0,    29.0,     17.0,   18.3   },
        {3,    2500,   0.0,      3.3,   40.0,    29.0,     17.0,   18.3   },

        {4,    2500,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {4,    2600,   0.0,      7.5,   40.0,    29.0,     17.0,   18.3   },
        {4,    2700,   0.0,      6.5,   40.0,    29.0,     17.0,   18.3   },

        {5,    2700,   0.0,      6.0,   40.0,    29.0,     17.0,   18.3   },
        {5,    2850,   0.0,      6.0,   40.0,    29.0,     17.0,   18.3   },
        {5,    3000,   0.0,      3.4,   40.0,    29.0,     17.0,   18.3   },
    };  */

   const SourceModuleTestPointMB TestCaseMidBandSourceModuleRfPath::kTestPointMidBandRevDiss2[] =
    {
      // Path, Frequency,  Power In,  Min Power Out, F0, F2, F3, F4
      //       (KHz)        (dBm)      (dBm)
        {0,     400,  0.0,       12.4,    40.0,    33.0,     17.0,   18.3   },
        {0,     950,  0.0,       12.2,    40.0,    43.0,     17.0,   18.3   },
        {0,    1500,  0.0,       11.4,    40.0,    38.0,     17.0,   18.3   },

        {1,    1490,  0.0,       10.8,    56.0,    29.0,     33.0,   34.0   },
        {1,    1685,  0.0,       9.38,    55.0,    29.0,     46.0,   43.0   },
        {1,    1880,  0.0,       6.5,     46.0,    29.0,     66.0,   51.0   },

        {2,    1850,   0.0,      10.9,    54.0,    29.0,     32.0,   50.0   },
        {2,    2050,   0.0,      11.2,    74.0,    29.0,     52.0,   57.0   },
        {2,    2250,   0.0,      11.2,    60.0,    29.0,     64.0,   49.0   },

        {3,    2250,   0.0,      11.1,    59.0,    29.0,     51.0,   50.0   },
        {3,    2375,   0.0,      6.65,    67.0,    29.0,     56.0,   50.0   },
        {3,    2500,   0.0,      4.44,    61.0,    29.0,     76.0,   52.0   },

        {4,    2500,   0.0,      9.33,    63.0,    29.0,     72.0,   51.0   },
        {4,    2600,   0.0,      7.48,    60.0,    29.0,     78.0,   49.0   },
        {4,    2700,   0.0,      7.65,    54.0,    29.0,     75.0,   46.0   },

        {5,    2700,   0.0,      6.89,    60.0,    29.0,     74.0,   47.0   },
        {5,    2850,   0.0,      6.5,     49.0,    29.0,     68.0,   50.0   },
        {5,    3000,   0.0,      3.2,     44.0,    29.0,     61.0,   51.0   },
    };

   const SourceModuleTestPointMB TestCaseMidBandSourceModuleRfPath::kTestPointMidBandRevCIss2[] =
    {
      // Path, Frequency,  Power In,  Min Power Out, F0, F2, F3, F4
      //       (KHz)        (dBm)      (dBm)
        {0,     400,  0.0,       11.0,    40.0,    30.0,     17.0,   18.3   },
        {0,     950,  0.0,       10.0,    40.0,    34.0,     17.0,   18.3   },
        {0,    1500,  0.0,       11.0,    40.0,    29.0,     17.0,   18.3   },

        {1,    1490,  0.0,       9.0,     42.0,    29.0,     21.0,   30.0   },
        {1,    1685,  0.0,       7.0,     48.0,    29.0,     34.0,   24.0   },
        {1,    1880,  0.0,       11.0,    47.0,    29.0,     52.0,   34.0   },

        {2,    1850,   0.0,      10.0,    54.0,    29.0,     27.0,   28.0   },
        {2,    2050,   0.0,      9.0,     61.0,    29.0,     42.0,   27.0   },
        {2,    2250,   0.0,      11.0,    57.0,    29.0,     49.0,   27.0   },

        {3,    2250,   0.0,      10.0,    55.0,    29.0,     38.0,   27.0   },
        {3,    2375,   0.0,      10.0,    68.0,    29.0,     51.0,   25.0   },
        {3,    2500,   0.0,      10.0,    40.0,    29.0,     17.0,   20.0   },

        {4,    2500,   0.0,      10.0,    57.0,    29.0,     48.0,   18.0   },
        {4,    2600,   0.0,      11.0,    64.0,    29.0,     60.0,   26.0   },
        {4,    2700,   0.0,      12.0,    56.0,    29.0,     65.0,   30.0   },

        {5,    2700,   0.0,      12.0,    61.0,    29.0,     52.0,   30.0   },
        {5,    2850,   0.0,      8.0,     57.0,    29.0,     70.0,   24.0   },
        {5,    3000,   0.0,      10.0,    54.0,    29.0,     68.0,   30.0   },
    };

   const SourceModuleTestPointMB TestCaseMidBandSourceModuleRfPath::kTestPointMidBandRevBtoC[] =
    {
      // Path, Frequency,  Power In,  Min Power Out, F0, F2, F3, F4
      //       (KHz)        (dBm)      (dBm)
        {0,     400,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {0,     950,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {0,    1500,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },

        {1,    1490,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {1,    1685,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },
        {1,    1880,  0.0,       8.0,   40.0,    29.0,     17.0,   18.3   },

        {2,    1850,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {2,    2050,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {2,    2250,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },

        {3,    2250,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {3,    2375,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {3,    2500,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },

        {4,    2500,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {4,    2600,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },
        {4,    2700,   0.0,      8.0,   40.0,    29.0,     17.0,   18.3   },

        {5,    2700,   0.0,      6.0,   40.0,    29.0,     17.0,   18.3   },
        {5,    2850,   0.0,      6.0,   40.0,    29.0,     17.0,   18.3   },
        {5,    3000,   0.0,      6.0,   40.0,    29.0,     17.0,   18.3   },
    };

   /*const SourceModuleTestPointHB TestCaseMidBandSourceModuleRfPath::kTestPointHighBand[] =
    {
      // Path, Frequency,  Power In,  Min Power Out,  Next Peak Rejection Min, Next Next Peak Rejection Min, Next Next Next Peak Rejection Min
      //          (MHz)  (dBm)  (dBm)   (dBm)   (dBm)       (dBm)
        {0,    1000,  0.0,       7.1 ,    7.5,        15.0,       17.0  },
        {0,    1500,  0.0,       8.8,     7.0,         11.0,       19.0 },
        {0,    2400,  0.0,       7.5 ,    8.0,         9.0,       20.0},

        {1,    2400, 0.0,       6.7,       20.0,      23.0,      31.0},
        {1,    2850, 0.0,       7.3,       25.0,      35.0,      35.0},
        {1,     3400, 0.0,      4.1,       23.0,      26.0,      34.0},

        {2,     3400, 0.0,      12.6,        21.0,    28.0,     34.0},
        {2,     4000, 0.0,      11.8,        41.0,    41.0,     41.0},
        {2,     4600, 0.0,       8.6,         33.0,    36.0,     39.0},

        {3,    4600, 0.0,       4.0,         8.0,    25.0,      25.0},
        {3,    5400, 0.0,       7.5,         22.0,    23.0,      28.0},
        {3,    6000, 0.0,       5.0,         28.0,    34.0,      34.0},
    };  */

   const SourceModuleTestPointHB TestCaseMidBandSourceModuleRfPath::kTestPointHighBand[] =
    {
      // Path, Frequency,  Power In,  Min Power Out,  Next Peak Rejection Min, Next Next Peak Rejection Min, Next Next Next Peak Rejection Min
      //          (MHz)  (dBm)  (dBm)   (dBm)   (dBm)       (dBm)
        {0,    1000,  0.0,     -4.5 ,      12.0,      18.0,      29.0},
        {0,    1500,  0.0,      2.6,       26.0,      32.0,      37.0},
        {0,    2400,  0.0,      0.6 ,      17.0,      35.0,      36.0},

        {1,    2400,  0.0,      0.8,       23.0,      36.0,      37.0},
        {1,    2850,  0.0,      2.5,       38.0,      38.0,      39.0},
        {1,    3400,  0.0,     -0.5,       33.0,      34.0,      35.0},

        {2,    3400,  0.0,      3.5,       25.0,      28.0,      36.0},
        {2,    4000,  0.0,      2.1,       36.0,      37.0,      38.0},
        {2,    4600,  0.0,      1.6,       37.0,      38.0,      38.0},

        {3,    4600,  0.0,      2.7,       13.0,      27.0,      32.0},
        {3,    5400,  0.0,      8.3,       24.0,      24.0,      43.0},
        {3,    6000,  0.0,      7.9,       43.0,      44.0,      44.0},
    };

   const SourceModuleTestPointMB * TestCaseMidBandSourceModuleRfPath::getTestPointMidBand(int revision, int i)
   {
       if(revision <= static_cast<int> (common::AssyRevision::ASY0033Rev::REVBtoC))
       {
           return &kTestPointMidBandRevBtoC[i];
       }
       else if (revision <= static_cast<int> (common::AssyRevision::ASY0033Rev::REVCIss4))
       {
           return &kTestPointMidBandRevCIss2[i];
       }
       else
       {
           return &kTestPointMidBandRevDiss2[i];
       }
   }

    TestCaseMidBandSourceModuleRfPath::TestCaseMidBandSourceModuleRfPath(QObject* parent) :
        TestCaseSourceBoard("RF Paths", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SpectrumAnalyser));
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseMidBandSourceModuleRfPath::setupMessage()
    {
        return QString();
    }

    bool TestCaseMidBandSourceModuleRfPath::execute(bool manualSteps)
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

            int revision = dut()->assyRevision()->getUUTRevInt();

            TEST_STEP(m_pSrcBrd->enableDaughter5V5(true),                                                    "enable daughter board 5V5 supply");
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::RfAtt::DB_0),                            "set source board attenuaton to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                                                          "disable source board blanking");
            TEST_STEP(pHarness->setBlankIn(false),                                                           "disable external blanking");
            TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_1),                    "set doubler output port to 1");
            TEST_STEP(m_pSrcBrd->setDaughterRfPath(0u),                                                      "set doubler RF path to 0");
            TEST_STEP(m_pSrcBrd->setDaughterAttenuation(0u),                                                 "set doubler attenuation to 0");

            // Create an RF switch matrix object and select the LHS DUT port on the test jig,
            // TE port will be switched throughout test
            testjig::RfSwitchMatrix jigRfSwitch(pHarness->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::RHS),                   "set jig DUT port to: RHS");

            TEST_STEP(te::TestEquipment::resetTe(teList()),                                                  "reset test equipment");

             dut::DaughterModule* pDut = static_cast<dut::DaughterModule*>(dut());

            if(pDut->isMidBand())
             {
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setRefLevel(kAnalyserRefLev),   "set spectrum analyser reference level");
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setSpan(kAnalyserSpan),         "set spectrum analyser span");
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setResBw(kAnalyserRbw),         "set spectrum analyser RBW");
            }
            else
            {
                //TEST_STEP(m_pSrcBrd->setDaughterAttenuation(12u),                                               "set doubler attenuation to 0");
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setRefLevel(kAnalyserRefLevHB), "set spectrum analyser reference level");
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setResBw(kAnalyserRbwHB),       "set spectrum analyser RBW");
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setStartFrequency(20),          "set spectrum analyser start");
                TEST_STEP(te::TestEquipmentStore::instance()->spectrumAnalyser()->setStopFrequency(6500),         "set spectrum analyser stop")
            }

            TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setRfEnabled(true),                  "enable signal generator RF output");

            int numTestPoints = 0;
            if(pDut->isMidBand())
            {
                if(revision <= static_cast<int>(common::AssyRevision::ASY0033Rev::REVBtoC))
                {
                    numTestPoints = static_cast<int>(sizeof(kTestPointMidBandRevBtoC) / sizeof(kTestPointMidBandRevBtoC[0]));
                }
                else
                {
                    numTestPoints =  static_cast<int>(sizeof(kTestPointMidBandRevDiss2) / sizeof(kTestPointMidBandRevDiss2[0]));
                }
            }
            else
            {
                 numTestPoints =  static_cast<int>(sizeof(kTestPointHighBand) / sizeof(kTestPointHighBand[0]));
            }

            uint8_t path = 0;
            uint64_t freq = 0.0;
            double powerIn;
            double powerOutMin;

            for (int i = 0; (i < numTestPoints) && KEEP_GOING; i++)
            {
                if(pDut->isMidBand())
                {
                    path = getTestPointMidBand(revision, i)->path;
                    freq = getTestPointMidBand(revision, i)->freq * 1000000u;
                    powerIn = getTestPointMidBand(revision, i)->powerIn;
                    powerOutMin = getTestPointMidBand(revision, i)->powerOutMin;
                }
                else
                {
                    TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_1), "set doubler output port to 1");
                    path = kTestPointHighBand[i].path;
                    freq = kTestPointHighBand[i].freq * 1000000u;
                    powerIn = kTestPointHighBand[i].powerIn;
                    powerOutMin = kTestPointHighBand[i].powerOutMin;
                }

                TEST_STEP(m_pSrcBrd->setDaughterRfPath(path), "set doubler RF path");

                common::Logging::instance()->log(QString("RF Path: %1").arg(path + 1));
                common::Logging::instance()->log(QString("Output Frequency: %1 MHz").arg(freq /1e6));
                {
                    // Indent log (automatically de-indented when object is destroyed)
                    common::LogIndent indent;

                    // If this is a doubled path then frequency at input of doubler is half that at output
                    uint64_t sigGenFreq = freq;
                    uint16_t divisor = pDut->getRfPathDivisor(path);
                    sigGenFreq /= divisor;

                    TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setFrequency(sigGenFreq),       "set signal generator to test input frequency");

                    // Get signal generator power to use to compensate for cable loss & measured signal generator inaccuracy
                    //testjig::RfPathPowerMonitor sigGenPath(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);
                    testjig::RfPathHighBandModule   sigGenPath(testjig::RfPathHighBandModule::RfCableSet::SIG_GEN_AND_CAB0255);
                    double sigGenPower = powerIn + testjig::CableCal::correctedPower(sigGenPath, sigGenFreq, 0.0);

                    TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setOutputPower(sigGenPower),    "set signal generator to compensated input power level");
                    TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::POWER_METER),          "set jig TE port to: Power Meter");

                    double power;
                    TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->getPower(power),                     "get power reading from power meter");

                    if(pDut->isMidBand())
                    {
                        // Correct power reading for specific path on test jig
                        testjig::RfPathSourceAssy pwrMeterpath(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                                            testjig::RfPathSourceAssy::DutRfPort::RHS,
                                                            testjig::RfPathSourceAssy::TePort::POWER_METER);

                        power = testjig::CableCal::correctedPower(pwrMeterpath, freq, power);
                    }
                    else
                    {
                         testjig::RfPathHighBandModule  hbPathPowerMeter(testjig::RfPathHighBandModule::RfCableSet::POWER_METER_AND_CAB0255);
                         power = testjig::CableCal::correctedPower(hbPathPowerMeter, freq, power);
                    }

                    // Round result to 1 d.p.
                    power = std::round(power * 10.0) / 10.0;

                    // Output power test - we have a minimum, just use an impossibly high figure as max (+40dBm : 10W)
                    TEST_RESULT("Output Power", power, powerOutMin, 40.0, "dBm");

                    if(pDut->isHighBand())
                    {
                        TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_2), "set doubler output port to 1");
                    }

                    // Switch TE port to Spectrum Analyser for harmonic rejection tests
                    TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::SPECTRUM_ANALYSER), "set jig TE port to: Spectrum Analyser");

                    double freqWanted = freq;
                    double powerWanted;

                    // Mid Band uses centre frequency and span
                    // High band uses start and stop frequency.
                    if(pDut->isMidBand())
                    {
                        TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->setFrequency(freqWanted), "get wanted power");
                    }

                    TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeakPower(powerWanted), "get wanted power");

                    bandTestParams params;

                    testjig::RfPathSourceAssy mbSpecAnpath(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                                           testjig::RfPathSourceAssy::DutRfPort::RHS,
                                                           testjig::RfPathSourceAssy::TePort::SPECTRUM_ANALYSER);

                    testjig::RfPathHighBandModule hbSpecAnPath(testjig::RfPathHighBandModule::RfCableSet::SPEC_AN_AND_CAB0255);

                    if(pDut->isMidBand())
                    {
                        powerWanted = testjig::CableCal::correctedPower(mbSpecAnpath, freqWanted, powerWanted);
                        params.specAnPath = static_cast<testjig::RfPathSourceAssy *>(&mbSpecAnpath);
                    }
                    else
                    {
                        powerWanted = testjig::CableCal::correctedPower(hbSpecAnPath, freqWanted, powerWanted);
                        params.specAnPath = static_cast<testjig::RfPathHighBandModule *>(&hbSpecAnPath);
                    }

                    params.manualSteps = manualSteps;
                    params.testsPassed = testsPassed;
                    params.freqWanted = freqWanted;
                    params.powerWanted = powerWanted;
                    params.sigGenFreq = sigGenFreq;

                     testsuite::SourceModuleTestPointMB   mbtp;
                     testsuite::SourceModuleTestPointHB   hbtp;

                    if(pDut->isMidBand())
                    {
                        params.pathdivisor = pDut->getRfPathDivisor(getTestPointMidBand(revision, i)->path);
                        mbtp = *getTestPointMidBand(revision, i);
                    }
                    else
                    {
                        params.pathdivisor = pDut->getRfPathDivisor(kTestPointHighBand[i].path);
                        hbtp = kTestPointHighBand[i];
                    }

                    //
                    //   SELECT MID / HIGH BAND MODULE FOR TESTING
                    //
                    if(pDut->isMidBand())
                    {
                        testsPassed  = testMidBandPathHarmonics(params,  mbtp, retVal);
                    }
                    else
                    {
                        testsPassed  = testHighBandPathHarmonics(params, hbtp, retVal);
                    }

                    common::Logging::instance()->insertBreak();
                }
            }  // loop

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

    bool TestCaseMidBandSourceModuleRfPath::testMidBandPathHarmonics(bandTestParams prm, testsuite::SourceModuleTestPointMB mbtp, bool& retVal)
    {
        bool testsPassed = prm.testsPassed;
        bool manualSteps = prm.manualSteps;

        testjig::RfPathSourceAssy * mbpathPtr = static_cast<testjig::RfPathSourceAssy *>(prm.specAnPath);
        testjig::RfPathSourceAssy mbpath = * mbpathPtr;
        if (mbpathPtr != NULL)
        {
            mbpath = *mbpathPtr;
        }
        else
        {
            return false;
        }

        if (prm.pathdivisor == 1)
        {
            // Not doubled path - just measure 2x fundamental
            double freq2F0 = prm.freqWanted * 2.0;
            double power2F0;

            // Read and correct 2F0 power
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->setFrequency(freq2F0),                 "set spectrum analyser to 2x fundamental frequency");
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeakPower(power2F0),                "get 2x fundamental power");
            power2F0 = testjig::CableCal::correctedPower(mbpath, freq2F0, power2F0);

            // Calculate rejection & round to 1 d.p.
            double reject2F0 = prm.powerWanted - power2F0;
            reject2F0 = std::round(reject2F0 * 10.0) / 10.0;

            // Test 2F0 against min limit, use impossibly large number as max limit
            TEST_RESULT("2x Fundamental Rejection", reject2F0, mbtp.F2, 100.0, "dBc");
        }
        else
        {
            // Doubled path - measure fundamental (half wanted), 3x fundamental and 4x fundamental
            double freqF0  = prm.sigGenFreq;
            double freq3F0 = freqF0 * 3.0;
            double freq4F0 = freqF0 * 4.0;
            double powerF0, power3F0, power4F0;

            // Read and correct F0 power
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->setFrequency(freqF0),                 "set spectrum analyser to fundamental frequency");
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeakPower(powerF0),                "get fundamental power");
            powerF0 = testjig::CableCal::correctedPower(mbpath, freqF0, powerF0);

            // Calculate rejection & round to 1 d.p.
            double rejectF0 = prm.powerWanted - powerF0;
            rejectF0 = std::round(rejectF0 * 10.0) / 10.0;

            // Test F0 against min limit, use impossibly large number as max limit
            TEST_RESULT("Fundamental Rejection", rejectF0, mbtp.F0, 100.0, "dBc");

            // Read and correct 3F0 power
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->setFrequency(freq3F0),                 "set spectrum analyser to 3x fundamental frequency");
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeakPower(power3F0),                "get 3x fundamental power");
            power3F0 = testjig::CableCal::correctedPower(mbpath, freq3F0, power3F0);

            // Calculate rejection & round to 1 d.p.
            double reject3F0 = prm.powerWanted - power3F0;
            reject3F0 = std::round(reject3F0 * 10.0) / 10.0;

            // Test 3F0 against min limit, use impossibly large number as max limit
            TEST_RESULT("3x Fundamental Rejection", reject3F0, mbtp.F3, 100.0, "dBc");

            // Read and correct 4F0 power
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->setFrequency(freq4F0),                 "set spectrum analyser to 4x fundamental frequency");
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeakPower(power4F0),                "get 4x fundamental power");
            power4F0 = testjig::CableCal::correctedPower(mbpath, freq4F0, power4F0);

            // Calculate rejection & round to 1 d.p.
            double reject4F0 = prm.powerWanted - power4F0;
            reject4F0 = std::round(reject4F0 * 10.0) / 10.0;

            // Test 3F0 against min limit, use impossibly large number as max limit
            TEST_RESULT("4x Fundamental Rejection", reject4F0, mbtp.F4, 100.0, "dBc");
        }

        return testsPassed;
    }

    bool  TestCaseMidBandSourceModuleRfPath::testHighBandPathHarmonics(bandTestParams prm,  testsuite::SourceModuleTestPointHB  hbtp, bool& retVal)
    {
          bool testsPassed = prm.testsPassed;
          bool manualSteps = prm.manualSteps;

            // Measure the next peak frequency and power
            uint64_t nextPeakfreq;
            double nextPeakpower;

            testjig::RfPathHighBandModule * hbpathPtr = static_cast<testjig::RfPathHighBandModule *>(prm.specAnPath);
            testjig::RfPathHighBandModule hbpath;
            if (hbpathPtr != NULL)
            {
                hbpath =  * hbpathPtr;
            }
            else
            {
                return false;
            }

            // Read and correct next peak power  (First Next Peak)
            bool nextPeak = true;
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeak(nextPeak, nextPeakfreq, nextPeakpower), "get next peak frequency and  power");
            nextPeakpower = testjig::CableCal::correctedPower(hbpath, nextPeakfreq, nextPeakpower);

            // Calculate rejection & round to 1 d.p.
            double rejectNF0 = prm.powerWanted - nextPeakpower;
            rejectNF0 = std::round(rejectNF0 * 10.0) / 10.0;

            // Test  against min limit, use impossibly large number as max limit
            TEST_RESULT("Nx Fundamental Rejection", rejectNF0, hbtp.npeak1Rejection, 100.0, "dBc");

            // Read and correct next peak power  (Second Next Peak)
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeak(nextPeak, nextPeakfreq, nextPeakpower), "get next peak frequency and  power");
            nextPeakpower = testjig::CableCal::correctedPower(hbpath, nextPeakfreq, nextPeakpower);

            // Calculate rejection & round to 1 d.p.
            rejectNF0 = prm.powerWanted - nextPeakpower;
            rejectNF0 = std::round(rejectNF0 * 10.0) / 10.0;

            // Test  against min limit, use impossibly large number as max limit
            TEST_RESULT("Nx Fundamental Rejection", rejectNF0, hbtp.npeak2Rejection, 100.0, "dBc");

            // Read and correct next peak power   (Third Next Peak)
            nextPeak = true;
            TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeak(nextPeak, nextPeakfreq, nextPeakpower), "get next peak frequency and  power");
            nextPeakpower = testjig::CableCal::correctedPower(hbpath, nextPeakfreq, nextPeakpower);

            // Calculate rejection & round to 1 d.p.
            rejectNF0 = prm.powerWanted - nextPeakpower;
            rejectNF0 = std::round(rejectNF0 * 10.0) / 10.0;

            // Test  against min limit, use impossibly large number as max limit
            TEST_RESULT("Nx Fundamental Rejection", rejectNF0, hbtp.npeak3Rejection, 100.0, "dBc");

            return testsPassed;
    }

}  // namespace
