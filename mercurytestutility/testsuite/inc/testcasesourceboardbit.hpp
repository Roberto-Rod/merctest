#ifndef TESTCASESOURCEBOARDBIT_HPP
#define TESTCASESOURCEBOARDBIT_HPP

#include "testcasesourceboard.hpp"
#include "sourceboardbit.hpp"

namespace testsuite
{
    class TestCaseSourceBoardBit : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardBit(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const dut::SourceBoardBit kBitLimitMin;      //!< Minimum pass thresholds for BIT test results
        static const dut::SourceBoardBit kBitLimitMax;      //!< Maximum pass thresholds for BIT test results
    };
}

#endif // TESTCASESOURCEBOARDBIT_HPP
