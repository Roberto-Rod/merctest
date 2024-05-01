#include "directserialcomms.hpp"
#include <QDebug>

namespace comms
{
    DirectSerialComms::DirectSerialComms(QObject* parent) :
        SerialComms(parent),
        m_pSerialPort(new QSerialPort(this)),
        m_resourceError(false)
    {
    }

    DirectSerialComms::DirectSerialComms(int32_t readTimeout, QObject* parent) :
        SerialComms(readTimeout, parent),
        m_pSerialPort(new QSerialPort(this)),
        m_resourceError(false)
    {
    }

    DirectSerialComms::~DirectSerialComms()
    {
        qDebug() << "Delete Serial Comms";
    }

    bool DirectSerialComms::openPort(QString portName)
    {
        bool retVal = false;

        if (!m_resourceError)
        {
            // Close serial port if it is currently open
            closePort();

            // Attempto to open the named port, using fixed settings: 115,200 bps 8N1
            m_pSerialPort->setPortName(portName);
            m_pSerialPort->setBaudRate(115200);
            m_pSerialPort->setDataBits(QSerialPort::Data8);
            m_pSerialPort->setParity(QSerialPort::NoParity);
            m_pSerialPort->setStopBits(QSerialPort::OneStop);
            m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);

            if (m_pSerialPort->open(QIODevice::ReadWrite))
            {
                retVal = true;

                // Connect the comms closed signal
                connect(m_pSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),
                         this,          SLOT  (handleError(QSerialPort::SerialPortError)));
            }
        }

        return retVal;
    }

    bool DirectSerialComms::isPortOpen() const
    {
        bool retVal = false;

        if (!m_resourceError && (m_pSerialPort != 0))
        {
            retVal = m_pSerialPort->isOpen();
        }

        return retVal;
    }

    void DirectSerialComms::closePort()
    {
        if (!m_resourceError && (m_pSerialPort != 0))
        {
            m_pSerialPort->close();
        }
    }

    bool DirectSerialComms::write(const QByteArray& bytesOut, bool waitWritten)
    {
        bool retVal = false;

        if (isPortOpen())
        {
            if (m_pSerialPort->write(bytesOut) == bytesOut.length())
            {
                if (waitWritten)
                {
                    int retryCount(0);

                    while (m_pSerialPort->bytesToWrite() > 0 && retryCount < 30)
                    {
                        m_pSerialPort->waitForBytesWritten(25);
                        retryCount++;
                    }

                    retVal = m_pSerialPort->bytesToWrite() == 0;
                }
                else
                {
                    retVal = true;
                }
            }
        }

        return retVal;
    }

    bool DirectSerialComms::read(QByteArray& bytesIn, uint16_t maxBytes)
    {
        bool retVal = false;

        if (isPortOpen())
        {
            bytesIn.clear();

            while ((bytesIn.length() < maxBytes) && (m_pSerialPort->waitForReadyRead(m_readTimeout)))
            {
                bytesIn.append(m_pSerialPort->readAll());
            }

            retVal = true;
        }

        return retVal;
    }

    bool DirectSerialComms::read(QByteArray& bytesIn, QString terminator)
    {
        bool retVal = false;

        if (isPortOpen())
        {
            // Wait for the terminator to be present in the array of bytes we've read
            while ((bytesIn.lastIndexOf(terminator) == -1) &&
                   (m_pSerialPort->waitForReadyRead(m_readTimeout)))
            {
                bytesIn.append(m_pSerialPort->readAll());
            }

            // Chop the terminator and everything that follows it off the byte array
            // If the terminator was not found then this will empty the array
            int idx = bytesIn.lastIndexOf(terminator);
            bytesIn.chop(bytesIn.length() - idx);

            if (bytesIn.length() > 0)
            {
                retVal = true;
            }
        }

        return retVal;
    }

    void DirectSerialComms::flush()
    {
        if (isPortOpen())
        {
            // Just keep reading until there's nothing left (within timeout)
            while (m_pSerialPort->waitForReadyRead(m_readTimeout))
            {
                m_pSerialPort->readAll();
            }
        }
    }

    void DirectSerialComms::handleError(QSerialPort::SerialPortError error)
    {
        // ResourceError occurs when device is removed from system
        if (error == QSerialPort::ResourceError)
        {
            qDebug() << "Comms Closed";
            m_resourceError = true;

            emit commsClosed();
        }
    }
}
