#ifndef TESTCASECTSBOARDUSBSERIALPORT_HPP
#define TESTCASECTSBOARDUSBSERIALPORT_HPP

#include "testcasectsboard.hpp"

#include <QByteArray>

namespace testsuite
{
    class TestCaseCtsBoardUsbSerialPort: public TestCaseCtsBoard
    {
        Q_OBJECT

    public:
        TestCaseCtsBoardUsbSerialPort(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    signals:
        void testState(bool state);

    public slots:
        void findUsbPortName();

    private:
        static const uint32_t kUsbCommsTimeout_ms = 5000;
        static const uint32_t kConnectWait_ms = 2500;
        static const uint16_t kMaxReadBytes = 32;
        static const QByteArray kSendBytes;
        static const QByteArray kExpectBytes;

        QStringList m_portNamesStart;
        QString m_usbPortName;

        void getPortNames(QStringList& portNames) const;
    };
}

#endif // TESTCASECTSBOARDUSBSERIALPORT_HPP

