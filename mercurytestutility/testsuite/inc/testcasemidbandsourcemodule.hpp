#ifndef TESTCASEMIDBANDSOURCEMODULE_HPP
#define TESTCASEMIDBANDSOURCEMODULE_HPP

#include "testcase.hpp"

namespace testsuite
{
    class TestCaseMidBandSourceModule : public TestCase
    {
        Q_OBJECT

    public:
        TestCaseMidBandSourceModule(QObject* parent = 0);

        virtual QString setupMessage();

        virtual bool execute(bool manualSteps = false);
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULE_HPP
