#ifndef TESTCASESOURCEBOARDDCPOWER_HPP
#define TESTCASESOURCEBOARDDCPOWER_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardDcPower : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDcPower(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kShutdownCurrentMin    = 0;        //!< Minimum current (mA) when in shutdown mode
        static const uint16_t kShutdownCurrentMax    = 10;       //!< Maximum current (mA) when in shutdown mode
        static const uint16_t kPowerupCurrentMin     = 40;       //!< Minimum current (mA) immediately after power-up
        static const uint16_t kPowerupCurrentMax     = 95;       //!< Maximum current (mA) immediately after power-up  //!!!!!!!!!
        static const uint16_t kInitialisedCurrentMin = 70;       //!< Minimum current (mA) after initialisation
        static const uint16_t kInitialisedCurrentMax = 120;      //!< Maximum current (mA) after initialisation
    };
}

#endif // TESTCASESOURCEBOARDDCPOWER_HPP
