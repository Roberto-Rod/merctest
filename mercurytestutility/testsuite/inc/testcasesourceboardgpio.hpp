#ifndef TESTCASESOURCEBOARDGPIO_HPP
#define TESTCASESOURCEBOARDGPIO_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardGpio : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
        TestCaseSourceBoardGpio(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const uint16_t kDutGpioBaseAddr = 0x0002;    //!< Address of the GPIO register in the DUT (Source Board)
        static const uint16_t kJigGpioBaseAddr = 0x4006;    //!< Address of the GPIO register in the Test Jig
        static const uint8_t  kGpioWidth       = 8;         //!< Width of the GPIO register (DUT & Jig)
    };
}

#endif // TESTCASESOURCEBOARDGPIO_HPP
