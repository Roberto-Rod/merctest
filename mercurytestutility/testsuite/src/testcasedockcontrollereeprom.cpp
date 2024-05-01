#include "testcasedockcontrollereeprom.hpp"
#include "dockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseDockControllerEeprom::TestCaseDockControllerEeprom(QObject* parent) :
        TestCaseDockController("EEPROM", parent)
    {
    }

    QString TestCaseDockControllerEeprom::setupMessage()
    {
        return QString();
    }

    bool TestCaseDockControllerEeprom::execute(bool manualSteps)
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

            // Get a pointer to the DUT
            dut::DockController* pDut = static_cast<dut::DockController*>(dut());

            // Get a pointer to the test harness
            testjig::TestHarnessDockController* pHarness = static_cast<testjig::TestHarnessDockController*>(pDut->testHarness());

            // Power cycle DUT & wait 1 second to allow FPGA to configure
            TEST_STEP(pHarness->powerCycle(), "power cycle DUT");
            if (KEEP_GOING) { QThread::sleep(1); }

            // The EEPROM information can be retrieved via either port, the same information is reported on the two ports so
            // in these tests we just retrieve information from the default port (channel 1). If channel 1 comms fail
            // then these tests will fail.

            // Test that EEPROM manufacturer/device ID matches that expected
            uint16_t mfrDevId;
            TEST_STEP(pDut->getEepromMfrDevId(mfrDevId), "read manufacturer/device ID from EEPROM on DUT");
            TEST_BOOL("Manufacturer/Device ID", (mfrDevId == kExpectedMfrDevId), QString("0x%1").arg(mfrDevId, 4, 16, QChar('0')));

            // Read and log the unique ID
            uint32_t uId;
            TEST_STEP(pDut->getEepromUniqueId(uId), "read unique ID from EEPROM on DUT");
            if (KEEP_GOING) { common::Logging::instance()->log(QString("Unique ID: 0x%1").arg(uId, 8, 16, QChar('0'))); }

            // Clean up - attempt regardless of test state
            pHarness->enablePower(false);
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }
}
