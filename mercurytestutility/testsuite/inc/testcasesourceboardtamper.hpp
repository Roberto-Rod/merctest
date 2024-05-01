#ifndef TESTCASESOURCEBOARDTAMPER_HPP
#define TESTCASESOURCEBOARDTAMPER_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardTamper : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardTamper(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);

    public slots:
        void getTestStateUnilluminated();
        void getTestStateIlluminated();

    private:
        static const uint8_t kPowerCycleDelay       = 1;     //!< Time, in seconds, to stay off for during power cycle
        static const uint8_t kPowerCycleDelayTamper = 10;    //!< Time, in seconds, to stay off for during anti-tamper power-off test
    };
}

#endif // TESTCASESOURCEBOARDTAMPER_HPP
