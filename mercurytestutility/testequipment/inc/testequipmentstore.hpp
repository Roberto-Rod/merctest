#ifndef TESTEQUIPMENTSTORE_HPP
#define TESTEQUIPMENTSTORE_HPP

#include "testequipment.hpp"
#include "signalgenerator.hpp"
#include "powermeter.hpp"
#include "spectrumanalyser.hpp"

#include <QList>

namespace te
{
    /**
     * @brief The TestEquipmentStore class - implemented as a "Singleton"
     */
    class TestEquipmentStore
    {
    public:
        static TestEquipmentStore* instance();

        static TestEquipment*      te(ResourceType type);

        static ISpectrumAnalyser*  spectrumAnalyser();
        static IPowerMeter*        powerMeter();
        static ISignalGenerator*   signalGenerator();

        const QList<TestEquipment*>& te();

    protected:
        TestEquipmentStore();

    private:
        static TestEquipmentStore* m_pInstance;

        QList<te::TestEquipment*> m_testEquipment;
    };
}

#endif // TESTEQUIPMENTSTORE_HPP
