#include "testequipmentfactory.hpp"
#include "dummytestequipmentfactory.hpp"
#include "visatestequipmentfactory.hpp"
#include "settings.hpp"

namespace te
{
    TestEquipmentFactory* TestEquipmentFactory::m_pInstance = 0;

    TestEquipmentFactory::TestEquipmentFactory()
    {
    }

    TestEquipmentFactory* TestEquipmentFactory::instance()
    {
        if (m_pInstance == 0)
        {
            if (common::Settings::instance()->teDebugMode())
            {
                m_pInstance = DummyTestEquipmentFactory::instance();
            }
            else
            {
                m_pInstance = VisaTestEquipmentFactory::instance();
            }
        }

        return m_pInstance;
    }
}
