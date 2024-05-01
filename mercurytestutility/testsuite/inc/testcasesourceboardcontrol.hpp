#ifndef TESTCASESOURCEBOARDCONTROL_HPP
#define TESTCASESOURCEBOARDCONTROL_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardControl : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardControl(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint32_t kClockCountPpsMin = 79992000;
        static const uint32_t kClockCountPpsMax = 80008000;
    };
}

#endif // TESTCASESOURCEBOARDCONTROL_HPP
