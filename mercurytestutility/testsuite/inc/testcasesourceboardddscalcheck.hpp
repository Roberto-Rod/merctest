#ifndef TESTCASESOURCEBOARDDDSCALCHECK_HPP
#define TESTCASESOURCEBOARDDDSCALCHECK_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    struct ddsLimits
    {
        uint32_t fr;
        uint32_t lo;
        uint32_t hi;
    };

    class TestCaseSourceBoardDdsCalCheck : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDdsCalCheck(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint64_t kTestStartFreq = 20000000;     //!< Start test at 20 MHz
        static const uint64_t kTestStopFreq  = 1500000000;   //!< Stop test at 1500 MHz
        static const uint64_t kTestStepFreq  = 20000000;     //!< Test steps 20 MHz
        static constexpr double kDDSCalMin = 0.0;
        static constexpr double kDDSCalMax = 0.25;

         bool   limitcheck(uint16_t freq, uint16_t val);
         ddsLimits * getLimitPoint(uint16_t freq);

        uint16_t m_flimitLo;
        uint16_t m_flimitHi;
        bool m_hititeAmp = false;
    };  // end of class
}  // end of namespace

#endif // TESTCASESOURCEBOARDDDSCALCHECK_HPP
