#ifndef TESTCASEMIDBANDSOURCEMODULEPORTSEL_HPP
#define TESTCASEMIDBANDSOURCEMODULEPORTSEL_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseMidBandSourceModulePortSel : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseMidBandSourceModulePortSel(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint64_t kTestFrequencyMB = 500;   // 500 MHz
        static const uint64_t kTestFrequencyHB = 2400;  // 2400 MHZ
        static const double kTestPower;
        static const double kPowerOutMinMB;
        static const double kPowerOutMinHB;
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULEPORTSEL_HPP
