#ifndef TESTCASESOURCEBOARDDDSDCPOWER_HPP
#define TESTCASESOURCEBOARDDDSDCPOWER_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardDdsCurrent : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDdsCurrent(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint64_t kTestFreq   = 1000000000;  //!< Run test at 1000 MHz
        static const uint16_t kCurrentMin = 100;         //!< Minimum current (mA) when DDS is active
        static const uint16_t kCurrentMax = 300;         //!< Maximum current (mA) when DDS is active
    };
}

#endif // TESTCASESOURCEBOARDDDSDCPOWER_HPP
