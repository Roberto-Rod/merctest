#ifndef TESTCASECTSBOARDDCPOWER_HPP
#define TESTCASECTSBOARDDCPOWER_HPP

#include "testcasectsboard.hpp"

namespace testsuite
{
    class TestCaseCtsBoardDcPower : public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardDcPower(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kShutdownCurrentMin = 0;   //!< Minimum current (mA) when in shutdown mode
        static const uint16_t kShutdownCurrentMax = 3;   //!< Maximum current (mA) when in shutdown mode
        static const uint16_t kStandbyCurrentMin  = 10;  //!< Minimum current (mA) when in standby mode
        static const uint16_t kStandbyCurrentMax  = 30;  //!< Maximum current (mA) when in standby mode
        static const uint16_t kRfOnCurrentMin     = 150; //!< Minimum current (mA) when RF chain is on
        static const uint16_t kRfOnCurrentMax     = 200; //!< Maximum current (mA) when RF chain is on
    };
}

#endif // TESTCASECTSBOARDDCPOWER_HPP

