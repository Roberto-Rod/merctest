#ifndef DIRECTSERIALCOMMS_HPP
#define DIRECTSERIALCOMMS_HPP

#include "serialcomms.hpp"

#include <QObject>
#include <QSerialPort>

namespace comms
{
    class DirectSerialComms : public SerialComms
    {
        Q_OBJECT

    public:
        DirectSerialComms(QObject* parent = 0);

        DirectSerialComms(int32_t readTimeout, QObject* parent = 0);

        ~DirectSerialComms();

        bool openPort(QString portName);

        bool isPortOpen() const;

        void closePort();

        bool write(const QByteArray& bytesOut, bool waitWritten = false);

        bool read(QByteArray& bytesIn, uint16_t maxBytes);

        bool read(QByteArray& bytesIn, QString terminator);

        void flush();

    private slots:
        void handleError(QSerialPort::SerialPortError error);

    private:
        QSerialPort* m_pSerialPort;
        bool m_resourceError;
    };
}

#endif // DIRECTSERIALCOMMS_HPP
