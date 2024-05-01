#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "testequipment.hpp"
#include "calibration.hpp"
#include "sanitycheckcal.hpp"

#include <QSettings>
#include <QMap>
#include <QVector>
#include <QDebug>

namespace common
{
    enum class FirmwareImage
    {
        SRC_BRD_BOOT,
        SRC_BRD_MAIN,
        SRC_BRD_FPGA,
        CTS_BRD_FIRMWARE
    };

    enum AssyKey
    {
        NULLASSY,
        PCB0044,
        ASY0033,
        ASY129,
        PSUMan,
        PSUVeh,
        ASY20or21,
        ASY130,
        PCB0114
    };

    class Settings : public QSettings
    {
    public:
        ~Settings();

        static Settings* instance();

        const QString resourceName(te::ResourceType type) const;
        void setResourceName(te::ResourceType type, const QString& resourceName);

        void getCalExpiry(const QString& calKey, QDate& calExpiry) const;
        void setCalExpiry(const QString& calKey, const QDate& calExpiry);

        bool getCalPoints(QString calKey, QVector<CalPoint>& calPoints);
        bool setCalPoints(QString calKey, const QVector<CalPoint>& calPoints);

        bool getSanityResults(testjig::SanityCheckType sanityKey, testjig::sanityResult& result);
        bool setSanityResults(testjig::SanityCheckType sanityKey, const testjig::sanityResult& result);

        QString jigPortName() const;
        void setJigPortName(QString portName);

        QString srcPortName() const;
        void setSrcPortName(QString portName);

        QString ctsPortName() const;
        void setCtsPortName(QString portName);

        QString reportsDirectory() const;
        void setReportsDirectory(QString dir);

        QString programmingScriptsDirectory() const;
        void setProgrammingScriptsDirectory(QString dir);

        QString firmwareImageFileName(FirmwareImage img) const;
        void setFirmwareImageFileName(const QString& file, FirmwareImage img);

        bool stopOnFail() const;
        void setStopOnFail(bool sof);

        bool teDebugMode() const;

        bool compareRevision(AssyKey key, int revision);
        int getRevision(AssyKey key);
        void setRevision(AssyKey key, int revision);
        bool revExists(AssyKey);
        QString getKey(AssyKey key);
        QString getSanityKey(testjig::SanityCheckType key);
        void setSanityResult(testjig::SanityCheckType key, testjig::sanityResult sanityRes);
        testjig::sanityResult getSanityResult(testjig::SanityCheckType key);

    protected:
        Settings(QObject* parent = 0);

    private:
        static const QString kOrganisation;
        static const QString kApplication;

        static Settings* m_pInstance;

        QMap<te::ResourceType, QString> resourceMap;

        static QString firmwareImageKey(FirmwareImage img);

        QStringList akey = { "files/NULLKEY",
                             "files/PCB0044",
                             "files/ASY0033",
                             "files/ASY129",
                             "files/PSUMan",
                             "files/PSUVeh",
                             "files/ASSY20or21",
                             "files/ASSY130",
                             "files/PCB0114" };

        QStringList snkey = { "sanity/SOURCEBOARD",
                              "sanity/POWERMONITOR",
                              "sanity/HIGHBAND_POWERMETER",
                              "sanity/HIGHBAND_SIGGEN" };
    };
}

#endif
