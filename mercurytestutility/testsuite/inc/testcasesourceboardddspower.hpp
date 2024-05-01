#ifndef TESTCASESOURCEBOARDDDSPOWER_HPP
#define TESTCASESOURCEBOARDDDSPOWER_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    struct DdsPowerSpec
    {
        uint64_t freq;  //!< Frequency in Hz
        double pwr;     //!< Minimum power in dBm
    };

    class TestCaseSourceBoardDdsPower : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDdsPower(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

        static bool powerOk(uint64_t freq, double power, double& max_power);

    private:
        static const uint64_t kTestStartFreq = 20000000;     //!< Start test at 20 MHz
        static const uint64_t kTestStopFreq  = 1500000000;   //!< Stop test at 1500 MHz
        static const uint64_t kTestStepFreq  = 148000000;    //!< Test steps 148 MHz

        static const DdsPowerSpec kDdsPowerSpecTable[];
    };
}

#endif // TESTCASESOURCEBOARDDDSPOWER_HPP
