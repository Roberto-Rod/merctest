#include "sourceboardcomms.hpp"
#include "serialcomms.hpp"
#include "xmodemwrapper.hpp"

#include <QTextStream>
#include <QThread>
#include <QProgressDialog>

namespace comms
{
    const QString SourceBoardComms::kBootloaderTerminator = QString("ecm: ");
    const QString SourceBoardComms::kStandardTerminator   = QString(">");

    SourceBoardComms::SourceBoardComms(SerialComms* pComms, QObject* parent) :
        QObject(parent),
        m_regOffset(0u),
        m_pSerialComms(pComms)
    {
        // Set the read timeout
        pComms->setReadTimeout(kReadTimeout);
    }

    SourceBoardComms::~SourceBoardComms()
    {
    }

    SerialComms* SourceBoardComms::serialComms()
    {
        return m_pSerialComms;
    }

    bool SourceBoardComms::isReady() const
    {
        bool retVal = false;

        if (m_pSerialComms && m_pSerialComms->isPortOpen())
        {
            // Send an empty command (just CR) and check for a valid response
            // (the sendCommand method checks for the '>' character being returned)
            retVal = sendCommand("", kStandardTerminator);

            // After sending CR, flush the buffer as the Source Board could be spitting KBs of
            // help text out of the CLI
            m_pSerialComms->flush();
        }

        return retVal;
    }

    bool SourceBoardComms::waitReady() const
    {
        bool retVal = false;

        if (m_pSerialComms && m_pSerialComms->isPortOpen())
        {
            // Calculate retry count based on wait time required and the read timeout on each try.
            // The extra time spent going round the loop and sending the CRLFs is neglected.
            int32_t retryCount = kWaitReadyTimeout / kReadTimeout;

            QProgressDialog progress("Initialising Source Board<BR>Timeout at 100%...", "Cancel", 0, retryCount - 1);
            progress.setWindowModality(Qt::ApplicationModal);
            progress.setFixedSize(progress.sizeHint());

            for (int i = 0; (i < retryCount) && !retVal && !progress.wasCanceled(); i++)
            {
                retVal = isReady();
                progress.setValue(i);
            }

            // Was the progress bar cancelled?
            retVal &= !progress.wasCanceled();
        }

        return retVal;
    }

    bool SourceBoardComms::readRegister(uint32_t addr, uint32_t& data) const
    {
        bool retVal = false;

        if (addr <= kMaxRegister)
        {
            // Construct the "fpga write" command
            QString cmd = QString("fpga read 0x%1").arg(addr, 0, 16);
            QString resp;

            // Send the command and get the response
            if (sendCommand(cmd, resp, ">"))
            {
                // Parse the response - look for "Value 0x" which precedes the response value
                // - note that this appears after the echoed command and a CRLF
                QString substr("Value 0x");

                int valIdx = resp.indexOf(substr);

                // Was it there?
                if (valIdx != -1)
                {
                    // Take the end of the string which contains the hex value, the -2 causes
                    // "0x" to be included in the string
                    resp = resp.mid(valIdx + substr.length() - 2);

                    // Convert it to a text stream & then stream into the data variable to perform
                    // coversion to uint32_t
                    QTextStream stream(&resp);
                    stream >> data;

                    // Test the stream status to see if conversion worked
                    if (stream.status() == QTextStream::Ok)
                    {
                        retVal = true;
                    }
                }
            }
        }

        return retVal;
    }

    bool SourceBoardComms::writeRegister(uint32_t addr, uint32_t data) const
    {
        bool retVal = false;

        if (addr <= kMaxRegister)
        {
            // Construct the "fpga write" command
            QString cmd = QString("fpga write 0x%1 0x%2").arg(addr, 0, 16).arg(data, 0, 16);

            // Send the command
            if (sendCommand(cmd,  comms::SourceBoardComms::kStandardTerminator))
            {
                retVal = true;
            }
        }

        return retVal;
    }

    bool SourceBoardComms::readRegister(uint16_t addr, uint32_t& data) const
    {
        return readRegister((m_regOffset + addr), data);
    }

    bool SourceBoardComms::writeRegister(uint16_t addr, uint32_t data) const
    {
        return writeRegister((m_regOffset + addr), data);
    }

    bool SourceBoardComms::sendCommand(QString cmd, QString& resp, QString term) const
    {
        bool retVal = false;

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
                if (m_pSerialComms->read(rxBytes, term))
                {
                    // Convert the response byte array to a string
                    resp = QString::fromUtf8(rxBytes);

                    // Remove the echoed command
                    resp.remove(cmd);

                    // Remove any CR/LF characters and leading/trailing whitespace from the string
                    resp.remove(QRegExp("[\\r\\n]"));
                    resp = resp.trimmed();

                    retVal = true;
                }
            }
        }

        return retVal;
    }

    bool SourceBoardComms::sendCommand(QString cmd, QString term) const
    {
        QString resp;

        // Send the command
        return sendCommand(cmd, resp, term);
    }

    bool SourceBoardComms::breakInBootloader() const
    {
        bool retVal = false;

        // Set the (shorter) read timeout to attempt bootloader "break-in"
        if (m_pSerialComms)
        {
            m_pSerialComms->setReadTimeout(kReadTimeoutBreakIn);
        }

        // Attempt the break in - just keep sending ctrl-c and checking for the ": " response
        // prompt that comes back
        for (uint8_t i = 0; (i < kBreakinAttempts) && !retVal; i++)
        {
            QString resp;
            if (sendCommand(QString(QChar(0x03)), resp, kBootloaderTerminator))
            {
                // We are expecting a "?" back (due to the \r sent by sendCommand) - once
                // this is returned we know that we have the bootloader command prompt
                if (resp == "?")
                {
                    retVal = true;
                }
            }
        }

        // Return to standard timeout
        if (m_pSerialComms)
        {
            m_pSerialComms->setReadTimeout(kReadTimeout);
        }

        return retVal;
    }

    bool SourceBoardComms::sendFile(QFile& file, QString destName, comms::FileMethod method) const
    {
        bool retVal = false;

        // Open the file for reading - not in text mode as we don't want newline characters messed with
        if (file.open(QIODevice::ReadOnly))
        {
            file.reset();

            // Whilst performing file operations, increase the read timeout to 2s to
            // allow the DUT more time to process commands (delete command can take a
            // long time)
            if (m_pSerialComms)
            {
                m_pSerialComms->setReadTimeout(kReadTimeoutSendFile);
            }

            if (method == comms::FileMethod::BOOTLOADER_XMODEM)
            {
                retVal = sendFileBootloaderXmodem(file, destName);
            }
            else if (method == comms::FileMethod::MAIN_FW_FDOWNLOAD)
            {
                retVal = sendFileMainFwFDownload(file, destName);
            }

            // Return the read timeout to standard
            if (m_pSerialComms)
            {
                m_pSerialComms->setReadTimeout(kReadTimeout);
            }
        }

        return retVal;
    }

    bool SourceBoardComms::sendFileBootloaderXmodem(QFile& file, QString destName) const
    {
        bool retVal = false;

        // Remove destination file it it already exists
        if (sendCommand(QString("fd %1").arg(destName), kBootloaderTerminator))
        {
            int32_t fileSize = file.bytesAvailable();

            // Form the fr command
            QString cmd = QString("fr %1 %2").arg(destName).arg(fileSize);

            // Send the command and wait for the expected response
            if (sendCommand(cmd, "Waiting to receive file by xmodem(crc)\r\n"))
            {
                // Display a progress dialog

                QProgressDialog progress("Sending file...", "Cancel", 0, fileSize);
                progress.setWindowModality(Qt::ApplicationModal);
                progress.setValue(0);

                connect(XModemWrapper::instance(), SIGNAL(progress(int)),
                                        &progress, SLOT(setValue(int)));

                // Set the serial comms pointer in the xmodem wrapper instance
                XModemWrapper::instance()->setSerialComms(m_pSerialComms);

                // Now send the data
                retVal = XModemWrapper::instance()->sendFile(file);
            }
        }

        return retVal;
    }

    bool SourceBoardComms::sendFileMainFwFDownload(QFile& file, QString destName) const
    {
        bool retVal = false;

        // Remove destination file it it already exists
        if (sendCommand(QString("rm %1").arg(destName)))
        {
            // Read all the source file bytes into memory
            QByteArray bytes = file.readAll();

            // Form the fdownload command
            QString cmd = QString("fdownload %1 %2").arg(destName)
                                                      .arg(bytes.length());

            // Send the command and wait for the expected response
            if (sendCommand(cmd, "Waiting for data ...\r\n"))
            {
                retVal = true;

                // Display a progress dialog
                int32_t fileSize = bytes.length();
                QProgressDialog progress("Sending file...", "Cancel", 0, fileSize);
                progress.setWindowModality(Qt::ApplicationModal);

                // Now send all the bytes in the file
                while (bytes.length() > 0)
                {
                    progress.setValue(fileSize - bytes.length());

                    // Take a chunk from the start of the byte array the 'left' method
                    // will get what's left in the array if there isn't an entire chunk
                    QByteArray chunk = bytes.left(kFileChunkSize);
                    bytes.remove(0, chunk.length());

                    // Write this chunk out
                    retVal &= (m_pSerialComms && m_pSerialComms->write(chunk, true));

                    // Wait for the "inter-chunk" delay
                    if (bytes.length() > 0)
                    {
                        QThread::msleep(kFileChunkDelay);
                    }
                }

                progress.setValue(fileSize);

                // Check for the confirmation
                if (retVal)
                {
                    retVal = (m_pSerialComms && m_pSerialComms->read(bytes, "Download Complete\r\n"));
                }
            }
        }

        return retVal;
    }
}
