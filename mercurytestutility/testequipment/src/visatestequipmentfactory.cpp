#include "visatestequipmentfactory.hpp"
#include "visaspectrumanalyser.hpp"
#include "visapowermeter.hpp"
#include "visasignalgenerator.hpp"

namespace te
{
    VisaTestEquipmentFactory* VisaTestEquipmentFactory::m_pInstance = 0;

    VisaTestEquipmentFactory::VisaTestEquipmentFactory()
    {
    }

    VisaTestEquipmentFactory* VisaTestEquipmentFactory::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new VisaTestEquipmentFactory;
        }

        return m_pInstance;
    }

    TestEquipment* VisaTestEquipmentFactory::makeSpectrumAnalyser(const QString& resourceName)
    {
        return new VisaSpectrumAnalyser(resourceName);
    }

    TestEquipment* VisaTestEquipmentFactory::makePowerMeter(const QString& resourceName)
    {
        return new VisaPowerMeter(resourceName);
    }

    TestEquipment* VisaTestEquipmentFactory::makeSignalGenerator(const QString& resourceName)
    {
        return new VisaSignalGenerator(resourceName);
    }
}
