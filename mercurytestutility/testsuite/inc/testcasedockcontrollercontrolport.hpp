#ifndef TESTCASEDOCKCONTROLLERCONTROLPORT_HPP
#define TESTCASEDOCKCONTROLLERCONTROLPORT_HPP

#include "testcasedockcontroller.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseDockControllerControlPort : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerControlPort(hardware::Port port, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kExpectedDwgNum = 21;        //!< Dock Controller FPGA drawing number
        hardware::Port m_port;
    };
}

#endif // TESTCASEDOCKCONTROLLERCONTROLPORT_HPP
