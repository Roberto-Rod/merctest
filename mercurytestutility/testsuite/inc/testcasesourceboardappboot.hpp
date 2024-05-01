#ifndef  TESTCASESOURCEBOARDAPPBOOT_HPP
#define TESTCASESOURCEBOARDAPPBOOT_HPP

#include "testcasesourceboard.hpp"
#include "settings.hpp"

namespace testsuite
{
    class TestCaseSourceBoardAppBoot : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardAppBoot(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t kPowerCycleDelay = 1;     //!< Time, in seconds, to stay off for during power cycle
    };
}

#endif // TESTCASESOURCEBOARDAPPBOOT_HPP
