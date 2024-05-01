#include "testcasemidbandsourcemoduleassy.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "daughtermodule.hpp"
#include "rfswitchmatrix.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "cablecal.hpp"
#include "messagedialog.hpp"

namespace testsuite
{
    TestCaseMidBandSourceModuleAssy::TestCaseMidBandSourceModuleAssy(QObject* parent) :
        TestCaseSourceBoard("Complete Assembly", parent)
    {
        // Register the test equipment that this Test Case uses
        m_teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));
    }

    QString TestCaseMidBandSourceModuleAssy::setupMessage()
    {
        return QString("Please complete the assembly by removing the Signal Generator connection and<BR>"
                        "connecting DUT SK5  (RF OUT2) to DUT J2 (RF I/P) using CAB0124.");
    }

    bool TestCaseMidBandSourceModuleAssy::execute(bool manualSteps)
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

            // Create an RF switch matrix object and select the LHS DUT port on the test jig,
            // TE port will be switched throughout test
            testjig::RfSwitchMatrix jigRfSwitch(pHarness->m_pJigRegs);
            TEST_STEP(jigRfSwitch.setDutRfPort(testjig::RfPathSourceAssy::DutRfPort::RHS),          "set jig DUT port to: RHS");
            TEST_STEP(jigRfSwitch.setTestEquipment(testjig::RfPathSourceAssy::TePort::POWER_METER), "set jig TE port to: Power Meter");

            TEST_STEP(te::TestEquipment::resetTe(teList()),                                         "reset test equipment");

            uint64_t ddsFreq , testFreq;
            double minPowerOut;

            // Is the test frequency via a doubled path?
            uint16_t divisor = 1;

            dut::DaughterModule* pDut = static_cast<dut::DaughterModule*>(dut());
            divisor = pDut->getRfPathDivisor(kTestPath);

            if(pDut->isMidBand())
            {
                testFreq = kTestFreqMB * 1e6;
                minPowerOut = kMinPowerOutMB;
            }
            else
            {
                 testFreq = kTestFreqHB * 1e6;
                 minPowerOut = kMinPowerOutHB;
            }

            ddsFreq = testFreq;
            ddsFreq /= divisor;

            TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->setFrequency(testFreq),              "set power meter test frequency");

            TEST_STEP(m_pSrcBrd->enableDaughter5V5(true),                                                    "enable daughter board 5V5 supply");
            TEST_STEP(m_pSrcBrd->setRfAttenuation(dut::SourceBoard::RfAtt::DB_0),                            "set source board attenuation to 0dB");
            TEST_STEP(m_pSrcBrd->setRfBlank(false),                                                          "disable source board blanking");
            TEST_STEP(pHarness->setBlankIn(false),                                                           "disable external blanking");
            TEST_STEP(m_pSrcBrd->setDaughterOutputPort(dut::SourceBoard::RfPort::PORT_1),                    "set doubler output port to 1");
            TEST_STEP(m_pSrcBrd->setRfOutputPort(dut::SourceBoard::RfPort::PORT_2),                          "set source board output port to 2");
            TEST_STEP(m_pSrcBrd->setDaughterRfPath(kTestPath),                                               "set doubler RF path");
            TEST_STEP(m_pSrcBrd->setDaughterAttenuation(0u),                                                 "set doubler attenuation to 0 dB");
            TEST_STEP(m_pSrcBrd->initialiseSynth(),                                                          "initialise synthesizer");
            TEST_STEP(m_pSrcBrd->setDdsCw(ddsFreq, kTestAttQtDb),                                            "set DDS to output CW");

            double power;
            TEST_STEP(te::TestEquipmentStore::instance()->powerMeter()->getPower(power),                     "get power reading from power meter");

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

            TEST_BOOL("Output Power", (power >= minPowerOut), QString("%1dBm").arg(QString::number(power, 'f', 2)));

            // Clean up - attempt regardless of test state
            if (m_pSrcBrd != 0)
            {
                // Disable power to DUT
                pHarness->enablePower(false);

                // Disconnect DUT serial port
                m_pSrcBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
