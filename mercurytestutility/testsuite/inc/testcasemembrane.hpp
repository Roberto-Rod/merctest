#ifndef TESTCASEMEMBRANE_HPP
#define TESTCASEMEMBRANE_HPP

#include "testcase.hpp"

namespace testsuite
{
    class TestCaseMembrane : public TestCase
    {
        Q_OBJECT

    public:
        TestCaseMembrane(QObject* parent = 0);
        ~TestCaseMembrane();

        virtual QString setupMessage();
    };
}

#endif // TESTCASEMEMBRANE_HPP
