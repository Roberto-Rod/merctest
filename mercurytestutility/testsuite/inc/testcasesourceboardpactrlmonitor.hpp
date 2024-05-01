#ifndef TESTCASESOURCEBOARDPACTRLMONITOR_HPP
#define TESTCASESOURCEBOARDPACTRLMONITOR_HPP

#include "testcasesourceboard.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseSourceBoardPaCtrlMonitor : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardPaCtrlMonitor(hardware::Port port, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint32_t kPaMonitorExpected = 0x400;   //!< Expected value from emulator on Test Jig - this is delaying the input sequence and feeding it back to master
        static const uint8_t  kPaCtrlExpectedRegisters[];   //!< Expected register values from Test Jig emulator - fixed to 0x12, 0x34, 0x56, 0x78

        hardware::Port m_port;
    };
}

#endif // TESTCASESOURCEBOARDPACTRLMONITOR_HPP
