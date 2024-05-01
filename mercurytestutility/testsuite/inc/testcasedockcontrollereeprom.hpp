#ifndef TESTCASEDOCKCONTROLLEREEPROM_HPP
#define TESTCASEDOCKCONTROLLEREEPROM_HPP

#include "testcasedockcontroller.hpp"

namespace testsuite
{
    class TestCaseDockControllerEeprom : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerEeprom(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint32_t kExpectedMfrDevId = 0x2941;
    };
}

#endif // TESTCASEDOCKCONTROLLEREEPROM_HPP
