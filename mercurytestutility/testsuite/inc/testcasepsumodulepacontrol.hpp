#ifndef TESTCASEPSUMODULEPACONTROL_HPP
#define TESTCASEPSUMODULEPACONTROL_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModulePaControl : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModulePaControl(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
    };
}

#endif // TESTCASEPSUMODULEPACONTROL_HPP
