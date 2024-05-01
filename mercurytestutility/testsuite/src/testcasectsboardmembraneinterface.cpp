#include "testcasectsboardmembraneinterface.hpp"
#include "ctsboard.hpp"
#include "logging.hpp"
#include "directserialcomms.hpp"
#include "actiondialog.hpp"
#include "manualtestdialog.hpp"

namespace testsuite
{
    TestCaseCtsBoardMembraneInterface::TestCaseCtsBoardMembraneInterface(QObject* parent) :
        TestCaseCtsBoard("Membrane Interface", parent),
        m_buttonUnderTest(dut::CtsBoard::Button::Power)
    {
        m_buttonMap[dut::CtsBoard::Button::Power]       = QString("Power (Green)");
        m_buttonMap[dut::CtsBoard::Button::Scan]        = QString("Scan (Blue)");
        m_buttonMap[dut::CtsBoard::Button::Exclamation] = QString("Alarm (!) (Red)");
        m_buttonMap[dut::CtsBoard::Button::Cross]       = QString("Zeroize (X) (Red)");
        m_ledMap[dut::CtsBoard::Led::Power]             = QString("Green Power");
        m_ledMap[dut::CtsBoard::Led::ScanGreen]         = QString("Green Scanning");
        m_ledMap[dut::CtsBoard::Led::ScanRed]           = QString("Red Scanning");
        m_ledMap[dut::CtsBoard::Led::ExclamationGreen]  = QString("Green Alarm (!)");
        m_ledMap[dut::CtsBoard::Led::ExclamationRed]    = QString("Red Alarm (!)");
        m_ledMap[dut::CtsBoard::Led::CrossGreen]        = QString("Green Zeroize (X)");
        m_ledMap[dut::CtsBoard::Led::CrossRed]          = QString("Red Zeroize (X)");
        m_ledMap[dut::CtsBoard::Led::Battery]           = QString("Green Charging");
        m_buttonToLedMap[dut::CtsBoard::Button::Power]       = dut::CtsBoard::Led::Power; 
        m_buttonToLedMap[dut::CtsBoard::Button::Scan]        = dut::CtsBoard::Led::ScanGreen;
        m_buttonToLedMap[dut::CtsBoard::Button::Exclamation] = dut::CtsBoard::Led::ExclamationGreen;
        m_buttonToLedMap[dut::CtsBoard::Button::Cross]       = dut::CtsBoard::Led::CrossGreen;
    }

    QString TestCaseCtsBoardMembraneInterface::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardMembraneInterface::execute(bool manualSteps)
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
            QString usbPortName;

            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "initialise DUT");

            bool ok(false);
            TEST_STEP((ok = m_pCtsBrd->setInterfaceOverride(true)), "set DUT interface override");

            if (ok && KEEP_GOING)
            {
                // Loop around LEDs
                for (uint8_t led(kFirstLed); (led <= kLastLed) && KEEP_GOING; led++)
                {
                    m_ledUnderTest = static_cast<dut::CtsBoard::Led>(led);

                    // Turn LED On
                    TEST_STEP(m_pCtsBrd->setInterfaceLed(m_ledUnderTest), QString("turn %1 LED on").arg(m_ledMap[m_ledUnderTest]));

                    // Display a test dialog
                    QString title("Verify Membrane LED");
                    QString msg(QString("Verify that the <b>%1</b> LED is illuminated<br>"
                                        "(and no other LED is illuminated).").arg(m_ledMap[m_ledUnderTest]));

                    gui::ManualTestDialog dialog(title, msg);

                    // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                    TEST_STEP(dialog.exec(), QString("wait for %1 LED confirmation from user").arg(m_ledMap[m_ledUnderTest]));

                    // Get the test state
                    TEST_BOOL(QString("%1 LED").arg(m_ledMap[m_ledUnderTest]), dialog.status(), "");
                }

                // Loop around buttons
                for (uint8_t button(kFirstButton); (button <= kLastButton) && KEEP_GOING; button++)
                {
                    m_buttonUnderTest = static_cast<dut::CtsBoard::Button>(button);
                    QString title("Press Membrane Button");
                    QString msg(QString("Please press and hold the <b>%1</b> button.").arg(m_buttonMap[m_buttonUnderTest]));
                    gui::ActionDialog dialog(title, msg);

                    connect(&dialog, SIGNAL(getTestState()),     this, SLOT(getButtonTestState()));
                    connect(   this, SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                    // Turn LED On
                    dut::CtsBoard::Led led(m_buttonToLedMap[m_buttonUnderTest]);
                    TEST_STEP(m_pCtsBrd->setInterfaceLed(led), QString("turn %1 LED on").arg(m_ledMap[led]));

                    // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                    TEST_STEP(dialog.exec(), QString("wait for %1 button press").arg(m_buttonMap[m_buttonUnderTest]));

                    // Get the test state
                    TEST_BOOL(QString("%1 Button").arg(m_buttonMap[m_buttonUnderTest]), dialog.status(), "");
                }
            }

            TEST_STEP(m_pCtsBrd->setInterfaceOverride(false), "clear DUT interface override");

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

    void TestCaseCtsBoardMembraneInterface::getButtonTestState()
    {
        emit testState(m_pCtsBrd->isButtonPressed(m_buttonUnderTest));
    }
}
