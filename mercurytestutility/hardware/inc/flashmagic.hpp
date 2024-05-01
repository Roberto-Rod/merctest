#ifndef FLASHMAGIC_HPP
#define FLASHMAGIC_HPP

#include <QObject>
#include <QMap>
#include <QProcess>

namespace hardware
{
    class FlashMagic : public QObject
    {
        Q_OBJECT

    public:
        enum Device
        {
            LPC1778,
            LPC1788
        };

        FlashMagic(const QString& portName, Device device, QObject* parent = 0);

        bool checkDeviceId();

        bool flashDevice(const QString& hexFileName);

    private slots:
        void error(QProcess::ProcessError error);

        void finished(int exitCode, QProcess::ExitStatus exitStatus);

    private:
        static const uint32_t kCommandTimeout = 30ul;       //!< Timeout before giving up on FlashMagic commands, seconds
        QString m_portName;
        Device m_device;
        QMap<Device, QString> m_deviceNameMap;
        QMap<Device, uint32_t> m_deviceIdMap;
        bool m_ok;
        bool m_complete;

        bool executeCommand(const QString& cmd, const QString& expRespBegin);

        bool executeCommand(const QString& cmd, const QString& expRespBegin, QString& resp);

        bool eraseSectors(const QString& hexFileName);

        bool programFlash(const QString& hexFileName);

        bool verifyFlash(const QString& hexFileName);
    };
}

#endif // FLASHMAGIC_HPP
