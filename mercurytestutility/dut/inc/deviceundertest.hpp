#ifndef DEVICEUNDERTEST_HPP
#define DEVICEUNDERTEST_HPP

#include <cstdint>

#include <QObject>
#include <QString>

namespace testsuite
{
    class TestCase;
}

namespace testjig
{
    class TestJig;
    class TestHarness;
}

namespace common
{
    class AssyRevision;
}

namespace dut
{
    enum class RunMethod
    {
        AUTOMATIC_RUN,
        MANUAL_RUN,
        AUTOMATIC_SMART
    };

    enum class DutState
    {
        NO_COMMS,
        NO_POWER,
        INITIALISING,
        READY
    };

    struct Variant
    {
        Variant(QString name, QString number) :
            m_name(name),
            m_number(number)
        {
        }

        virtual ~Variant() {}

        QString nameAndNumber() const { return m_name + " (" + m_number + ")"; }
        QString m_name;
        QString m_number;
    };

    class DeviceUnderTest : public QObject
    {
        Q_OBJECT

    public:
        DeviceUnderTest(testjig::TestJig* parentJig = 0);

        virtual ~DeviceUnderTest();

        virtual bool connect()= 0;

        virtual void disconnect()= 0;

        virtual bool isConnected() const = 0;

        virtual DutState state() const = 0;

        virtual bool waitReady() const = 0;

        virtual QString serialNumber() const;

        virtual bool setSerialNumber(QString ser);

        testsuite::TestCase* topLevelTestCase() const;

        testjig::TestHarness* testHarness() const;

        common::AssyRevision* assyRevision() const;

        QList<Variant*> variants() const;

        const Variant* selectedVariant() const;

        const Variant* genericVariant() const;

        bool isSingleVariant();

        bool setSelectedVariant(int selectedVariant);

    protected:
        QList<Variant*> m_variants;

        QString m_serialNumber;

        testsuite::TestCase* m_pTopLevelTestCase;

        testjig::TestHarness* m_pTestHarness;

        common::AssyRevision* m_pRevision;

    private:
        int m_selectedVariant;
    };
}

#endif // DEVICEUNDERTEST_HPP
