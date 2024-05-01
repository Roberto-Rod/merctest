#ifndef TESTCASECTSBOARDMEMBRANEINTERFACE_HPP
#define TESTCASECTSBOARDMEMBRANEINTERFACE_HPP

#include "testcasectsboard.hpp"
#include "ctsboard.hpp"

#include <QMap>

namespace testsuite
{
    class TestCaseCtsBoardMembraneInterface: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardMembraneInterface(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);

    public slots:
        void getButtonTestState();

    private:
        static const uint8_t kFirstButton = static_cast<uint8_t>(dut::CtsBoard::Button::Power);
        static const uint8_t kLastButton = static_cast<uint8_t>(dut::CtsBoard::Button::Cross);
        static const uint8_t kFirstLed = static_cast<uint8_t>(dut::CtsBoard::Led::Power);
        static const uint8_t kLastLed = static_cast<uint8_t>(dut::CtsBoard::Led::CrossGreen);

        QMap<dut::CtsBoard::Button, QString> m_buttonMap;
        QMap<dut::CtsBoard::Led, QString> m_ledMap;
        QMap<dut::CtsBoard::Button, dut::CtsBoard::Led> m_buttonToLedMap;
        dut::CtsBoard::Button m_buttonUnderTest;
        dut::CtsBoard::Led m_ledUnderTest;
    };
}

#endif // TESTCASECTSBOARDMEMBRANE_HPP

