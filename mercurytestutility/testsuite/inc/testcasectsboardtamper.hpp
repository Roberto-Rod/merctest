#ifndef TESTCASECTSBOARDTAMPER_HPP
#define TESTCASECTSBOARDTAMPER_HPP

#include "testcasectsboard.hpp"
#include "ctsboard.hpp"

namespace testsuite
{
    class TestCaseCtsBoardTamper : public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardTamper(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);

    public slots:
        void getTestStateUntampered();
        void getTestStateTampered();

    private:
        static const uint8_t kPowerCycleDelay       = 1;  //!< Time, in seconds, to stay off for during power cycle
        static const uint8_t kPowerCycleDelayTamper = 10; //!< Time, in seconds, to stay off for during anti-tamper power-off test

        dut::CtsBoard::TamperChannel m_channelUnderTest;  //!< The tamper channel being tested
    };
}

#endif // TESTCASECTSBOARDTAMPER_HPP

