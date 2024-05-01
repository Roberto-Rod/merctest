#ifndef TESTCASEMEMBRANELEDS_HPP
#define TESTCASEMEMBRANELEDS_HPP

#include "testcase.hpp"
#include "registergpio.hpp"

#include <QMap>

namespace testsuite
{
    class TestCaseMembraneLeds : public TestCase
    {
        Q_OBJECT
    public:
        TestCaseMembraneLeds(QObject* parent = 0);
        ~TestCaseMembraneLeds();

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        // GPIO register details
        static const uint16_t kAddrOffsetLedGpio    = 0x8004;  //!< Base address of the GPIO control registers
        static const uint8_t kLedGpioWidth          = 8;       //!< LED GPIO Width
        static const uint8_t kLedLsb                = 0;       //!< LSB position of LEDs within register

        // GPIO bit positions
        static const uint8_t kJammingGreenLed       = 0;       //!< Jamming LED (Green)
        static const uint8_t kJammingRedLed         = 1;       //!< Jamming LED (Red)
        static const uint8_t kAlarmGreenLed         = 2;       //!< Alarm LED (Green)
        static const uint8_t kAlarmRedLed           = 3;       //!< Alarm LED (Red)
        static const uint8_t kZeroizeGreenLed       = 4;       //!< Zeroize LED (Green)
        static const uint8_t kZeroizeRedLed         = 5;       //!< Zeroize LED (Red)
        static const uint8_t kSyncLed               = 6;       //!< Sync LED (Green)
        static const uint8_t kPowerLed              = 7;       //!< Power LED (Green)

        bool testLed(uint8_t ledGpioPos, bool manualSteps, bool& testsPassed);    //!< Helper method to test an individual LED

        hardware::RegisterGpio* m_pMembraneIo;                                      //!< Pointer to GPIO unit used to set/get LED/switch state
        QMap<uint8_t, QString> m_ledMap;                                            //!< Map LED GPIO positions to names
    };
}

#endif // TESTCASEMEMBRANELEDS_HPP
