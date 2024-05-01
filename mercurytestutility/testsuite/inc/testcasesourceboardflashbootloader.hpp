#ifndef TESTCASESOURCEBOARDFLASHBOOTLOADER_HPP
#define TESTCASESOURCEBOARDFLASHBOOTLOADER_HPP

#include "testcasesourceboard.hpp"
#include "jlink.hpp"

namespace testsuite
{
    class TestCaseSourceBoardFlashBootloader : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardFlashBootloader(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const QString kBootloaderConfigData; //!< The magic bytes to write into the bootloader to make it work
        static const uint8_t kMaxAttempts = 3;      //!< Maximum attempts to erase/program/verify flash
        static const hardware::Jlink::Device kDevice = hardware::Jlink::Device::CORTEX_M3;
    };

}

#endif // TESTCASESOURCEBOARDFLASHBOOTLOADER_HPP
