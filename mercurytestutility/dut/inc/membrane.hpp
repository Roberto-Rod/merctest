#ifndef MEMBRANE_HPP
#define MEMBRANE_HPP

#include "deviceundertest.hpp"

#include <QString>

namespace comms
{
    class RegisterControl;
    class RegisterComms;
}

namespace dut
{
    struct MembraneVariant : public Variant
    {
        MembraneVariant(QString name, QString number, bool syncLedPresent) :
            Variant(name, number),
            m_syncLedPresent(syncLedPresent)
        {
        }

        ~MembraneVariant() {}

        bool m_syncLedPresent;
    };

    class Membrane : public DeviceUnderTest
    {
        Q_OBJECT

    public:
        Membrane(testjig::TestJig* parentJig = 0);

        ~Membrane();

        bool connect();

        void disconnect();

        bool isConnected() const;

        DutState state() const;

        bool waitReady() const;

        bool syncLedPresent() const;
    };
}

#endif // MEMBRANE_HPP
