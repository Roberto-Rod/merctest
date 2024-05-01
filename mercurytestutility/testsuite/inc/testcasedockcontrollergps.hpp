#ifndef TESTCASEDOCKCONTROLLERGPS_HPP
#define TESTCASEDOCKCONTROLLERGPS_HPP

#include "testcasedockcontroller.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseDockControllerGps : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerGps(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);

    public slots:
        void getTestState();

    private:
        static const uint32_t kClkCountPpsMin = 19998000;
        static const uint32_t kClkCountPpsMax = 20002000;
    };
}

#endif // TESTCASEDOCKCONTROLLERGPS_HPP
