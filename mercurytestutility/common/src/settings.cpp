#include "settings.hpp"
#include "logging.hpp"
#include "jlink.hpp"
#include "assyrevision.hpp"

#include <QVariant>
#include <QDate>
#include <QRegExp>
#include <QDebug>
#include <QApplication>

namespace common
{
    const QString Settings::kOrganisation = "Kirintec Ltd";
    const QString Settings::kApplication  = "Mercury Test Utility";

    Settings* Settings::m_pInstance = 0;

    Settings::~Settings()
    {
        qDebug() << "Deleting Settings";
    }

    Settings* Settings::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new Settings(qApp);
        }

        return m_pInstance;
    }

    Settings::Settings(QObject* parent) :
        QSettings(kOrganisation, kApplication, parent)
    {
        resourceMap[te::ResourceType::SpectrumAnalyser] = "resources/names/spectrumanalyser";
        resourceMap[te::ResourceType::PowerMeter]       = "resources/names/powermeter";
        resourceMap[te::ResourceType::SignalGenerator]  = "resources/names/signalgenerator";
    }

    const QString Settings::resourceName(te::ResourceType type) const
    {
        return value(resourceMap.value(type), QVariant(QString("<undefined>"))).toString();
    }

    void Settings::setResourceName(te::ResourceType type, const QString& resourceName)
    {
        setValue(resourceMap.value(type), QVariant(resourceName));
    }

    void Settings::getCalExpiry(const QString& calKey, QDate& calExpiry) const
    {
        int32_t defaultYear;

        // Default calibration expiry to the first day of the year - this will prevent the user from
        // having to spin the year to the current year when setting the correct date.
        defaultYear = QDate::currentDate().year();

        // If it *is* the first day of the year then set the default date back to the previous year -
        // this prevents the default from being valid on New Years' Day each year!
        if ((QDate::currentDate().day() == 1) && (QDate::currentDate().month() == 1))
        {
            defaultYear--;
        }

        QDate defaultDate(defaultYear, 1, 1);
        QString defaultDateStr = defaultDate.toString("dd/MM/yyyy");

        QString str(value(QString("resources/calexpiry/%1").arg(calKey), defaultDateStr).toString());
        calExpiry = QDate::fromString(str, "dd/MM/yyyy");

        // If cal expiry was not sucessfuly converted then set to the default date
        if (!calExpiry.isValid())
        {
            calExpiry = defaultDate;
        }
    }

    bool Settings::getCalPoints(QString calKey, QVector<CalPoint>& calPoints)
    {
        bool retVal = false;

        // Clear the vector
        calPoints.clear();

        if (calKey != "")
        {
            // Begin reading an array
            int size = beginReadArray("resources/calpoints/" + calKey);

            // Write all the points to the array
            for (int i = 0; i < size; i++)
            {
                setArrayIndex(i);
                CalPoint point;
                point.freq = value("freq", 0ull).toULongLong();
                point.loss = value("loss", 0.0).toDouble();

                calPoints.append(point);
            }

            // End reading array
            endArray();

            retVal = true;
        }

        return retVal;
    }

    bool Settings::setCalPoints(QString calKey, const QVector<CalPoint>& calPoints)
    {
        bool retVal = false;

        if (calKey != "")
        {
            QString key = "resources/calpoints/" + calKey;

            // Clear old data
            remove(key);

            // Begin writing a new array
            beginWriteArray(key);

            // Write all the points to the array
            for (int i = 0; i < calPoints.length(); i++)
            {
                setArrayIndex(i);
                setValue("freq", calPoints.at(i).freq);
                setValue("loss", calPoints.at(i).loss);
            }

            // End writing array
            endArray();

            retVal = true;
        }

        return retVal;
    }

    bool Settings::getSanityResults(testjig::SanityCheckType sanityKey, testjig::sanityResult&  result)
    {
        bool retVal = false;
        QString sanityKeyStr = getSanityKey(sanityKey);

        if (sanityKeyStr != "")
        {
            QString maxKey("results/sanity/" + sanityKeyStr + "min");
            QString minKey("results/sanity/" + sanityKeyStr + "max");

            result.maxErrNeg = value(minKey, 99.0).toDouble();
            result.maxErrPos = value(maxKey, 99.0).toDouble();

            retVal = true;
        }

        return retVal;
    }

    bool Settings::setSanityResults(testjig::SanityCheckType sanityKey, const testjig::sanityResult&  result)
    {
        bool retVal = false;
        QString sanityKeyStr = getSanityKey(sanityKey);

        if (sanityKeyStr != "")
        {
            QString maxKey("results/sanity/" + sanityKeyStr + "min");
            QString minKey("results/sanity/" + sanityKeyStr + "max");

            setValue(minKey, result.maxErrNeg);
            setValue(maxKey, result.maxErrPos);

            retVal = true;
        }

        return retVal;
    }

    void Settings::setCalExpiry(const QString& calKey, const QDate& calExpiry)
    {
        setValue(QString("resources/calexpiry/%1").arg(calKey), calExpiry.toString("dd/MM/yyyy"));
    }

    bool Settings::teDebugMode() const
    {
        bool retVal = false;

        if (value(QString("tedebugmode")).toInt() == 1)
        {
            retVal = true;
        }

        return retVal;
    }

    QString Settings::jigPortName() const
    {
        QString str(value(QString("ports/testjig"), "<no port setting>").toString());
        return str;
    }

    void Settings::setJigPortName(QString portName)
    {
        setValue(QString("ports/testjig"), QVariant(portName));
    }

    QString Settings::srcPortName() const
    {
        QString str(value(QString("ports/src"), "<no port setting>").toString());
        return str;
    }

    void Settings::setSrcPortName(QString portName)
    {
        setValue(QString("ports/src"), QVariant(portName));
    }

    QString Settings::ctsPortName() const
    {
        QString str(value(QString("ports/cts"), "<no port setting>").toString());
        return str;
    }

    void Settings::setCtsPortName(QString portName)
    {
        setValue(QString("ports/cts"), QVariant(portName));
    }

    QString Settings::reportsDirectory() const
    {
        // Get the reports directory. If the key is not available then return the
        // current user's home directory
        QString str(value(QString("directories/reports"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString());
        return str;
    }

    void Settings::setReportsDirectory(QString dir)
    {
        setValue(QString("directories/reports"), QVariant(dir));
    }

    QString Settings::programmingScriptsDirectory() const
    {
        // Get the reports directory. If the key is not available then return the
        // current user's home directory
        QString str(value(QString("directories/programming"), QStandardPaths::writableLocation(QStandardPaths::TempLocation)).toString());
        return str;
    }

    void Settings::setProgrammingScriptsDirectory(QString dir)
    {
        setValue(QString("directories/programming"), QVariant(dir));
        QString msg(hardware::Jlink::createJlinkScripts() ? "Successfully created" : "Failed to create");
        msg += QString(" J-Link scripts in %1").arg(dir);
        Logging::instance()->log(msg, Logging::LOG_DEST_PROGRESS);
    }

    QString Settings::firmwareImageFileName(FirmwareImage img) const
    {
        // Get the file name. If the key or value are not available then return empty string
        QString key = firmwareImageKey(img);
        QString str;

        if (!key.isEmpty())
        {
            str = value(key, QString("")).toString();
        }

        return str;
    }

    void Settings::setFirmwareImageFileName(const QString& file, FirmwareImage img)
    {
        QString key = firmwareImageKey(img);

        // Set filename only if we have a key
        if (!key.isEmpty())
        {
            setValue(key, QVariant(file));
        }
    }

    QString Settings::firmwareImageKey(FirmwareImage img)
    {
        QString key;

        if      (img == FirmwareImage::SRC_BRD_FPGA) { key = "files/image_src_brd_fpga"; }
        else if (img == FirmwareImage::SRC_BRD_MAIN) { key = "files/image_src_brd_main"; }
        else if (img == FirmwareImage::SRC_BRD_BOOT) { key = "files/image_src_brd_boot"; }

        return key;
    }

    bool Settings::stopOnFail() const
    {
        return value(QString("stoponfail")).toBool();
    }

    void Settings::setStopOnFail(bool sof)
    {
        setValue(QString("stoponfail"), QVariant(sof));
    }

    QString Settings::getKey(AssyKey key)
    {
        QString skey = akey[key];
        return skey;
    }

    QString Settings::getSanityKey(testjig::SanityCheckType  key)
    {
        QString sanitykey = snkey[key];
        return sanitykey;
    }

    bool Settings::compareRevision(AssyKey key, int revision)
    {
        bool result = false;
        QString skey = getKey(key);
        int storedrev = this->value(skey).toInt();
        if(storedrev == revision)
        {
           result = true;
        }
        return result;
    }

    void Settings::setRevision(AssyKey key, int revision)
    {
        QString skey = getKey(key);
        this->setValue(skey, revision);
    }

     int Settings::getRevision(AssyKey key)
     {
         QString skey = getKey(key);
         int storedrev = this->value(skey).toInt();
         return storedrev;
     }

     /* void Settings::setUUTKey(assyKey key)
     {
         m_UUTkey = key;
     }

     assyKey Settings::getUUTKey()
     {
         return m_UUTkey;
     }  */

    bool Settings::revExists(AssyKey key)
    {
        bool result = false;
        QString skey = getKey(key);
        if(this->contains(skey))
        {
            result = true;
        }
        return result;
    }
}
