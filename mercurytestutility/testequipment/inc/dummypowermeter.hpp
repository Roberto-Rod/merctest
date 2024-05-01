#ifndef DUMMYPOWERMETER_HPP
#define DUMMYPOWERMETER_HPP

#include "powermeter.hpp"
#include "testequipment.hpp"

namespace te
{
    class DummyPowerMeter : public IPowerMeter,
                            public TestEquipment
    {
    public:
        DummyPowerMeter();
        DummyPowerMeter(const QString& resourceName);
        ~DummyPowerMeter();

        ResourceType resourceType() { return PowerMeter; }
        QString resourceTypeString() { return QObject::tr("Power Meter"); }

        bool reset();
        bool connect();

        bool zeroSensor();

        bool setOffset(double offset);
        bool getOffset(double& offset);

        bool setFrequency(uint64_t freq);
        bool getFrequency(uint64_t& freq);

        bool getPower(double& power);

    private:
        double   m_offset;
        uint64_t m_freq;
    };
}

#endif // DUMMYPOWERMETER_HPP
