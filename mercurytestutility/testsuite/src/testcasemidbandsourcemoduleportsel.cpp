#include "testcasemidbandsourcemoduleportsel.hpp"
#include "sourceboard.hpp"
#include "daughtermodule.hpp"
#include "testharnesssourceboard.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "rfswitchmatrix.hpp"
#include "cablecal.hpp"

#include <QProcess>
#include <QThread>

namespace testsuite
{
    const double TestCaseMidBandSourceModulePortSel::kTestPower   = 0.0;
    const double TestCaseMidBandSourceModulePortSel::kPowerOutMinMB = 9.0;
    const double TestCaseMidBandSourceModulePortSel::kPowerOutMinHB = 2.0;

    TestCaseMidBandSourceModulePortSel::TestCaseMidBandSourceModulePortSel(QObject* parent) :
        TestCaseSourceBoard("Port Selection", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::SpectrumAnalyser));
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseMidBandSourceModulePortSel::setupMessage()
    {
        return QString();
    }

    bool TestCaseMidBandSourceModulePortSel::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed = true;

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal = TestCase::execute();

        dut::DaughterModule* pDut = static_cast<dut::DaughterModule*>(dut());

         uint64_t  Tfrequency;
         double TpowerOutMin;

        if(pDut->isMidBand())
        {
            Tfrequency = kTestFrequencyMB * 1e6;
            TpowerOutMin = kPowerOutMinMB;
        }
        else
        {
             Tfrequency = kTestFrequencyHB * 1e6;
             TpowerOutMin = kPowerOutMinHB;
        }

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise the Source Board
            TEST_STEP(initialiseSrcBrd(), "initialise DUT");

            // Get a pointer to the Test Harness object
            testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(dut()->testHarness());

            TEST_STEP(m_pSrcBrd->enableDaughter5V5(true),                                                    "enable daughter board 5V5 supply");
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::RfAtt::DB_0),                            "set source board attenuaton to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                                                          "disable source board blanking");
            TEST_STEP(pHarness->setBlankIn(false),                                                           "disable external blanking");
            TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_1),                    "set doubler output port to 1");
            TEST_STEP(m_pSrcBrd->setDaughterRfPath(0u),                                                      "set doubler RF path to 0");
            TEST_STEP(m_pSrcBrd->setDaughterAttenuation(0u),                                                 "set doubler attenuation to 0");

            // Create an RF switch matrix object and select the Power Meter port
            // LHS DUT port on the test jig
            testjig::RfSwitchMatrix jigRfSwitch(pHarness->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::POWER_METER),          "set jig TE port to: Power Meter");
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::RHS),                   "set jig DUT port to: RHS");

            TEST_STEP(te::TestEquipment::resetTe(teList()),                                                  "reset test equipment");
            uint16_t divisor = pDut->getRfPathDivisor(0);
            uint64_t sigGenFreq = Tfrequency / divisor;

            TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setFrequency(sigGenFreq),       "set signal generator to test frequency");

            // Get signal generator power to use to compensate for cable loss & measured signal generator inaccuracy
            //testjig::RfPathPowerMonitor sigGenPath(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM);
            testjig::RfPathHighBandModule   sigGenPath(testjig::RfPathHighBandModule::RfCableSet::SIG_GEN_AND_CAB0255);
            double sigGenPower = kTestPower + testjig::CableCal::correctedPower(sigGenPath, sigGenFreq, kTestPower);

            TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setOutputPower(sigGenPower),    "set signal generator to test power");
            TEST_STEP(te::TestEquipmentStore::instance()->signalGenerator()->setRfEnabled(true),             "enable signal generator RF output");

            QThread::msleep(500);

            double power;

            TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->getPower(power),                     "get power from power meter");

            // Correct power reading for specific path on test jig
            testjig::RfPathSourceAssy pwrMeterpath(testjig::RfPathSourceAssy::DutRfCable::LONG,
                                                    testjig::RfPathSourceAssy::DutRfPort::RHS,
                                                    testjig::RfPathSourceAssy::TePort::POWER_METER);

            power = testjig::CableCal::correctedPower(pwrMeterpath, Tfrequency, power);

            TEST_BOOL("Port 1", (power >= TpowerOutMin), "");

            // Port two tests, High Band uses sptrum Analyser, Mid band uses  the power meter.

            TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_2),  "set doubler output port to 1");
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::LHS), "set jig DUT port to: LHS");

            if(pDut->isHighBand())
            {
                // Switch TE port to Spectrum Analyser for port 2 test
                TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::SPECTRUM_ANALYSER), "set jig TE port to: Spectrum Analyser");

                testjig::RfPathHighBandModule  hbPathSpectrumAnalyser(testjig::RfPathHighBandModule::RfCableSet::SPEC_AN_AND_CAB0255);

                double power;
                TEST_STEP(te::TestEquipmentStore::spectrumAnalyser()->getPeakPower(power), "get wanted power");

                power = testjig::CableCal::correctedPower(hbPathSpectrumAnalyser,  Tfrequency, power);
            }  // is high band

            else  // Mid Band, use power meter
            {
                TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->getPower(power), "get power from power meter");

                // Correct the power for the new path
                pwrMeterpath.setDutPort(testjig::RfPathSourceAssy::DutRfPort::LHS);
                power = testjig::CableCal::correctedPower(pwrMeterpath,  Tfrequency, power);
            }

            TEST_BOOL("Port 2", (power >= TpowerOutMin), "");

            // Clean up - attempt regardless of test state
            if (m_pSrcBrd != 0)
            {
                m_pSrcBrd->enableDaughter5V5(false);
                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }  // If ret Val is good

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
