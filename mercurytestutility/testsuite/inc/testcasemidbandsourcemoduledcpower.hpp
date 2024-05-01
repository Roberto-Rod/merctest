#ifndef TESTCASEMIDBANDSOURCEMODULEDCPOWER_HPP
#define TESTCASEMIDBANDSOURCEMODULEDCPOWER_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseMidBandSourceModuleDcPower : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseMidBandSourceModuleDcPower(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kInitialisedCurrentMin = 80;       //!< Minimum current (mA) after initialisation
        static const uint16_t kInitialisedCurrentMax = 120;      //!< Maximum current (mA) after initialisation
        static const int32_t  kDeltaCurrentMinMB       = 60;       //!< Minimum delta current (daughter board 5V5 enabled vs. disabled)
        static const int32_t  kDeltaCurrentMaxMB      = 90;       //!< Minimum delta current (daughter board 5V5 enabled vs. disabled)
        static const int32_t  kDeltaCurrentMinHB       = 75;       //!< Minimum delta current (daughter board 5V5 enabled vs. disabled)
        static const int32_t  kDeltaCurrentMaxHB      = 105;       //!< Minimum delta current (daughter board 5V5 enabled vs. disabled)
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULEDCPOWER_HPP
