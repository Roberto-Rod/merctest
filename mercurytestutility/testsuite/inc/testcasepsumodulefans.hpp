#ifndef TESTCASEPSUMODULEFANS_HPP
#define TESTCASEPSUMODULEFANS_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleFans : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleFans(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t k12VRailMin       = 11500; //!< 12V rail voltage (12.1V nom.), millivolts, min
        static const uint16_t k12VRailMax       = 12700; //!< 12V rail voltage (12.1V nom.), millivolts, max
        static const uint16_t kPwmLowMin        = 18;    //!< Fan PWM low time, microseconds, min
        static const uint16_t kPwmLowMax        = 22;    //!< Fan PWM low time, microseconds, max
        static const uint16_t kPwmHighMin       = 18;    //!< Fan PWM high time, microseconds, min
        static const uint16_t kPwmHighMax       = 22;    //!< Fan PWM high time, microseconds, max
        static const uint8_t  kTachoReadingMin  = 63;    //!< Fan tacho reading, min
        static const uint8_t  kTachoReadingMax  = 77;    //!< Fan tacho reading, max
    };
}

#endif // TESTCASEPSUMODULEFANS_HPP
