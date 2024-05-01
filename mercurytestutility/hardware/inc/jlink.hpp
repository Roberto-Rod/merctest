#ifndef JLINK_HPP
#define JLINK_HPP

#include <QObject>
#include <QMap>
#include <QProcess>
#include <QFile>

namespace hardware
{
    class Jlink : public QObject
    {
        Q_OBJECT

    public:
        enum class Device
        {
            CORTEX_M3,
            CORTEX_M4
        };

        Jlink(Device device, QObject* parent = 0);

        bool checkDeviceId();
        bool flashDevice(const QString& programmingBatchFile);
        static bool createJlinkScripts();

    private slots:
        void error(QProcess::ProcessError error);

        void finished(int exitCode, QProcess::ExitStatus exitStatus);

    private:
        static const uint32_t kCommandTimeout = 30ul; //!< Timeout before giving up on J-Link commands, seconds
        Device m_device;
        QMap<Device, QString> m_deviceIdMap;
        bool m_ok;
        bool m_complete;

        bool executeCommand(const QString& programBatchFile, const QString& expRespBegin);

        bool executeCommand(const QString& programBatchFile, const QString& expRespBegin, QString& resp);

        bool checkMessages(QStringList& stringlist, const QString& msg1, const QString& msg2, int lines);
    };
}

#endif // JLINK_HPP
