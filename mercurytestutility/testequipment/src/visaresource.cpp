#include "visaresource.hpp"
#include <stdint.h>
#include <QThread>
#include <QDebug>

namespace te
{
    VisaResource::VisaResource() :
        m_resourceName(QObject::tr("<undefined>")),
        m_sessionOpen(false),
        m_viDefaultRm(VI_NULL),
        m_viSession(VI_NULL),
        m_status(-1)
    {
        setTimeout(kDefaultTimeout);
    }

    VisaResource::~VisaResource()
    {
        closeSession();

        if (m_viDefaultRm != VI_NULL)
        {
            viClose(m_viDefaultRm);
        }
    }

    bool VisaResource::isSessionOpen() const
    {
        return m_sessionOpen;
    }

    bool VisaResource::openSession()
    {
        bool retVal = false;

        // Open default VISA session
        if (m_viDefaultRm == VI_NULL)
        {
            m_status = viOpenDefaultRM (&m_viDefaultRm);

            if (m_status != 0)
            {
                m_viDefaultRm = VI_NULL;
            }
        }

        if (m_viDefaultRm != VI_NULL)
        {
            if (m_viSession != VI_NULL)
            {
                closeSession();
            }

            m_status = viOpen(m_viDefaultRm, (ViRsrc)m_resourceName.toUtf8().data(), VI_NULL, VI_NULL, &m_viSession);

            if (m_status == 0)
            {
                // Disable Visa events (exceptions) for this session
                m_status = viDisableEvent(m_viSession, VI_ALL_ENABLED_EVENTS, VI_ALL_MECH);

                m_sessionOpen = true;
                retVal = true;
            }
        }

        return retVal;
    }

    void VisaResource::closeSession()
    {
        if (m_viSession != VI_NULL)
        {
            viClose(m_viSession);
            m_viSession = VI_NULL;
        }

        m_sessionOpen = false;
    }

    bool VisaResource::setTimeout(int32_t milliseconds) const
    {
        bool retVal = false;

        if (m_viSession != VI_NULL)
        {
            retVal = viSetAttribute(m_viSession, VI_ATTR_TMO_VALUE, milliseconds);
        }

        return retVal;
    }

    bool VisaResource::sendCommand(const QString& command) const
    {
        bool retVal = false;

        if (m_sessionOpen)
        {
            // Take a local copy of the command and append a new-line character if there
            // isn't one at the end of the command.
            QString cmd(command);

            if (cmd.at(cmd.length() - 1) != '\n')
            {
                cmd.append(QLatin1Char('\n'));
            }

            if(viPrintf(m_viSession, ViString(cmd.toUtf8().constData())) == 0)
            {
                retVal = true;
            }
        }

        return retVal;
    }

    bool VisaResource::sendCommand(const QString& command, QString& response) const
    {
        bool retVal = false;

        if (sendCommand(command))
        {
            char buf[kResponseBufferSize];
            memset(buf, 0, sizeof(buf));

            uint32_t readCount;

            if(viBufRead(m_viSession, ViPBuf(buf), kResponseBufferSize - 1, (ViPUInt32)(&readCount)) == 0)
            {
                response = QString::fromUtf8(buf);

                // Remove any newline characters from the response
                response.remove(QRegExp("[\r\n]"));

                // Remove one or more spaces from the end of the response
                response.remove(QRegExp(" +$"));

                retVal = true;
            }
        }

        return retVal;
    }

    bool VisaResource::checkResponse(const QString& command, const QString& expected) const
    {
        bool retVal = false;

        QString response;
        if (sendCommand(command, response))
        {
            QString expectedTrimmed(expected);
            expectedTrimmed.remove("\n");
            expectedTrimmed.remove(QRegExp(" +$"));

            retVal = (expectedTrimmed == response);
        }

        return retVal;
    }

    bool VisaResource::checkResponse(const QString& command, const QChar& expected) const
    {
        bool retVal = false;

        QString response;
        if (sendCommand(command, response) && response.at(0) == expected)
        {
            retVal = true;
        }

        return retVal;
    }

    bool VisaResource::systemStatus() const
    {
        return checkResponse("SYST:ERR?", '0');
    }

    bool VisaResource::waitStatOperCond() const
    {
        bool retVal = false;

        if (m_sessionOpen)
        {
            for(int i = 0; (i < kWaitCompletionAttempts) && !retVal; i++)
            {
                // Sleep for 100 milliseconds
                QThread::msleep(100);

                retVal = checkResponse("STAT:OPER:COND?", '0');
            }
        }

        return retVal;
    }

    bool VisaResource::waitOpc() const
    {
        bool retVal = false;

        QString response;
        if (sendCommand("*OPC?", response))
        {
            // Check that the response was "1". Perform integer
            // conversion as this could be returned as "1" or "+1"
            bool ok;
            int32_t i = response.toInt(&ok);

            retVal = (ok && (i == 1));
        }

        return retVal;
    }

    bool VisaResource::details(QString &details) const
    {
        bool retVal = false;

        if (m_sessionOpen)
        {
            viPrintf(m_viSession, ViString("*IDN?\n"));

            char buf[kResponseBufferSize];
            memset(buf, 0, sizeof(buf));

            ViUInt32 retCnt;

           if (viBufRead(m_viSession, (ViPBuf)buf, kResponseBufferSize, &retCnt) == 0)
           //  if (viBufRead(m_viSession, (ViPBuf)buf, 50, &retCnt) == 0)
            {
                details = QString(buf);
                retVal = true;
            }
        }

        return retVal;
    }

    void VisaResource::setResourceName(const QString &resourceName)
    {
        // If the resource name has been updated then re-open the session. If
        // it hasn't been updated then no need to do anything.
        if (resourceName != m_resourceName)
        {
            m_resourceName = resourceName;
            openSession();
        }
    }

    QString VisaResource::errorString()
    {
        // Default to using the hexadecimal error code
        QString err = QString("0x%1").arg(m_status, 8, 16, QLatin1Char('0'));

        if (m_status == 0)
        {
            err = "Success";
        }
        else if (m_status == VI_SUCCESS_DEV_NPRESENT)
        {
            err = "Device Not Present";
        }
        else if (m_status == VI_ERROR_SYSTEM_ERROR)
        {
            err = "System Error";
        }
        else if (m_status == VI_ERROR_INV_OBJECT)
        {
            err = "Invalid Object";
        }
        else if (m_status == VI_ERROR_RSRC_LOCKED)
        {
            err = "Resource Locked";
        }
        else if (m_status == VI_ERROR_INV_EXPR)
        {
            err = "Invalid Expression";
        }
        else if (m_status == VI_ERROR_RSRC_NFOUND)
        {
            err = "Resource Not Found";
        }
        else if (m_status == VI_ERROR_INV_RSRC_NAME)
        {
            err = "Invalid Resource Name";
        }

        return err;
    }
}
