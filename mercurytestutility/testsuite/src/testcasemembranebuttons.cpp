#include "testcasemembranebuttons.hpp"
#include "membrane.hpp"
#include "testharness.hpp"
#include "actiondialog.hpp"
#include "logging.hpp"

namespace testsuite
{
    // Constant definitions within namespace, in addition to the declaration in the class,
    // needed to satisfier linker when linking the constructor
    const uint8_t TestCaseMembraneButtons::kJammingButton;
    const uint8_t TestCaseMembraneButtons::kAlarmButton;
    const uint8_t TestCaseMembraneButtons::kZeroizeButton;
    const uint8_t TestCaseMembraneButtons::kPowerButton;

    TestCaseMembraneButtons::TestCaseMembraneButtons(QObject* parent) :
        TestCase(QString("Membrane Buttons"), parent),
        m_pLedGpio(0),
        m_pButtonState(0)
    {
        m_buttonMap[kPowerButton]   = QString("Power (Green)");
        m_buttonMap[kJammingButton] = QString("Jamming (Blue)");
        m_buttonMap[kAlarmButton]   = QString("Alarm (!) (Red)");
        m_buttonMap[kZeroizeButton] = QString("Zeroize (X) (Red)");
    }

    TestCaseMembraneButtons::~TestCaseMembraneButtons()
    {
    }

    QString TestCaseMembraneButtons::setupMessage()
    {
        return QString();
    }

    bool TestCaseMembraneButtons::execute(bool manualSteps)
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

            m_pLedGpio = new hardware::RegisterGpio(membrane->testHarness()->m_pJigRegs, kAddrOffsetLedGpio, kLedGpioWidth);
            m_pButtonState = new hardware::RegisterGpio(membrane->testHarness()->m_pJigRegs, kRegAddrButtonState, kButtonWidth);

            // Set all the LEDs to outputs
            TEST_STEP(m_pLedGpio->setDirection(kLedLsb, kLedGpioWidth, hardware::Gpio::Dir::OUT), "enable LED outputs");

            // Set all the LEDs to outputs off (logic '1') & power LED on then test the power button
            TEST_STEP(m_pLedGpio->setData(kLedLsb, kLedGpioWidth, 0xFFFFFFFF), "turn all LEDs off");
            TEST_STEP(m_pLedGpio->setData(kPowerLed, false),                   "turn power LED on");
            TEST_STEP(testButton(kPowerButton, manualSteps, testsPassed),      "test power button");

            // Set all the LEDs to outputs off (logic '1') & jamming green LED on then test the jamming button
            TEST_STEP(m_pLedGpio->setData(kLedLsb, kLedGpioWidth, 0xFFFFFFFF), "turn all LEDs off");
            TEST_STEP(m_pLedGpio->setData(kJammingGreenLed, false),            "turn green jamming LED on");
            TEST_STEP(testButton(kJammingButton, manualSteps, testsPassed),    "test jamming button");

            // Set all the LEDs to outputs off (logic '1') & alarm green LED on then test the alarm button
            TEST_STEP(m_pLedGpio->setData(kLedLsb, kLedGpioWidth, 0xFFFFFFFF), "turn all LEDs off");
            TEST_STEP(m_pLedGpio->setData(kAlarmGreenLed, false),              "turn green alarm LED on");
            TEST_STEP(testButton(kAlarmButton, manualSteps, testsPassed),      "test alarm button");

            // Set all the LEDs to outputs off (logic '1') & zeroize green LED on then test the zeroize button
            TEST_STEP(m_pLedGpio->setData(kLedLsb, kLedGpioWidth, 0xFFFFFFFF), "turn all LEDs off");
            TEST_STEP(m_pLedGpio->setData(kZeroizeGreenLed, false),            "turn green zeroize LED on");
            TEST_STEP(testButton(kZeroizeButton, manualSteps, testsPassed),    "test zeroize button");

            // Set the entire register to inputs
            TEST_STEP(m_pLedGpio->setDirection(0u, kLedGpioWidth, hardware::Gpio::Dir::IN), "disable LED outputs");

            // Set and log the status
            setStatus(retVal, testsPassed);

            // Delete the membrane IO object and set the pointer to 0
            delete m_pLedGpio;
            delete m_pButtonState;
            m_pLedGpio = 0;
            m_pButtonState = 0;
        }

        return retVal;
    }

    bool TestCaseMembraneButtons::testButton(uint8_t buttonGpioPos, bool manualSteps, bool& testsPassed)
    {
        bool retVal = false;

        if (m_pLedGpio != 0)
        {
            retVal = true;

            m_buttonUnderTest = buttonGpioPos;

            QString title = QString("Press Membrane Button");
            QString msg   = QString("Please press and hold the <b>%1</b> button.").arg(m_buttonMap[buttonGpioPos]);

            gui::ActionDialog dialog(title, msg);

            connect(&dialog, SIGNAL(getTestState()),     this, SLOT(getTestState()));
            connect(   this, SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

            // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
            TEST_STEP(dialog.exec(), QString("wait for %1 button press").arg(m_buttonMap[buttonGpioPos]));

            // Get the test state
            TEST_BOOL(QString("%1 Button").arg(m_buttonMap[buttonGpioPos]), dialog.status(), "");
        }

        return retVal;
    }

    void TestCaseMembraneButtons::getTestState()
    {
        bool state = false;
        bool buttonState = false;

        if (m_pButtonState != 0)
        {
            if (m_pButtonState->getData(m_buttonUnderTest, buttonState))
            {
                state = buttonState;
            }
        }

        emit testState(state);
    }
}
