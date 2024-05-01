#ifndef TESTCASESOURCEBOARDDAUGHTER_HPP
#define TESTCASESOURCEBOARDDAUGHTER_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardDaughter : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDaughter(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint8_t kExpectedId = 0xF;    //!< Expected daughter ID with no daughter board connected
    };
}

#endif // TESTCASESOURCEBOARDDAUGHTER_HPP
