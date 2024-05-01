#ifndef TESTCASEDOCKCONTROLLERPROGFPGA_HPP
#define TESTCASEDOCKCONTROLLERPROGFPGA_HPP

#include "testcasedockcontroller.hpp"
#include "port.hpp"

namespace testsuite
{
    class TestCaseDockControllerProgFpga : public TestCaseDockController
    {
        Q_OBJECT

    public:
        TestCaseDockControllerProgFpga(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
    };
}

#endif // TESTCASEDOCKCONTROLLERPROGFPGA_HPP
