#include "testcasemembraneleds.hpp"
#include "membrane.hpp"
#include "testharness.hpp"
#include "manualtestdialog.hpp"
#include "logging.hpp"
#include "settings.hpp"

namespace testsuite
{
    // Constant definitions within namespace, in addition to the declaration in the class,
    // needed to satisfier linker when linking the constructor
    const uint8_t TestCaseMembraneLeds::kJammingGreenLed;
    const uint8_t TestCaseMembraneLeds::kJammingRedLed;
    const uint8_t TestCaseMembraneLeds::kAlarmGreenLed;
    const uint8_t TestCaseMembraneLeds::kAlarmRedLed;
    const uint8_t TestCaseMembraneLeds::kZeroizeGreenLed;
    const uint8_t TestCaseMembraneLeds::kZeroizeRedLed;
    const uint8_t TestCaseMembraneLeds::kSyncLed;
    const uint8_t TestCaseMembraneLeds::kPowerLed;

    TestCaseMembraneLeds::TestCaseMembraneLeds(QObject* parent) :
        TestCase(QString("Membrane LEDs"), parent)
    {
        m_ledMap[kPowerLed]        = QString("Green Power");
        m_ledMap[kJammingGreenLed] = QString("Green Jamming");
        m_ledMap[kJammingRedLed]   = QString("Red Jamming");
        m_ledMap[kAlarmGreenLed]   = QString("Green Alarm (!)");
        m_ledMap[kAlarmRedLed]     = QString("Red Alarm (!)");
        m_ledMap[kZeroizeGreenLed] = QString("Green Zeroize (X)");
        m_ledMap[kZeroizeRedLed]   = QString("Red Zeroize (X)");
        m_ledMap[kSyncLed]         = QString("Green Sync");
    }

    TestCaseMembraneLeds::~TestCaseMembraneLeds()
    {
    }

    QString TestCaseMembraneLeds::setupMessage()
    {
        return QString();
    }

    bool TestCaseMembraneLeds::execute(bool manualSteps)
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

            dut::Membrane* membrane = dynamic_cast<dut::Membrane*>(dut());

            m_pMembraneIo = new hardware::RegisterGpio(membrane->testHarness()->m_pJigRegs, kAddrOffsetLedGpio, kLedGpioWidth);

            // Set all the LEDs to outputs off (logic '1')
            TEST_STEP(m_pMembraneIo->setData(kLedLsb, kLedGpioWidth, 0xFFFFFFFF),                    "turn all LEDs off");

            // Set all the LEDs to outputs
            TEST_STEP(m_pMembraneIo->setDirection(kLedLsb, kLedGpioWidth, hardware::Gpio::Dir::OUT), "enable LED outputs");

            TEST_STEP(testLed(kPowerLed, manualSteps, testsPassed),        "test power LED");
            TEST_STEP(testLed(kJammingGreenLed, manualSteps, testsPassed), "test green jamming LED");
            TEST_STEP(testLed(kJammingRedLed, manualSteps, testsPassed),   "test red jamming LED");
            TEST_STEP(testLed(kAlarmGreenLed, manualSteps, testsPassed),   "test green alarm LED");
            TEST_STEP(testLed(kAlarmRedLed, manualSteps, testsPassed),     "test red alarm LED");
            TEST_STEP(testLed(kZeroizeGreenLed, manualSteps, testsPassed), "test green zeroize LED");
            TEST_STEP(testLed(kZeroizeRedLed, manualSteps, testsPassed),   "test red zeroize LED");

            if (membrane->syncLedPresent())
            {
                TEST_STEP(testLed(kSyncLed, manualSteps, testsPassed),     "test sync LED");
            }

            // Set the entire register to inputs
            TEST_STEP(m_pMembraneIo->setDirection(0u, kLedGpioWidth, hardware::Gpio::Dir::IN),       "disable LED outputs");

            // Set and log the status
            setStatus(retVal, testsPassed);

            // Delete the membrane IO object and set the pointer to 0
            delete m_pMembraneIo;
            m_pMembraneIo = 0;
        }

        return retVal;
    }

    bool TestCaseMembraneLeds::testLed(uint8_t ledGpioPos, bool manualSteps, bool& testsPassed)
    {
        // Return value tells us all the comms worked
        bool retVal = false;

        if (m_pMembraneIo != 0)
        {
            retVal = true;

            // Turn LED On
            TEST_STEP(m_pMembraneIo->setData(ledGpioPos, false), QString("turn %1 LED on").arg(m_ledMap[ledGpioPos]));

            if (KEEP_GOING)
            {
                // Display a test dialog
                QString title = QString("Verify Membrane LED");
                QString msg   = QString("Verify that the <b>%1</b> LED is illuminated<br>"
                                         "(and no other LED is illuminated).").arg(m_ledMap[ledGpioPos]);

                gui::ManualTestDialog dialog(title, msg);

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), QString("wait for %1 LED confirmation from user").arg(m_ledMap[ledGpioPos]));

                // Get the test state
                TEST_BOOL(QString("%1 LED").arg(m_ledMap[ledGpioPos]), dialog.status(), "");
            }

            // Turn LED Off
            TEST_STEP(m_pMembraneIo->setData(ledGpioPos, true), QString("turn %1 LED off").arg(m_ledMap[ledGpioPos]));
        }

        return retVal;
    }
}
