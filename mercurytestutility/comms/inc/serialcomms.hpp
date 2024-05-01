#ifndef SERIALCOMMS_HPP
#define SERIALCOMMS_HPP

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QChar>

namespace comms
{
    class SerialComms : public QObject
    {
        Q_OBJECT

    public:
        SerialComms(QObject* parent = 0);

        SerialComms(int32_t readTimeout, QObject* parent = 0);

        virtual ~SerialComms() {}

        virtual bool openPort(QString portName)= 0;

        virtual bool isPortOpen() const = 0;

        virtual void closePort() = 0;

        virtual bool write(const QByteArray& bytesOut, bool waitWritten = false) = 0;

        virtual bool read(QByteArray& bytesIn, uint16_t maxBytes) = 0;

        virtual bool read(QByteArray& bytesIn, QString terminator) = 0;

        virtual void flush() = 0;

        bool setReadTimeout(int32_t milliseconds);

    signals:
        void commsClosed();

    protected:
        int32_t m_readTimeout;

    private:
        static const int32_t kDefaultReadTimeout = 1000; //!< Default read timeout in milliseconds
    };
}

#endif // SERIALCOMMS_HPP
