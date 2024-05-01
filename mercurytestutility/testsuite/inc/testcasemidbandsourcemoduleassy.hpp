#ifndef TESTCASEMIDBANDSOURCEMODULEASSY_HPP
#define TESTCASEMIDBANDSOURCEMODULEASSY_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseMidBandSourceModuleAssy : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseMidBandSourceModuleAssy(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static constexpr double kMinPowerOutMB = 10.0;
        static constexpr double kMinPowerOutHB = -2.3;
        static const uint8_t    kTestPath    = 0;           //!< Doubler RF path
       static const uint64_t   kTestFreqMB    = 400;   //!< Test frequency out (MHz)
         static const uint64_t   kTestFreqHB    = 2400;   //!< Test frequency out (MHz)
        // Note STI boards have a higher gain (2dB),  attenuation reduced for Kasdon boards.
        // static const uint8_t    kTestAttQtDb = 84;          //!< DDS attenuation (quarter dB)
         static const uint8_t    kTestAttQtDb = 73;          //!< DDS attenuation (quarter dB)
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULEASSY_HPP
