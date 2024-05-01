#include "xmodemwrapper.hpp"
#include "logging.hpp"

#include <QFile>
#include <QApplication>

namespace comms
{
    XModemWrapper* XModemWrapper::m_pXmodemWrapper = 0;

    XModemWrapper::XModemWrapper(QObject* parent) :
        QObject(parent),
        m_bytesSent(0ul)
    {
    }

    XModemWrapper* XModemWrapper::instance()
    {
        if (m_pXmodemWrapper == 0)
        {
            m_pXmodemWrapper = new XModemWrapper(qApp);
        }

        return m_pXmodemWrapper;
    }

    void XModemWrapper::setSerialComms(SerialComms* pSerialComms)
    {
        m_pSerialComms = pSerialComms;
    }

    bool XModemWrapper::sendFile(QFile& file)
    {
        bool retVal = false;

        m_bytesSent = 0ul;

        if (file.isReadable())
        {
            // Read all of the bytes from the current file position
            QByteArray fileBytes = file.readAll();

            if (m_pSerialComms->isPortOpen() && (fileBytes.length() > 0))
            {
                // Now transmit the data
                int st = xmodemTransmit((unsigned char*)(fileBytes.data()), fileBytes.length());

                if (st > 0)
                {
                    retVal = true;
                }
                else
                {
                    common::Logging::instance()->log(QString("xmodem error code: %1").arg(st));
                }
            }
        }

        return retVal;
    }

    char XModemWrapper::inByte(uint16_t timeout, bool& ok)
    {
        char c = 0;
        ok = false;

        QByteArray byteArray;

        m_pSerialComms->setReadTimeout(static_cast<int32_t>(timeout));
        if (m_pSerialComms->read(byteArray, 1u))
        {
            if (byteArray.length() == 1)
            {
                c = byteArray.at(0);
                ok = true;
            }
        }

        return c;
    }

    void XModemWrapper::outByte(char c)
    {
        QByteArray byteArray;
        byteArray.append(c);
        m_pSerialComms->write(byteArray);
    }

    void XModemWrapper::packetSent()
    {
        // Increment bytes sent count
        m_bytesSent += kPacketSize;

        // and emit signal
        emit progress(static_cast<int>(m_bytesSent));
    }
}

int _inbyte(unsigned short timeout)
{
    int retVal = -1;

    bool ok;
    char c = comms::XModemWrapper::instance()->inByte(timeout, ok) & 0xFF;

    if (ok) { retVal = static_cast<int>(c); }

    return retVal;
}

void _outbyte(int c)
{
    comms::XModemWrapper::instance()->outByte(static_cast<char>(c));
}

void packetSent()
{
    comms::XModemWrapper::instance()->packetSent();
}
