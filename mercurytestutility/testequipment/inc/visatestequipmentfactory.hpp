#ifndef VISATESTEQUIPMENTFACTORY_HPP
#define VISATESTEQUIPMENTFACTORY_HPP

#include "testequipmentfactory.hpp"

namespace te
{
    class VisaTestEquipmentFactory : public TestEquipmentFactory
    {
    public:
        static VisaTestEquipmentFactory* instance();

        TestEquipment* makeSpectrumAnalyser(const QString& resourceName);
        TestEquipment* makePowerMeter(const QString& resourceName);
        TestEquipment* makeSignalGenerator(const QString& resourceName);

    protected:
        VisaTestEquipmentFactory();

    private:
        static VisaTestEquipmentFactory* m_pInstance;
    };
}

#endif // VISATESTEQUIPMENTFACTORY_HPP
