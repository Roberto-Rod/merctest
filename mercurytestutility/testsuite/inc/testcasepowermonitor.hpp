#ifndef TESTCASEPOWERMONITOR_HPP
#define TESTCASEPOWERMONITOR_HPP

#include "testcase.hpp"

namespace testsuite
{
    class TestCasePowerMonitor : public TestCase
    {
        Q_OBJECT

    public:
        TestCasePowerMonitor(QObject* parent = 0);

        TestCasePowerMonitor(QString name, QObject* parent = 0);

        ~TestCasePowerMonitor() {}

        virtual QString setupMessage();
    };
}

#endif // TESTCASEPOWERMONITOR_HPP
