#ifndef TESTCASESOURCEBOARDFILELOADER_HPP
#define TESTCASESOURCEBOARDFILELOADER_HPP

#include "testcasesourceboard.hpp"
#include "settings.hpp"

namespace testsuite
{
    class TestCaseSourceBoardFileLoader : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardFileLoader(common::FirmwareImage img, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t kPowerCycleDelay = 1;     //!< Time, in seconds, to stay off for during power cycle

        common::FirmwareImage m_firmwareImage;
    };
}

#endif // TESTCASESOURCEBOARDFPGALOADER_HPP
