#ifndef TESTCASESOURCEBOARD_HPP
#define TESTCASESOURCEBOARD_HPP

#include "testcase.hpp"
#include "deviceundertest.hpp"

namespace dut
{
    class SourceBoard;
}

namespace testsuite
{
    class TestCaseSourceBoard : public TestCase
    {
        Q_OBJECT

    public:
        TestCaseSourceBoard(QObject* parent = 0);

        TestCaseSourceBoard(QString name, QObject* parent = 0);

        ~TestCaseSourceBoard();

        virtual QString setupMessage();

        virtual void setRunMode(dut::RunMethod runMethod);

        virtual dut::RunMethod getRunMode();

        virtual void setSkipTestFlag(bool skipTest);

        virtual bool getSkipTestFlag();

        virtual bool execute(bool manualSteps = false);

    protected:
        bool initialiseSrcBrd();

        dut::SourceBoard* m_pSrcBrd;

    private:
        static const uint16_t kAddrOffsetPaPortA  = 0x0003;
        static const uint16_t kAddrOffsetPaPortB  = 0x0005;
        static const QString kFpgaTestImage;
        dut::RunMethod m_runMethod;
        bool m_skipTest;
    };
}

#endif // TESTCASESOURCEBOARD_HPP
