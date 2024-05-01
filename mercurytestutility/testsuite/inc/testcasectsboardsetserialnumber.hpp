#ifndef TESTCASECTSSETSERIALNUMBER_HPP
#define TESTCASECTSSETSERIALNUMBER_HPP

#include "testcasectsboard.hpp"

namespace testsuite
{
    class TestCaseCtsBoardSetSerialNumber: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardSetSerialNumber(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();
    };
}

#endif // TESTCASECTSSETSERIALNUMBER_HPP

