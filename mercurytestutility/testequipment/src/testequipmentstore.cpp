#include "testequipmentstore.hpp"
#include "testequipmentfactory.hpp"
#include "settings.hpp"

namespace te
{
    TestEquipmentStore* TestEquipmentStore::m_pInstance = 0;

    TestEquipmentStore::TestEquipmentStore()
    {
        common::Settings* settings = common::Settings::instance();
        te::TestEquipmentFactory* teFactory = te::TestEquipmentFactory::instance();

        m_testEquipment.append(teFactory->makeSpectrumAnalyser(settings->resourceName(te::SpectrumAnalyser)));
        m_testEquipment.append(teFactory->makePowerMeter(settings->resourceName(te::PowerMeter)));
        m_testEquipment.append(teFactory->makeSignalGenerator(settings->resourceName(te::SignalGenerator)));
    }

    TestEquipmentStore* TestEquipmentStore::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new TestEquipmentStore;
        }

        return m_pInstance;
    }

    TestEquipment* TestEquipmentStore::te(ResourceType type)
    {
        TestEquipment* te = 0;

        if (type == ResourceType::SpectrumAnalyser)
        {
            te = instance()->te().at(0);
        }
        else if (type == ResourceType::PowerMeter)
        {
            te = instance()->te().at(1);
        }
        else if (type == ResourceType::SignalGenerator)
        {
            te = instance()->te().at(2);
        }

        return te;
    }

    ISpectrumAnalyser* TestEquipmentStore::spectrumAnalyser()
    {
        return dynamic_cast<ISpectrumAnalyser*>(te(ResourceType::SpectrumAnalyser));
    }

    IPowerMeter* TestEquipmentStore::powerMeter()
    {
        return dynamic_cast<IPowerMeter*>(te(ResourceType::PowerMeter));
    }

    ISignalGenerator* TestEquipmentStore::signalGenerator()
    {
        return dynamic_cast<ISignalGenerator*>(te(ResourceType::SignalGenerator));
    }

    const QList<TestEquipment*>& TestEquipmentStore::te()
    {
        return m_testEquipment;
    }
}
