#ifndef GPIOTESTER_HPP
#define GPIOTESTER_HPP

#include "gpio.hpp"
#include "tester.hpp"
#include "testcase.hpp"

namespace hardware
{
    class GpioTester : public testsuite::Tester
    {
    public:
        GpioTester(Gpio& jigGpio, Gpio& dutGpio, QObject* parent = 0);
        ~GpioTester() {}

        bool execute(bool& testsPassed, bool asInput = true, bool asOutput = true);

        testsuite::TestStatus status();

    private:
        Gpio& m_jigGpio;
        Gpio& m_dutGpio;
        testsuite::TestStatus m_status;
        uint32_t m_errorMask;

        bool execute(Gpio& srcGpio, Gpio& destGpio, QString testName);

        bool allZeros(Gpio& srcGpio, Gpio& destGpio);
        bool allOnes(Gpio& srcGpio, Gpio& destGpio);
        bool stripe(Gpio& srcGpio, Gpio& destGpio, bool invert);
    };
}

#endif // GPIOTESTER_HPP
