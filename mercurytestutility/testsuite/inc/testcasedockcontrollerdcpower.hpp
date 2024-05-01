#ifndef TESTCASEDOCKCONTROLLERDCPOWER_HPP
#define TESTCASEDOCKCONTROLLERDCPOWER_HPP

#include "testcasedockcontroller.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseDockControllerDcPower : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerDcPower(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kInputCurrentMin = 25;      //!< Minimum current (mA) after initialisation
        static const uint16_t kInputCurrentMax = 45;      //!< Maximum current (mA) after initialisation
    };
}

#endif // TESTCASEDOCKCONTROLLERDCPOWER_HPP
