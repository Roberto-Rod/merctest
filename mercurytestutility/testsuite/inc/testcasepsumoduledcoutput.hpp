#ifndef TESTCASEPSUMODULEDCOUTPUT_HPP
#define TESTCASEPSUMODULEDCOUTPUT_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleDcOutput : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleDcOutput(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kDisabledCurrentInMin  = 0;     //!< Input current, disabled state, milliamps, min
        static const uint16_t kDisabledCurrentInMax  = 100;   //!< Input current, disabled state, milliamps, max
        static const uint16_t kDisabledVoltageOutMin = 0;     //!< Output voltage, disabled state, millivolts, min
        static const uint16_t kDisabledVoltageOutMax = 100;   //!< Output voltage, disabled state, millivolts, max

        static const uint16_t kEnabledCurrentInMin   = 1590;  //!< Input current, enabled state, milliamps, min
        static const uint16_t kEnabledCurrentInMax   = 2300;  //!< Input current, enabled state, milliamps, max
        static const uint16_t kEnabledCurrentOutMin  = 1300;  //!< Output current, enabled state, milliamps, min
        static const uint16_t kEnabledCurrentOutMax  = 1430;  //!< Output current, enabled state, milliamps, max
        static const uint16_t kEnabledVoltageOutMin  = 27300; //!< Output voltage, enabled state, millivolts, min
        static const uint16_t kEnabledVoltageOutMax  = 28700; //!< Output voltage, enabled state, millivolts, max
    };
}

#endif // TESTCASEPSUMODULEDCOUTPUT_HPP
