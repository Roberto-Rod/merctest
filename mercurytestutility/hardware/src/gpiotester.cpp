#include "gpiotester.hpp"
#include "logging.hpp"

namespace hardware
{
    GpioTester::GpioTester(Gpio& jigGpio, Gpio& dutGpio, QObject* parent) :
        testsuite::Tester(parent),
        m_jigGpio(jigGpio),
        m_dutGpio(dutGpio),
        m_status(testsuite::TestStatus::NOT_TESTED),
        m_errorMask(0u)
    {
    }

    bool GpioTester::execute(bool& testsPassed, bool asInput, bool asOutput)
    {
        bool retVal = false;
        testsPassed = false;

        // Start with true return value and a PASS status if at least one test is commanded
        if (asInput || asOutput)
        {
            retVal = true;
            testsPassed = true;
        }

        if (asInput)
        {
            // Execute input test
            retVal &= execute(m_jigGpio, m_dutGpio, "As input");

            // Set status to FAILED if there are any bits set in the error mask
            if (m_errorMask != 0u) { testsPassed = false; }
        }

        if (asOutput)
        {
            // Execute output test
            retVal &= execute(m_dutGpio, m_jigGpio, "As output");

            // Set status to FAILED if there are any bits set in the error mask
            if (m_errorMask != 0u) { m_status = testsuite::TestStatus::FAILED; }
        }

        return retVal;
    }

    bool GpioTester::execute(Gpio& srcGpio, Gpio& destGpio, QString testName)
    {
        // Return value tells us that comms worked
        bool retVal = false;

        // Reset the error mask
        m_errorMask = 0u;

        // Only continue with test if GPIO widths match and are non-zero
        if ((srcGpio.width() == destGpio.width()) && (srcGpio.width() > 0))
        {
            if (!destGpio.setDirection(Gpio::Dir::IN))
            {
                // Destination error
                common::Logging::instance()->log(QString ("ERROR: failed to configure destination!"));
            }
            else if (!srcGpio.setDirection(Gpio::Dir::OUT))
            {
                // Source error
                common::Logging::instance()->log(QString ("ERROR: failed to configure source!"));
            }
            else
            {
                retVal = true;

                // All zeros
                retVal &= allZeros(srcGpio, destGpio);

                // All ones
                retVal &= allOnes(srcGpio, destGpio);

                // All zeros again
                retVal &= allZeros(srcGpio, destGpio);

                // Stripe a one
                retVal &= stripe(srcGpio, destGpio, false);

                // Stripe a zero
                retVal &= stripe(srcGpio, destGpio, true);
            }
        }

        // Now log the overall result
        QString statusString = testName + ": ";

        if (retVal)
        {
            // Test executed - test and log the error mask (8 hex digits, upper case)
            if (m_errorMask == 0u)
            {
                statusString += "Pass";
            }
            else
            {
                statusString += QString("**FAIL** (err mask: 0x%1)").
                                arg(m_errorMask, 8, 16, QChar('0'));
            }
        }
        else
        {
            // Test failed to execute
            statusString += "Failed to execute!";
        }

        common::Logging::instance()->log(statusString);

        return retVal;
    }

    bool GpioTester::allZeros(Gpio& srcGpio, Gpio& destGpio)
    {
        // Return value tells us that comms worked
        bool retVal = true;

        uint8_t width = srcGpio.width();
        uint32_t testMask = Gpio::getMask(width);
        uint32_t val;

        // Set all zeros at the source port
        retVal &= srcGpio.setData(0u, width, 0x00000000);

        // Get the data from the destination port
        retVal &= destGpio.getData(0u, width, val);

        // Any bits that are set are errors
        uint32_t error = (val & testMask);

        // OR these errors into the member variable which stores all of the errors
        m_errorMask |= error;

        return retVal;
    }

    bool GpioTester::allOnes(Gpio& srcGpio, Gpio& destGpio)
    {
        // Return value tells us that comms worked
        bool retVal = true;

        uint8_t width = srcGpio.width();
        uint32_t testMask = Gpio::getMask(width);
        uint32_t readVal;

        // Set all ones at the source port
        retVal &= srcGpio.setData(0u, width, 0xFFFFFFFF);

        // Get the data from the destination port
        retVal &= destGpio.getData(0u, width, readVal);

        // Any bits that are not set are errors
        uint32_t error = (~readVal & testMask);

        // OR these errors into the member variable which stores all of the errors
        m_errorMask |= error;

        return retVal;
    }

    bool GpioTester::stripe(Gpio& srcGpio, Gpio& destGpio, bool invert)
    {
        // Return value tells us that comms worked
        bool retVal = true;

        uint8_t width = srcGpio.width();
        uint32_t testMask = Gpio::getMask(width);

        // Initial stripe value is one (at LSB position)
        uint32_t stripe = 1u;

        // Stripe the one through the port
        for (uint8_t i = 0; i < width; i++)
        {
            // Set the stripe value at the source port
            uint32_t writeVal = invert ? ~stripe : stripe;
            retVal &= srcGpio.setData(0u, width, writeVal);

            // Get the data from the destination port
            uint32_t readVal;
            retVal &= destGpio.getData(0u, width, readVal);

            // Any bits which differ between read and write are an error
            uint32_t error = (writeVal ^ readVal) & testMask;

            // OR these errors into the member variable which stores all of the errors
            m_errorMask |= error;

            // Left-shift the stripe value (zero shifted in to LSB)
            stripe <<= 1;
        }

        return retVal;
    }

    testsuite::TestStatus GpioTester::status()
    {
        return m_status;
    }
}
