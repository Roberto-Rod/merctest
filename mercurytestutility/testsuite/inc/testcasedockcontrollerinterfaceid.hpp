#ifndef TESTCASEDOCKCONTROLLERINTERFACEID_HPP
#define TESTCASEDOCKCONTROLLERINTERFACEID_HPP

#include "testcasedockcontroller.hpp"

namespace testsuite
{
    class TestCaseDockControllerInterfaceId : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerInterfaceId(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t  kExpectedIfId = 0x0;      //!< Expected dock interface ID
    };
}

#endif // TESTCASEDOCKCONTROLLERINTERFACEID_HPP
