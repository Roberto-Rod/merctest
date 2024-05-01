#ifndef TESTCASECTSBOARD_HPP
#define TESTCASECTSBOARD_HPP

#include "testcase.hpp"
#include "deviceundertest.hpp"

namespace dut
{
    class CtsBoard;
}

namespace testsuite
{
    class TestCaseCtsBoard : public TestCase
    {
        Q_OBJECT

    public:
        TestCaseCtsBoard(QObject* parent = 0);

        TestCaseCtsBoard(QString name, QObject* parent = 0);

        ~TestCaseCtsBoard();

        virtual QString setupMessage();

        virtual void setRunMode(dut::RunMethod runMethod);

        virtual dut::RunMethod getRunMode();

        virtual void setSkipTestFlag(bool skipTest);

        virtual bool getSkipTestFlag();

        virtual bool execute(bool manualSteps = false);

    protected:
        bool initialiseCtsBrd();

        dut::CtsBoard* m_pCtsBrd;

    private:
        dut::RunMethod m_runMethod;
        bool m_skipTest;
    };
}

#endif // TESTCASECTSBOARD_HPP
