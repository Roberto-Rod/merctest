#ifndef TESTCASESOURCEBOARDDOCKCTRL_HPP
#define TESTCASESOURCEBOARDDOCKCTRL_HPP

#include "testcasesourceboard.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseSourceBoardDockCtrl : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardDockCtrl(hardware::Port port, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint32_t kDockRegisterExpected = 0x12345678;   //!< The test data that the test jig returns to the Source Board
        hardware::Port m_port;
    };
}

#endif // TESTCASESOURCEBOARDDOCKCTRL_HPP
