#ifndef TESTCASEMIDBANDSOURCEMODULEATT_HPP
#define TESTCASEMIDBANDSOURCEMODULEATT_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseMidBandSourceModuleAtt : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseMidBandSourceModuleAtt(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        struct AttError
        {
            double rangeMaxAtt;
            double maxError;
        };

        static const AttError kAttErrorPoints[];
        static const AttError kAttErrorPointsHB[];
        static const AttError kAttErrorPointsHB5G8[];

        static const uint64_t kTestFreqMB     = 400; //!< Test frequency out (MHz)
        //static const uint64_t kTestFreqHB     = 1500; //!< Test frequency out (MHz)
        static const uint64_t kTestFreqHBfirstLoop    = 2400; //!< Test frequency out (MHz)
        static const uint64_t kTestFreqHBsecondLoop    = 5800; //!< Test frequency out (MHz)
        static constexpr double kTestPower  = 0.0;       //!< Signal generator test power (dBm)

        static bool attOk(uint8_t attSetting, double attMeasured, bool isHighBand, bool isSecondLoop);
        bool  outputLUT();
         bool  outputAttPoint(uint16_t att);
         bool  setAttLUT();
         uint64_t getTestFreq(bool isMidband, bool  isFirstloop);
         uint16_t  getTestPath(bool isMidband, bool  isFirstloop);
    };
}

#endif // TESTCASEMIDBANDSOURCEMODULEATT_HPP
