#include "flashmagic.hpp"
#include "logging.hpp"

#include <QProcess>
#include <QThread>

namespace hardware
{
    FlashMagic::FlashMagic(const QString& portName, Device device, QObject* parent) :
        QObject(parent),
        m_portName(portName),
        m_device(device),
        m_ok(false),
        m_complete(false)
    {
        // Remove the COM prefix from the port name
        m_portName.remove("COM");

        // Initialise the device name map
        m_deviceNameMap[Device::LPC1778] = "LPC1778";
        m_deviceNameMap[Device::LPC1788] = "LPC1788";

        // Initialise the device ID map
        m_deviceIdMap[Device::LPC1778] = 0x27193F47;
        m_deviceIdMap[Device::LPC1788] = 0x281D3F47;
    }

    bool FlashMagic::checkDeviceId()
    {
        bool retVal = false;

        uint32_t deviceId = 0ul;

        common::Logging::instance()->log("FlashMagic: Check Device ID...");

        QString resp;
        if (executeCommand("READSIGNATURE", "Device ID", resp))
        {
            QString msg = "FlashMagic: ";

            bool ok = false;
            deviceId = resp.remove("Device ID: ").toUInt(&ok);
            if (!ok)
            {
                msg += "ERROR - failed to parse Device ID";
            }
            else
            {
                retVal = (deviceId == m_deviceIdMap[m_device]);

                if (retVal)
                {
                    msg += "Got expected ID (0x" + QString::number(deviceId, 16) + ")";
                }
                else
                {
                    msg += "ERROR - unexpected ID "
                           "(expected 0x" + QString::number(m_deviceIdMap[m_device], 16) + ", "
                           "got 0x" + QString::number(deviceId, 16) + ")";
                }
            }

            common::Logging::instance()->log(msg);
        }

        return retVal;
    }

    bool FlashMagic::flashDevice(const QString& hexFileName)
    {
        QString nativeFileName = QDir::toNativeSeparators(hexFileName);

        return (eraseSectors(nativeFileName) &&
                 programFlash(nativeFileName) &&
                 verifyFlash(nativeFileName));
    }

    bool FlashMagic::executeCommand(const QString& cmd, const QString& expRespBegin)
    {
        QString resp;
        return executeCommand(cmd, expRespBegin, resp);
    }

    bool FlashMagic::executeCommand(const QString& cmd, const QString& expRespBegin, QString& resp)
    {
        bool retVal = false;

        m_ok = false;
        m_complete = false;

        // Configure the FlashMagic command
        QString program = "FM.exe";
        QStringList arguments;
        arguments << QString("COM(%1, 57600)").arg(m_portName)
                  << QString("DEVICE(%1, 0.000000, 0)").arg(m_deviceNameMap[m_device])
                  << cmd;

        QProcess process;

        connect(&process, SIGNAL(error(QProcess::ProcessError)),
                 this,     SLOT(error(QProcess::ProcessError)));

        connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
                 this,     SLOT(finished(int, QProcess::ExitStatus)));

        process.start(program, arguments);

        uint32_t timeoutCount = 0ul;

        while (!m_complete && (timeoutCount++ < kCommandTimeout))
        {
            QThread::sleep(1ul);
            QCoreApplication::processEvents();
        }

        QString msg = "FlashMagic: ";

        if (!m_complete)
        {
            msg += "ERROR - timed out waiting for command line operation";
        }
        else if (m_ok)
        {
            // Read all of the lines output by the process
            QStringList output;
            while (process.canReadLine())
            {
                output << QString::fromLatin1(process.readLine());
            }

            // Return the last line to the calling method
            resp = output.last();

            // Check the last line for expected response
            if (resp.indexOf(expRespBegin) == 0)
            {
                retVal = true;
                msg += "OK";
            }
            // Not expected - check for known errors
            else if (resp.indexOf("Verify failed:") == 0)
            {
               msg += "ERROR - verify failed";
            }
            else if (resp.indexOf("Connection failed:") == 0)
            {
               msg += "ERROR - failed to connect to DUT";
            }
            // Unknown error
            else
            {
                msg += "ERROR - FlashMagic reported\"" + resp + "\"";
            }
        }
        else
        {
           msg += "ERROR - could not execute " + program + ", ensure FlashMagic is installed and included in PATH environment variable.";
        }

        common::Logging::instance()->log(msg);

        return retVal;
    }

    bool FlashMagic::eraseSectors(const QString& hexFileName)
    {
        common::Logging::instance()->log("FlashMagic: Erase Flash sectors used by hex file...");
        QString cmd = "ERASEUSED(" + hexFileName + ", PROTECTISP)";
        return executeCommand(cmd, "Erase complete");
    }

    bool FlashMagic::programFlash(const QString& hexFileName)
    {
        common::Logging::instance()->log("FlashMagic: Program Flash...");
        QString cmd = "HEXFILE(" + hexFileName + ", NOCHECKSUMS, NOFILL, PROTECTISP)";
        return executeCommand(cmd, "Hex file programming complete");
    }

    bool FlashMagic::verifyFlash(const QString& hexFileName)
    {
        common::Logging::instance()->log("FlashMagic: Verify Flash...");
        QString cmd = "VERIFY(" + hexFileName + ", NOCHECKSUMS)";
        return executeCommand(cmd, "Verify passed");
    }

    void FlashMagic::error(QProcess::ProcessError error)
    {
        (void)error;
        m_ok = false;
        m_complete = true;
    }

    void FlashMagic::finished(int exitCode, QProcess::ExitStatus exitStatus)
    {
        m_ok = (exitStatus == QProcess::ExitStatus::NormalExit && exitCode >= 0);
        m_complete = true;
    }
}
