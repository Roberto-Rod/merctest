#ifndef TESTCASEPSUMODULEPROGMICRO_HPP
#define TESTCASEPSUMODULEPROGMICRO_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleProgMicro : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleProgMicro(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
    };
}

#endif // TESTCASEPSUMODULEPROGMICRO_HPP
