#ifndef TESTCASESOURCEBOARDDDSCALIBRATION_HPP
#define TESTCASESOURCEBOARDDDSCALIBRATION_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardDdsCalibration : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDdsCalibration(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint64_t kTestStartFreq = 20000000;     //!< Start test at 20 MHz
        static const uint64_t kTestStopFreq  = 1500000000;   //!< Stop test at 1500 MHz
        static const uint64_t kTestStepFreq  = 20000000;     //!< Test steps 20 MHz
    };
}

#endif // TESTCASESOURCEBOARDDDSCALIBRATION_HPP
