#ifndef TESTCASEDOCKCONTROLLERPACTRLMONITOR_HPP
#define TESTCASEDOCKCONTROLLERPACTRLMONITOR_HPP

#include "testcasedockcontroller.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseDockControllerPaCtrlMonitor : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerPaCtrlMonitor(hardware::Port port, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint32_t kPaMonitorExpected = 0x400;   //!< Expected value from emulator on Test Jig - this is delaying the input sequence and feeding it back to master
        static const uint8_t  kPaCtrlExpectedRegisters[];   //!< Expected register values from Test Jig emulator - fixed to 0x12, 0x34, 0x56, 0x78

        hardware::Port m_port;
    };
}

#endif // TESTCASEDOCKCONTROLLERPACTRLMONITOR_HPP
