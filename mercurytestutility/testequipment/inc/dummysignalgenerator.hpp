#ifndef DUMMYSIGNALGENERATOR_HPP
#define DUMMYSIGNALGENERATOR_HPP

#include "signalgenerator.hpp"
#include "testequipment.hpp"

namespace te
{
    class DummySignalGenerator : public ISignalGenerator,
                                 public TestEquipment
    {
    public:
        DummySignalGenerator();
        DummySignalGenerator(const QString& resourceName);
        ~DummySignalGenerator();

        ResourceType resourceType() { return SignalGenerator; }
        QString resourceTypeString() { return QObject::tr("Signal Generator"); }

        bool reset();
        bool connect();

        bool setFrequency(uint64_t freq);
        bool setFrequency(uint64_t freq, QString unit);
        bool getFrequency(uint64_t& freq);

        bool setOutputPower(double power);
        bool getOutputPower(double& power);

        bool setRfEnabled(bool en);
        bool getRfEnabled(bool& en);

    private:
        uint64_t m_freq;
        double   m_power;
        bool     m_enabled;
    };
}

#endif // DUMMYSIGNALGENERATOR_HPP
