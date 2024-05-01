#include "ctstestband.hpp"

namespace testsuite
{
    const QMap<CtsTestBandType, QString> CtsTestBand::kTestBandTypeMap = QMap<CtsTestBandType, QString>({{CtsTestBandType::Stop, "Stop"},
                                                                                                         {CtsTestBandType::Transition, "Transition"},
                                                                                                         {CtsTestBandType::Pass, "Pass"}});

    QString CtsTestBand::typeString()
    {
        return kTestBandTypeMap[m_type];
    }
}
