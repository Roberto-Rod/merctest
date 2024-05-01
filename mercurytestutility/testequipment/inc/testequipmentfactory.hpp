#ifndef TESTEQUIPMENTFACTORY_HPP
#define TESTEQUIPMENTFACTORY_HPP

#include "testequipment.hpp"

namespace te
{
    /**
     * @brief The TestEquipmentFactory abstract factory class - implemented as a "Singleton"
     */
    class TestEquipmentFactory
    {
    public:
        static TestEquipmentFactory* instance();

        virtual TestEquipment* makeSpectrumAnalyser(const QString& resourceName)= 0;
        virtual TestEquipment* makePowerMeter(const QString& resourceName)= 0;
        virtual TestEquipment* makeSignalGenerator(const QString& resourceName)= 0;

    protected:
        TestEquipmentFactory();

    private:
        static TestEquipmentFactory* m_pInstance;
    };
}

#endif // TESTEQUIPMENTFACTORY_HPP
