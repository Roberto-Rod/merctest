#include "ctsboardcomms.hpp"
#include "serialcomms.hpp"

#include <QTextStream>
#include <QThread>

namespace comms
{
    const QString CtsBoardComms::kTerminator = QString("> ");
    const QString CtsBoardComms::kResponseOk = QString("OK");

    CtsBoardComms::CtsBoardComms(SerialComms* pComms, QObject* parent) :
        QObject(parent),
        m_pSerialComms(pComms)
    {
        // Set the read timeout
        pComms->setReadTimeout(kReadTimeout);
    }

    CtsBoardComms::~CtsBoardComms()
    {
    }

    SerialComms* CtsBoardComms::serialComms()
    {
        return m_pSerialComms;
    }

    bool CtsBoardComms::isReady() const
    {
        bool ok(false);

        if (m_pSerialComms && m_pSerialComms->isPortOpen())
        {
            // Send an empty command (just CR) and check for a valid response
            // (the sendCommand method checks for the '>' character being returned)
            ok = sendCommandExpectResponse("", "");
        }

        return ok;
    }

    bool CtsBoardComms::waitReady() const
    {
        bool ok(false);

        if (m_pSerialComms && m_pSerialComms->isPortOpen())
        {
            // Calculate retry count based on wait time required and the read timeout on each try.
            // The extra time spent going round the loop and sending the CRs is neglected.
            int32_t retryCount = kWaitReadyTimeout / kReadTimeout;

            for (int i = 0; (i < retryCount) && !ok; i++)
            {
                ok = isReady();
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, QString& resp) const
    {
        bool ok(false);

        // Add the terminator (a single Carriage Return) to the command, remove any CR/LF
        // that have previously been added
        cmd.remove(QRegExp("[\\r\\n]"));
        cmd.append("\r");

        // Convert the command string to a byte array
        QByteArray txBytes(cmd.toUtf8());
        QByteArray rxBytes;

        // Write the bytes out and wait till they're written
        if (m_pSerialComms && m_pSerialComms->isPortOpen())
        {
            if (m_pSerialComms->write(txBytes, true))
            {
                // Read the port and wait for the terminator character to be received
                // The read method will chop the terminator character and all that
                // follow it from the byte array.
                if (m_pSerialComms->read(rxBytes, kTerminator))
                {
                    // Convert the response byte array to a string
                    resp = QString::fromUtf8(rxBytes);

                    // Remove the echoed command
                    resp.remove(0, cmd.length() - 1);

                    // Remove everything on final line (text that preceded the terminator)
                    int idx = resp.lastIndexOf("\r");
                    if (idx >= 0)
                    {
                        resp.truncate(idx);
                    }

                    // Remove any CR/LF characters and leading/trailing whitespace from the string
                    resp.remove(QRegExp("[\\r\\n]"));
                    resp = resp.trimmed();

                    ok = true;
                }
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, uint32_t& resp) const
    {
        bool ok(false);
        QString strResp;

        if (sendCommand(cmd, strResp))
        {
            QStringList parts(strResp.split(" "));

            if (parts.length() > 0)
            {
                resp = parts.at(0).toUInt(&ok);
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, int32_t& resp) const
    {
        bool ok(false);
        QString strResp;

        if (sendCommand(cmd, strResp))
        {
            QStringList parts(strResp.split(" "));

            if (parts.length() > 0)
            {
                resp = parts.at(0).toInt(&ok);
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, uint16_t& resp) const
    {
        bool ok(false);
        uint32_t value;

        if (sendCommand(cmd, value))
        {
            if (value <= std::numeric_limits<uint16_t>::max())
            {
                resp = static_cast<uint16_t>(value);
                ok = true;
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, int16_t& resp) const
    {
        bool ok(false);
        int32_t value;

        if (sendCommand(cmd, value))
        {
            if (value >= std::numeric_limits<int16_t>::min() && value <= std::numeric_limits<int16_t>::max())
            {
                resp = static_cast<int16_t>(value);
                ok = true;
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, uint8_t& resp) const
    {
        bool ok(false);
        uint32_t value;

        if (sendCommand(cmd, value))
        {
            if (value <= std::numeric_limits<uint8_t>::max())
            {
                resp = static_cast<uint8_t>(value);
                ok = true;
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, float& resp) const
    {
        bool ok(false);
        QString strResp;

        if (sendCommand(cmd, strResp))
        {
            QStringList parts(strResp.split(" "));

            if (parts.length() > 0)
            {
                resp = parts.at(0).toFloat(&ok);
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd, bool& resp) const
    {
        bool ok(false);
        uint16_t value;

        if (sendCommand(cmd, value))
        {
            if (value <= 1)
            {
                resp = value;
                ok = true;
            }
        }

        return ok;
    }

    bool CtsBoardComms::sendCommand(QString cmd) const
    {
        return sendCommandExpectResponse(cmd, kResponseOk);
    }

    bool CtsBoardComms::sendCommandExpectResponse(QString cmd, QString expect) const
    {
        bool ok(false);
        QString resp;

        // Send the command
        if (sendCommand(cmd, resp))
        {
            if (resp == expect)
            {
                ok = true;
            }
        }

        return ok;
    }
}

