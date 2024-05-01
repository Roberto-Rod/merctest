#ifndef VISAPOWERMETER_HPP
#define VISAPOWERMETER_HPP

#include "powermeter.hpp"
#include "visatestequipment.hpp"
#include "visaresource.hpp"

#include <QStringList>

namespace te
{
    /** @brief Power Meter which uses a VisaResource to communicate */
    class VisaPowerMeter : public IPowerMeter,
                           public VisaTestEquipment
    {
    public:
        /** @brief Constructor with Visa Resource Name specified.
         *
         * Note: Visa Resource Name can be changed later using
         * te::VisaTestEquipment::setResourceName()
         *
         * @param resourceName The Visa Resource Name to assign to the
         * VisaResource on construction
         */
        VisaPowerMeter(const QString& resourceName = "");

        ~VisaPowerMeter();

        /** @sa te::TestEquipment::resourceType */
        ResourceType resourceType() { return PowerMeter; }

        /** @sa te::TestEquipment::resourceTypeString */
        QString resourceTypeString() { return QObject::tr("Power Meter"); }

        /** @sa te::IPowerMeter::reset */
        bool reset();

        /** @sa te::IPowerMeter::zeroSensor */
        bool zeroSensor();

        /** @sa te::IPowerMeter::setOffset */
        bool setOffset(double offset);

        /** @sa te::IPowerMeter::getOffset */
        bool getOffset(double& offset);

        /** @sa te::IPowerMeter::setFrequency */
        bool setFrequency(uint64_t freq);

        /** @sa te::IPowerMeter::getFrequency */
        bool getFrequency(uint64_t& freq);

        /** @sa te::IPowerMeter::getPower */
        bool getPower(double& power);

    private:
        static const QStringList kExpectedModels;
        static const QStringList kResetCmds;        //!< List of reset commands
        static const QString kZeroCmd;              //!< Zero sensor command
        static const QString kEnableOffsCmd;        //!< Enable offset command
        static const QString kDisableOffsCmd;       //!< Disable offset command
        static const QString kSetOffsCmd;           //!< Set offset command
        static const QString kGetOffsCmd;           //!< Get offset command
        static const QString kSetFreqCmd;           //!< Set frequency command
        static const QString kGetFreqCmd;           //!< Get frequency command
        static const QString kInitMeas;             //!< Initialise measurement command
        static const QString kFetchMeas;            //!< Fetch measurement command
    };
}

#endif // VISAPOWERMETER_HPP
