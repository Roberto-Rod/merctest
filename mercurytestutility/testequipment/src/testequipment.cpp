#include "testequipment.hpp"
#include "settings.hpp"
#include "messagedialog.hpp"
#include "griddialog.hpp"
#include "powermeter.hpp"

#include <QStringList>
#include <QRegExp>
#include <QList>

namespace te
{
    TeDetails::TeDetails() :
        m_manufacturer(QObject::tr("<not found>")),
        m_model(QObject::tr("<not found>")),
        m_serialNumber(QObject::tr("<not found>")),
        m_firmwareVersion(QObject::tr("<not found>"))
    {
    }

    TeDetails::TeDetails(const TeDetails& other)
    {
        *this = other;
    }

    TeDetails& TeDetails::operator =(const TeDetails& other)
    {
        m_manufacturer    = other.m_manufacturer;
        m_model           = other.m_model;
        m_serialNumber    = other.m_serialNumber;
        m_firmwareVersion = other.m_firmwareVersion;

        return *this;
    }

    const QString TestEquipment::kDefaultResourceName = QObject::tr("<undefined>");

    TestEquipment::TestEquipment() :
        common::Calibration(),
        m_details(),
        m_detected(false),
        m_resourceName(kDefaultResourceName)
    {
    }

    TestEquipment::~TestEquipment()
    {
    }

    bool TestEquipment::detected() const
    {
        return m_detected;
    }

    void TestEquipment::setResourceName(const QString& name)
    {
        m_resourceName = name;
    }

    const QString& TestEquipment::resourceName() const
    {
        return m_resourceName;
    }

    void TestEquipment::resetDetails()
    {
        // Reset all the members except for the resource name - we want to maintain
        // this as it was restored from the Settings class and can be used to rescan
        // for the equipment
        m_details      = TeDetails();
        m_calKey       = "";
    }

    const TeDetails& TestEquipment::details() const
    {
        return m_details;
    }

    bool TestEquipment::setDetails(const QString& id)
    {
        bool retVal = false;

        // Create a copy of the ID string and strip unwanted characters (new-lines and tabs)
        QString trimmed(id);
        trimmed.remove(QRegExp("[\\r\\n\\t]"));

        // Split the comma-separated ID string into fields
        QStringList fields = trimmed.split(",");

        // The VISA *IDN? command's response is specified as four comma-separated
        // fields. If we have not received such a response then consider it an error
        if (fields.length() == 4)
        {
            // Use the ID string as the calibration key but remove backslashes (as they are not
            // not allowed in the Windows registry) and forward slashes (as they create subkeys in
            // the windows registry)
            m_calKey = trimmed.remove(QRegExp("[\\/]"));

            // If we're left with an invalid key then this just isn't going to work - in that case
            // do not change the details fields
            if (isCalKeyValid())
            {
                m_details.m_manufacturer    = fields.at(0).trimmed();
                m_details.m_model           = fields.at(1).trimmed();
                m_details.m_serialNumber    = fields.at(2).trimmed();
                m_details.m_firmwareVersion = fields.at(3).trimmed();

                retVal = true;
            }
        }

        return retVal;
    }

    QString TestEquipment::formattedDetails(bool briefDetails)
    {
        QString details;

        if (!briefDetails)
        {
            details.append(QString("Equipment Type:      %1\r\n").arg(resourceTypeString()));
            details.append(QString("Manufacturer:        %1\r\n").arg(m_details.m_manufacturer));
            details.append(QString("Model:               %1\r\n").arg(m_details.m_model));
            details.append(QString("Serial Number:       %1\r\n").arg(m_details.m_serialNumber));
            details.append(QString("Firmware Version(s): %1\r\n").arg(m_details.m_firmwareVersion));
            details.append(QString("Calibration Expiry:  %1\r\n").arg(calExpiry().toString("dd/MM/yyyy")));
            details.append(QString("Calibration Status:  %1\r\n").arg(isCalExpired() ?
                                                                   QObject::tr("** EXPIRED **") :
                                                                   QObject::tr("Valid")));
        }
        else
        {
            details.append(QString("Mfr: %1\r\n").arg(m_details.m_manufacturer));
            details.append(QString("Model: %1\r\n").arg(m_details.m_model));
            details.append(QString("Serial: %1\r\n").arg(m_details.m_serialNumber));
            details.append(QString("Version: %1\r\n").arg(m_details.m_firmwareVersion));
            details.append(QString("Expiry: %1\r\n").arg(calExpiry().toString("dd/MM/yyyy")));
            details.append(QString("Status: %1\r\n").arg(isCalExpired() ?
                                                                               QObject::tr("** EXPIRED **") :
                                                                               QObject::tr("Valid")));
        }
        return details;
    }

    const QString& TestEquipment::typeToString(te::ResourceType type)
    {
        return m_typeToStringMap[type];
    }

    QMap<te::ResourceType, QString> TestEquipment::m_typeToStringMap = initTypeToString();

    QMap<te::ResourceType, QString> TestEquipment::initTypeToString()
    {
        QMap<te::ResourceType, QString> map;
        map[SpectrumAnalyser] = QObject::tr("Spectrum Analyser");
        map[PowerMeter]       = QObject::tr("Power Meter");
        map[SignalGenerator]  = QObject::tr("Signal Generator");

        return map;
    }

    bool TestEquipment::checkTeAvailable(const QList<TestEquipment*>& teList)
    {
        bool retVal = false;

        // Check that necessary TE is all available
        QString unavailableTe;

        int listNumber = 1;
        for (int i = 0; i < teList.length(); i++)
        {
            if (!teList.at(i)->detected())
            {
                unavailableTe += QString("<P><B>%1. ").arg(listNumber++) +
                                 teList.at(i)->resourceTypeString() + "</B>";
            }
        }

        if (unavailableTe.isNull())
        {
            // All requested equipment was detected
            retVal = true;
        }
        else
        {
            QString msg = "Some of the required test equipment is not available."
                          "<P>Please ensure that the following equipment is connected and discovered:";

            msg += unavailableTe;

            gui::MessageDialog dialog(msg, gui::MessageDialogType::ERR, true, false);
            dialog.exec();
        }

        return retVal;
    }

    bool TestEquipment::resetTe(const QList<TestEquipment*>& teList)
    {
        bool retVal = true;

        // Create a Grid Dialog
        gui::GridDialog gridDialog("Prepare Test Equipment", 4, 2);
        gridDialog.show();

        // Setup the grid dialog rows
        int gridRow = 0;
        for (int i = 0; i < teList.length(); i++)
        {
            // All equipment supports reset method
            gridDialog.setItem(QString("Reset %1...").arg(teList.at(i)->resourceTypeString()), gridRow++, 0);

            // Power Meter also supports zero method
            if (teList.at(i)->resourceType() == ResourceType::PowerMeter)
            {
                gridDialog.setItem(QString("Zero %1...").arg(teList.at(i)->resourceTypeString()), gridRow++, 0);
            }
        }

        // Reset/zero the equipment
        gridRow = 0;
        for (int i = 0; (i < teList.length()) && retVal; i++)
        {
            // All equipment supports reset method
            if (teList.at(i)->reset())
            {
                gridDialog.setItem("Done", gridRow++, 1);

                // Power Meter also supports zero method
                if (teList.at(i)->resourceType() == ResourceType::PowerMeter)
                {
                    IPowerMeter* pwrMtr = dynamic_cast<IPowerMeter*>(teList.at(i));
                    if ((pwrMtr != 0) && (pwrMtr->zeroSensor()))
                    {
                        gridDialog.setItem("Done", gridRow++, 1);
                    }
                    else
                    {
                        gridDialog.setItem("FAILED!", gridRow++, 1);
                        retVal = false;
                    }
                }
            }
            else
            {
                gridDialog.setItem("FAILED!", gridRow++, 1);
                retVal = false;
            }
        }

        return retVal;
    }
}
