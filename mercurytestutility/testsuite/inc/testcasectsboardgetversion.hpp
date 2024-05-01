#ifndef TESTCASECTSBOARDGETVERSION_HPP
#define TESTCASECTSBOARDGETVERSION_HPP

#include "testcasectsboard.hpp"

namespace testsuite
{
    class TestCaseCtsBoardGetVersion: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardGetVersion(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();
    };
}

#endif // TESTCASECTSBOARDGETVERSION_HPP

