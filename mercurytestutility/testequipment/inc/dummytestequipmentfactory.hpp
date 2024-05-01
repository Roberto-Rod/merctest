#ifndef DUMMYTESTEQUIPMENTFACTORY_HPP
#define DUMMYTESTEQUIPMENTFACTORY_HPP

#include "testequipmentfactory.hpp"

namespace te
{
    class DummyTestEquipmentFactory : public TestEquipmentFactory
    {
    public:
        static DummyTestEquipmentFactory* instance();

        TestEquipment* makeSpectrumAnalyser(const QString& resourceName);
        TestEquipment* makePowerMeter(const QString& resourceName);
        TestEquipment* makeSignalGenerator(const QString& resourceName);

    protected:
        DummyTestEquipmentFactory();

    private:
        static DummyTestEquipmentFactory* m_pInstance;
    };
}

#endif // DUMMYTESTEQUIPMENTFACTORY_HPP
