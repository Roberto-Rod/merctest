#ifndef TESTCASEPSUMODULECONTROLPORT_HPP
#define TESTCASEPSUMODULECONTROLPORT_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleControlPort : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleControlPort(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
    };
}

#endif // TESTCASEPSUMODULECONTROLPORT_HPP
