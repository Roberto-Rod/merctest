#ifndef TESTCASEMEMBRANEBUTTONS_HPP
#define TESTCASEMEMBRANEBUTTONS_HPP

#include "testcase.hpp"
#include "registergpio.hpp"

#include <QMap>

namespace testsuite
{
    class TestCaseMembraneButtons : public TestCase
    {
        Q_OBJECT
    public:
        TestCaseMembraneButtons(QObject* parent = 0);
        ~TestCaseMembraneButtons();

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);

    public slots:
        void getTestState();

    private:
        // GPIO register details
        static const uint16_t kAddrOffsetLedGpio    = 0x8004;  //!< Base address of the GPIO control registers
        static const uint8_t kLedGpioWidth          = 8;       //!< Membrane GPIO Width
        static const uint8_t kLedLsb                = 0;       //!< LSB position of LEDs within register

        static const uint16_t kRegAddrButtonState   = 0x8006;  //!< Address of button state register
        static const uint8_t kButtonWidth           = 6;       //!< Width of button portion of register

        // GPIO bit positions
        static const uint8_t kJammingGreenLed       = 0;       //!< Jamming LED (Green)
        static const uint8_t kJammingRedLed         = 1;       //!< Jamming LED (Red)
        static const uint8_t kAlarmGreenLed         = 2;       //!< Alarm LED (Green)
        static const uint8_t kAlarmRedLed           = 3;       //!< Alarm LED (Red)
        static const uint8_t kZeroizeGreenLed       = 4;       //!< Zeroize LED (Green)
        static const uint8_t kZeroizeRedLed         = 5;       //!< Zeroize LED (Red)
        static const uint8_t kSyncLed               = 6;       //!< Sync LED (Green)
        static const uint8_t kPowerLed              = 7;       //!< Power LED (Green)

        static const uint8_t kJammingButton         = 0;       //!< Jamming Push Button
        static const uint8_t kAlarmButton           = 1;       //!< Alarm Push Button
        static const uint8_t kZeroizeButton         = 2;       //!< Zeroize Push Button
        static const uint8_t kPowerButton           = 5;       //!< Power Push Button

        uint8_t m_buttonUnderTest;

        QMap<uint8_t, QString> m_buttonMap;                                            //!< Map button GPIO positions to names

        bool testButton(uint8_t buttonGpioPos, bool manualSteps, bool& testsPassed); //!< Helper method to test an individual push button

        hardware::RegisterGpio* m_pLedGpio;                                            //!< Pointer to GPIO unit used to set/get LED/switch state
        hardware::RegisterGpio* m_pButtonState;                                        //!< Pointer to GPIO unit used to set/get LED/switch state
    };
}

#endif // TESTCASEMEMBRANEBUTTONS_HPP
