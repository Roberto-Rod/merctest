#ifndef VISASIGNALGENERATOR_HPP
#define VISASIGNALGENERATOR_HPP

#include "signalgenerator.hpp"
#include "visatestequipment.hpp"
#include "visaresource.hpp"

#include <QStringList>

namespace te
{
    class VisaSignalGenerator : public ISignalGenerator,
                                public VisaTestEquipment
    {
    public:
        VisaSignalGenerator(const QString& resourceName = "");
        ~VisaSignalGenerator();

        /** Overrides te::VisaTestEquipment::resourceType */
        ResourceType resourceType() { return SignalGenerator; }

        /** Overrides te::VisaTestEquipment::resourceTypeString */
        QString resourceTypeString() { return QObject::tr("Signal Generator"); }

        /** Overrides te::ISignalGenerator::reset */
        bool reset();

        /** Overrides te::ISignalGenerator::setFrequency */
        bool setFrequency(uint64_t freq);
        bool setFrequency(uint64_t freq, QString unit);

        /** Overrides te::ISignalGenerator::getFrequency */
        bool getFrequency(uint64_t& freq);

        /** Overrides te::ISignalGenerator::setOutputPower */
        bool setOutputPower(double power);

        /** Overrides te::ISignalGenerator::getOutputPower */
        bool getOutputPower(double& power);

        /** Overrides te::ISignalGenerator::setRfEnabled */
        bool setRfEnabled(bool en);

        /** Overrides te::ISignalGenerator::getRfEnabled */
        bool getRfEnabled(bool& en);

    private:
        static const QStringList kExpectedModels;
        static const QStringList kResetCmds;
        static const QString kSetFreqCmd;
        static const QString kSetFreqAndUnitCmd;
        static const QString kGetFreqCmd;
        static const QString kSetRfLevCmd;
        static const QString kGetRfLevCmd;
        static const QString kRfEnableCmd;
        static const QString kRfDisableCmd;
        static const QString kGetRfEnCmd;
    };
}

#endif // VISASIGNALGENERATOR_HPP
