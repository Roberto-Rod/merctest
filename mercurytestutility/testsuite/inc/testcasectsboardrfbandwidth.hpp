#ifndef TESTCASECTSBOARDRFBANDWIDTH_HPP
#define TESTCASECTSBOARDRFBANDWIDTH_HPP

#include "testcasectsboard.hpp"
#include "ctstestband.hpp"
#include "ctsboard.hpp"

#include <QList>

namespace testsuite
{
    /** @brief This is a base class for any other classes which perform an RF bandwidth test.
     * This class is constructed with an empty list of test bands, child classes just need to
     * populate m_TestBands in their constructor and then use the execute function defined in
     * this base class.
     */
    class TestCaseCtsBoardRfBandwidth : public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardRfBandwidth(QString name, QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    protected:
        static const dut::CtsBoard::IfPath kIf900 = dut::CtsBoard::IfPath::If900;
        static const dut::CtsBoard::IfPath kIf1900 = dut::CtsBoard::IfPath::If1900;
        static const dut::CtsBoard::IfPath kIfNz = dut::CtsBoard::IfPath::IfNearZero;

        QList<CtsTestBand> m_testBands;
    };
}

#endif // TESTCASECTSBOARDRFBANDWIDTH_HPP

