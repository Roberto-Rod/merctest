#ifndef TESTCASEDOCKCONTROLLER_HPP
#define TESTCASEDOCKCONTROLLER_HPP

#include "testcase.hpp"

namespace testsuite
{
    class TestCaseDockController : public TestCase
    {
        Q_OBJECT

    public:
        TestCaseDockController(QObject* parent = 0);

        TestCaseDockController(QString name, QObject* parent = 0);

        ~TestCaseDockController() {}

        virtual QString setupMessage();
    };
}

#endif // TESTCASEDOCKCONTROLLER_HPP
