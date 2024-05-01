#ifndef TESTCASESOURCEBOARDFLASHAPP_HPP
#define TESTCASESOURCEBOARDFLASHAPP_HPP

#include "testcasesourceboard.hpp"
#include "settings.hpp"
#include "jlink.hpp"

namespace testsuite
{
    class TestCaseSourceBoardFlashApp : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardFlashApp(common::FirmwareImage img, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t kPowerCycleOffDelay = 1; //!< Time, in seconds, to stay off for during power cycle
        static const uint16_t kPowerCycleOnDelay = 500; //!< Time, in milliseconds, to wait for after power cycle
        static const hardware::Jlink::Device kDevice = hardware::Jlink::Device::CORTEX_M3;

        common::FirmwareImage m_firmwareImage;
    };
}

#endif // TESTCASESOURCEBOARDFLASHAPP_HPP
