#ifndef TESTCASECTSBOARDFLASHFIRMWARE_HPP
#define TESTCASECTSBOARDFLASHFIRMWARE_HPP

#include "testcasectsboard.hpp"
#include "jlink.hpp"

namespace testsuite
{
    class TestCaseCtsBoardFlashFirmware: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardFlashFirmware(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t kPowerCycleOffDelay = 1;  //!< Time, in seconds, to stay off for during power cycle
        static const uint16_t kPowerCycleOnDelay = 500; //!< Time, in milliseconds, to wait for after power cycle
        static const hardware::Jlink::Device kDevice = hardware::Jlink::Device::CORTEX_M4;
    };
}

#endif // TESTCASECTSBOARDFLASHFIRMWARE_HPP
