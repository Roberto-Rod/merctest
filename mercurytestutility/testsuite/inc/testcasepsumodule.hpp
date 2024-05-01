#ifndef TESTCASEPSUMODULE_HPP
#define TESTCASEPSUMODULE_HPP

#include "testcase.hpp"

namespace testsuite
{
    class TestCasePsuModule : public TestCase
    {
        Q_OBJECT

    public:
        TestCasePsuModule(QObject* parent = 0);

        TestCasePsuModule(QString name, QObject* parent = 0);

        ~TestCasePsuModule() {}

        virtual QString setupMessage();

    protected:
        static const uint32_t kPowerCycleDelay    = 1;    //!< Power cycle delay in seconds
        static const uint32_t kPsuEnPostDelay     = 2;    //!< PSU enable delay before making measurements, seconds
    };
}

#endif // TESTCASEPSUMODULE_HPP
