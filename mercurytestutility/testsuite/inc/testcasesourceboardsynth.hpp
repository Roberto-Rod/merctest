#ifndef TESTCASESOURCEBOARDSYNTH_HPP
#define TESTCASESOURCEBOARDSYNTH_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardSynth : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardSynth(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint32_t kSyncClkCountMaxErr = 2; //!< Max allowed sync clock count error
    };
}

#endif // TESTCASESOURCEBOARDSYNTH_HPP
