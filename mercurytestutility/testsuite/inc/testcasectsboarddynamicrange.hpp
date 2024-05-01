#ifndef TESTCASECTSBOARDDYNAMICRANGE_HPP
#define TESTCASECTSBOARDDYNAMICRANGE_HPP

#include "testcasectsboard.hpp"

#include <QList>

namespace testsuite
{
    struct CtsPowerSpec
    {
        double m_inputPower;
        uint16_t m_adcMin_mV;
        uint16_t m_adcMax_mV;
    };

    struct CtsDynamicRangePoint
    {
        uint8_t m_rfPath;
        uint32_t m_frequency_kHz;
        QList<CtsPowerSpec> m_powerSpec;
    };

    class TestCaseCtsBoardDynamicRange : public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardDynamicRange(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const QList<CtsDynamicRangePoint> kTestPoints;
    };
}

#endif // TESTCASECTSBOARDDYNAMICRANGE_HPP

