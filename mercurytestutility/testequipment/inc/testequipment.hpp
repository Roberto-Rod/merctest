#ifndef TESTEQUIPMENT_HPP
#define TESTEQUIPMENT_HPP

#include "calibration.hpp"

#include <cstdint>

#include <QString>
#include <QDate>
#include <QMap>
#include <QtCore>

namespace te
{
    enum ResourceType
    {
        SpectrumAnalyser,
        PowerMeter,
        SignalGenerator
    };

    struct TeDetails
    {
        TeDetails();
        TeDetails(const TeDetails& other);
        TeDetails& operator =(const TeDetails& other);

        QString m_manufacturer;                         //!< Manufacturer name as reported by equipment
        QString m_model;                                //!< Model number as reported by equipment
        QString m_serialNumber;                         //!< Serial number as reported by equipment
        QString m_firmwareVersion;                      //!< Firmware version/versions as reported by equipment
    };

    class TestEquipment : public common::Calibration
    {
    public:
        TestEquipment();

        virtual ~TestEquipment();

        /**
         * @brief connect Connect to equipment and get details
         *
         * @return true if equipment is detected
         */
        virtual bool connect()= 0;

        /**
         * @brief detected
         * @return true if the equipment was detected on the last connection attempt
         */
        bool detected() const;

        virtual bool reset()= 0;

        virtual ResourceType resourceType()= 0;

        virtual QString resourceTypeString()= 0;

        /**
         * @brief setResourceName Sets the equipments resource name
         *
         * This operation is virtual as subclasses may want to take action
         * when the resource name is changed (e.g. reattempt VISA connection)
         *
         * @param name The new resource name
         */
        virtual void setResourceName(const QString& name);

        /**
         * @brief resourceName Gets the current resource name
         * @return The current resource name
         */
        const QString& resourceName() const;

        void resetDetails();

        const TeDetails& details() const;

        /**
         * @brief setDetails Decodes an ID string and sets the details member variables accordingly
         *
         * @param id The identification string, comma separated, containing 4 fields of the form:
         * manufacturer_name,model,serial_number,firmware_version
         *
         * @return true if details were set, false if there was a parsing error
         */
        bool setDetails(const QString& id);

        /**
         * @brief formattedDetails Gets the equipment details in a formatted string intended for inclusion in test reports
         *
         * @return The formatted details string
         */
        QString formattedDetails(bool briefDetails = false);

        static const QString& typeToString(te::ResourceType type);

        static bool checkTeAvailable(const QList<TestEquipment*>& teList);

        static bool resetTe(const QList<TestEquipment*>& teList);

    protected:
        TeDetails m_details;                            //!< Test Equipment details
        bool m_detected;                                //!< True if equipment was detected on last connection attempt
        QString m_resourceName;                         //!< Resource name - for VISA subclasses this will be used as the VISA resource name

    private:
        static const QString kDefaultResourceName;      //!< Default resource name for equipment which has not been detected

        static QMap<te::ResourceType, QString> m_typeToStringMap;

        static QMap<te::ResourceType, QString> initTypeToString();
    };
}
#endif // TESTEQUIPMENT_HPP
