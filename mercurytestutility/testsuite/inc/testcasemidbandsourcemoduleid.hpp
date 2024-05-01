#ifndef TESTCASEMIDBANDSOURCEMODULEDAUGHTERID_HPP
#define TESTCASEMIDBANDSOURCEMODULEDAUGHTERID_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseMidBandSourceModuleId : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseMidBandSourceModuleId(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t kExpectedIdMB = 0x00;    //!< Expected daughter ID for Mid-Band Source Module
        static const uint8_t kExpectedIdHB = 0x01;    //!< Expected daughter ID for High-Band Source Module
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULEDAUGHTERID_HPP
