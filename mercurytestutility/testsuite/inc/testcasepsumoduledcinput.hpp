#ifndef TESTCASEPSUMODULEDCINPUT_HPP
#define TESTCASEPSUMODULEDCINPUT_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleDcInput : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleDcInput(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kDisabledCurrentMin = 0;    //!< Current in disabled state, min
        static const uint16_t kDisabledCurrentMax = 100;  //!< Current in disabled state, max
    };
}

#endif // TESTCASEPSUMODULEDCINPUT_HPP
