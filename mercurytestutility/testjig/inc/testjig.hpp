#ifndef TESTJIG_HPP
#define TESTJIG_HPP

#include <QObject>
#include <QList>

namespace dut
{
    class DeviceUnderTest;
}

namespace comms
{
    class RegisterComms;
}

namespace testjig
{
    class TestJig : public QObject
    {
        Q_OBJECT

    public:
        TestJig(QObject* parent = 0);

        virtual ~TestJig() {}

        virtual QString name() const = 0;

        virtual bool connect()= 0;

        virtual void disconnect()= 0;

        virtual bool isConnected() const = 0;

        QList<dut::DeviceUnderTest*>* duts();

        comms::RegisterComms* registerComms();

    signals:
        void statusUpdated();

        void commsClosed();

    protected:
        QList<dut::DeviceUnderTest*> m_duts;

        comms::RegisterComms* m_pRegComms;
    };
}
#endif // TESTJIG_HPP
