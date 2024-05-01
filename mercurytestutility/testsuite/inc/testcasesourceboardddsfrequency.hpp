#ifndef TESTCASESOURCEBOARDDDSFREQUENCY_HPP
#define TESTCASESOURCEBOARDDDSFREQUENCY_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardDdsFrequency : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDdsFrequency(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint64_t kTestFreq        = 1500000000; //!< Test at 1500 MHz (always test at whole MHz to allow ppm calc to work)
        static const uint64_t kSpecAnSpan      = 10000;      //!< Spectrum analyser span 10 kHz
        static const uint64_t kSpecAnRbw       = 10;         //!< Spectrum analyser resolution bandwidth 10 Hz
        static const uint8_t  kMaxFreqErrorPpm = 3;          //!< Max measured frequency error: +/-3ppm
        static const int32_t  kNumberTestReps  = 20;          //!< Number of times to measure frequency to find min/max error
        static constexpr  double  KMinFreqPower = -4.0;   // Was -12dBm (before Rf path correction)
        static constexpr  double   KMaxFreqPower = 6.0;   // Was 0dBm (before Rf path correction)
    };
}

#endif // TESTCASESOURCEBOARDDDSFREQUENCY_HPP
