#include "testcasectsboardsetserialnumber.hpp"
#include "ctsboard.hpp"
#include "logging.hpp"

namespace testsuite
{
    TestCaseCtsBoardSetSerialNumber::TestCaseCtsBoardSetSerialNumber(QObject* parent) :
        TestCaseCtsBoard("Set Serial Number", parent)
    {
    }

    QString TestCaseCtsBoardSetSerialNumber::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardSetSerialNumber::execute(bool manualSteps)
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

            // Get the currently stored serial number
            uint32_t programmedSerial;
            TEST_STEP(m_pCtsBrd->readSerialNumber(programmedSerial), "read currently programmed serial number");

            if (retVal)
            {
                bool ok;
                uint32_t newSerial(m_pCtsBrd->serialNumber().toUInt(&ok));

                if (ok)
                {
                    // Only write the serial number if it has changed, this preserves the memory
                    // used to store serial numbers in CTS which is One Time Programmable. There
                    // are 128 slots and once the serial number has been written 128 times it cannot
                    // be changed again.
                    if (programmedSerial != newSerial)
                    {
                        TEST_STEP((ok = m_pCtsBrd->writeSerialNumber(newSerial)), "write serial number to DUT");
                        TEST_BOOL("Write Serial Number to DUT", ok, "");
                    }
                    else
                    {
                        common::Logging::instance()->log("Write Serial Number to DUT skipped as it has not changed");
                    }
                }
                else
                {
                    common::Logging::instance()->log(QString("ERROR: could not convert serial number \"%1\" to unsigned integer").arg(m_pCtsBrd->serialNumber()));
                    retVal = false;
                }
            }

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
