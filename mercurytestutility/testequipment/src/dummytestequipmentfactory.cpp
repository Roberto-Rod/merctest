#include "dummytestequipmentfactory.hpp"
#include "dummyspectrumanalyser.hpp"
#include "dummypowermeter.hpp"
#include "dummysignalgenerator.hpp"

namespace te
{
    DummyTestEquipmentFactory* DummyTestEquipmentFactory::m_pInstance = 0;

    DummyTestEquipmentFactory::DummyTestEquipmentFactory()
    {
    }

    DummyTestEquipmentFactory* DummyTestEquipmentFactory::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new DummyTestEquipmentFactory;
        }

        return m_pInstance;
    }

    TestEquipment* DummyTestEquipmentFactory::makeSpectrumAnalyser(const QString& resourceName)
    {
        return new DummySpectrumAnalyser(resourceName);
    }

    TestEquipment* DummyTestEquipmentFactory::makePowerMeter(const QString& resourceName)
    {
        return new DummyPowerMeter(resourceName);
    }

    TestEquipment* DummyTestEquipmentFactory::makeSignalGenerator(const QString& resourceName)
    {
        return new DummySignalGenerator(resourceName);
    }
}
