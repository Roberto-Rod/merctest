#ifndef TESTCASEPSUMODULEBIT_HPP
#define TESTCASEPSUMODULEBIT_HPP

#include "testcasepsumodule.hpp"

namespace testsuite
{
    class TestCasePsuModuleBit : public TestCasePsuModule
    {
        Q_OBJECT

    public:
        TestCasePsuModuleBit(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kVoltageInMin          = 22800; //!< Input voltage, all states, millivolts, min
        static const uint16_t kVoltageInMax          = 25200; //!< Input voltage, all states, millivolts, max
        static const uint16_t k3V3RailMin            = 3150;  //!< 3V3 rail voltage (3.32V nom.), all states, millivolts, min
        static const uint16_t k3V3RailMax            = 3490;  //!< 3V3 rail voltage (3.32V nom.), all states, millivolts, max

        static const uint16_t kDisabledCurrentOutMin = 0;     //!< Output current, disabled state, milliamps, min
        static const uint16_t kDisabledCurrentOutMax = 100;   //!< Output current, disabled state, milliamps, max
        static const uint16_t kDisabledVoltageOutMin = 0;     //!< Output voltage, disabled state, millivolts, min
        static const uint16_t kDisabledVoltageOutMax = 300;   //!< Output voltage, disabled state, millivolts, max
        static const uint16_t kDisabled12VRailMin    = 0;     //!< 12V rail voltage, disabled state, millivolts, min
        static const uint16_t kDisabled12VRailMax    = 300;   //!< 12V rail voltage, disabled state, millivolts, max

        static const uint16_t kEnabledCurrentOutMin  = 1300;  //!< Output current, enabled state, milliamps, min
        static const uint16_t kEnabledCurrentOutMax  = 1470;  //!< Output current, enabled state, milliamps, max
        static const uint16_t kEnabledVoltageOutMin  = 26600; //!< Output voltage, enabled state, millivolts, min
        static const uint16_t kEnabledVoltageOutMax  = 29400; //!< Output voltage, enabled state, millivolts, max
        static const uint16_t kEnabled12VRailMin     = 11500; //!< 12V rail voltage (12.1V nom.), enabled state, millivolts, min
        static const uint16_t kEnabled12VRailMax     = 12700; //!< 12V rail voltage (12.1V nom.), enabled state, millivolts, max
    };
}

#endif // TESTCASEPSUMODULEBIT_HPP
