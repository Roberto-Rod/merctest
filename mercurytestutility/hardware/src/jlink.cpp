#include "jlink.hpp"
#include "logging.hpp"

#include <QProcess>
#include <QThread>

namespace hardware
{
    Jlink::Jlink(Device device, QObject* parent) :
        QObject(parent),
        m_device(device),
        m_ok(false),
        m_complete(false)
    {
        // Initialise the device ID map
        m_deviceIdMap[Device::CORTEX_M3] = "0x412FC230";
        m_deviceIdMap[Device::CORTEX_M4] = "0x410FC241";
    }

    bool Jlink::checkDeviceId()
    {
        bool retVal = false;

        common::Logging::instance()->log("J-Link: Check Device ID...");

        QString resp;
        if (executeCommand("READSIGNATURE", "Script processing completed", resp))
        {
            QString msg = "J-Link: bootloader & firmware programming OK ";
            common::Logging::instance()->log(msg);
            retVal = true;
        }

        return retVal;
    }

    bool Jlink::flashDevice(const QString& programmingBatchFile)
    {
        bool retVal = false;

        common::Logging::instance()->log("J-Link: programming flash device...");

        QString resp;
        if (executeCommand(programmingBatchFile, "Script processing completed", resp))
        {
            QString msg = "J-Link: flash programming OK ";
            common::Logging::instance()->log(msg);
            retVal = true;
        }

        return retVal;
    }

    bool Jlink::executeCommand(const QString& cmd, const QString& expRespBegin)
    {
        QString resp;
        return executeCommand(cmd, expRespBegin, resp);
    }

    bool Jlink::executeCommand(const QString& programmingBatchFile, const QString& expRespBegin, QString& resp)
    {
        bool retVal = false;

        m_ok = false;
        m_complete = false;
        QProcess process;
        QStringList output;
        QStringList arguments;
        arguments << QString("");

        connect(&process, SIGNAL(error(QProcess::ProcessError)),
                 this,    SLOT(error(QProcess::ProcessError)));

        connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
                 this,    SLOT(finished(int, QProcess::ExitStatus)));

        process.start(programmingBatchFile, arguments);

        uint32_t timeoutCount = 0ul;

        while (!m_complete && (timeoutCount++ < kCommandTimeout))
        {
            QThread::sleep(1ul);
            QCoreApplication::processEvents();
        }

        QString msg = "J-Link: ";

        if (!m_complete)
        {
            msg += "ERROR - timed out waiting for command line operation";
        }
        else if (!m_ok)
        {
            msg += "ERROR - could not execute " + programmingBatchFile + ", ensure SEGGER J-Link is installed and included in PATH environment variable.";
        }
        else
        {
            // Read all of the lines output by the process
            while (process.canReadLine())
            {
                output << QString::fromLatin1(process.readLine());
            }

            // Return the last line to the calling method
            output.removeLast();
            resp = output.last();
            // Check various messages in the output

            // Check the last line for expected response
            if (resp.indexOf(expRespBegin) == -1)
            {
                msg += "script not completed";
            }
            else if (output.indexOf(QRegExp(QString("CPUID register: %1.*").arg(m_deviceIdMap[m_device]))) == -1)
            {
                msg += "device not found";
            }
            else if(checkMessages(output, "Downloading file.+\r\n", "Failed to open file.\r\n", 1))
            {
                msg += "binary not found in scripts directory";
            }
            else if(!checkMessages(output, "Downloading file.+\r\n", "O.K.\r\n", 5))
            {
                msg += "device failed to program";
            }
            else if(!checkMessages(output, "Loading binary file.+\r\n", "Verify successful.\r\n", 5))
            {
                msg += "failed to verify";
            }
            else
            {
                msg += "programming & verification successful";
                retVal = true;
            }
        }  // m_ok && m_complete

        common::Logging::instance()->log(msg);

        return retVal;
   }

    // Check for presence of two messages in an output file and check they
    // are within a certain number of lines of each other

    bool  Jlink::checkMessages(QStringList &stringList, const QString& msg1, const QString& msg2, int lines)
    {
        bool result = true;
        QRegExp rx1(msg1);
        QRegExp rx2(msg2);
        int indx1 = stringList.indexOf(rx1);
        int indx2 = stringList.indexOf(rx2);

        if(indx1 == -1 || indx2 == -1)
        {
            result = false;
        }
        else if (indx2  >  indx1 + lines)
        {
            result = false;
        }
        return result;
    }

    void Jlink::error(QProcess::ProcessError error)
    {
        (void)error;
        m_ok = false;
        m_complete = true;
    }

    void Jlink::finished(int exitCode, QProcess::ExitStatus exitStatus)
    {
        m_ok = (exitStatus == QProcess::ExitStatus::NormalExit && exitCode >= 0);
        m_complete = true;
    }

    bool Jlink::createJlinkScripts()
    {
        bool retVal(true);

        // Get file path
        QString path = common::Settings::instance()->programmingScriptsDirectory() + "/";
        path = QDir::toNativeSeparators(path);

        // Assign  file handles for the 6 script files; 1 DOS batch file and 1 J-Link command
        // script for each of: Source Board bootloader, Source Board firmware, CTS firmware
        QFile fileProgBl(path + "progbl.bat");
        QFile fileProgFw(path + "progfw.bat");
        QFile fileProgCts(path + "progcts.bat");
        QFile fileCmdBl(path + "JLinkCommandFileBL.txt");
        QFile fileCmdFw(path + "JLinkCommandFileFW.txt");
        QFile fileCmdCts(path + "JLinkCommandFileCTS.txt");

        // Create the batch files
        if (fileProgBl.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&fileProgBl);
            out << QString("JLink.exe -device LPC1788 -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1 -CommanderScript %1JLinkCommandFileBL.txt\n").arg(path);
            fileProgBl.close();
        }
        else
        {
            retVal = false;
        }

        if (fileProgFw.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&fileProgFw);
            out << QString("JLink.exe -device LPC1788 -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1 -CommanderScript %1JLinkCommandFileFW.txt\n").arg(path);
            fileProgFw.close();
        }
        else
        {
            retVal = false;
        }

        if (fileProgCts.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&fileProgCts);
            out << QString("JLink.exe -device STM32L476VG -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1 -CommanderScript %1JLinkCommandFileCTS.txt\n").arg(path);
            fileProgCts.close();
        }
        else
        {
            retVal = false;
        }

        // Create the J-Link Commander scripts
        if (fileCmdBl.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&fileCmdBl);
            QString cmd;

            // Create the script file
            cmd += "r\n";                                        // Reset & halt target
            cmd += "erase\n";                                    // Mass erase flash
            cmd += QString("loadbin %1bm.bin, 0\n").arg(path);   // Load the bootloader binary at offset 0
            cmd += QString("verifybin %1bm.bin, 0\n").arg(path); // Verify the flash contents at offset 0
            cmd += "qc\n";                                       // Quit J-Link Commander

            out << cmd;
            fileCmdBl.close();
        }
        else
        {
            retVal = false;
        }

        if (fileCmdFw.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&fileCmdFw);
            QString cmd;

            // Create the script file
            cmd += "r\n";                                               // Reset & halt target
            cmd += QString("loadbin %1ecm.bin, 0x10000\n").arg(path);   // Load the firmware binary at offset 0x10000
            cmd += QString("verifybin %1ecm.bin, 0x10000\n").arg(path); // Verify the flash contents at offset 0x10000
            cmd += "qc\n";                                              // Quit J-Link Commander

            out << cmd;
            fileCmdFw.close();
        }
        else
        {
            retVal = false;
        }

        if (fileCmdCts.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&fileCmdCts);
            QString cmd;

            // Create the script file
            cmd += "r\n";                                            // Reset & halt target
            cmd += "w4 0x40022008 0x45670123\n";                     // Write KEY1 to Flash Key Register
            cmd += "w4 0x40022008 0xCDEF89AB\n";                     // Write KEY2 to Flash Key Register
            cmd += "w4 0x4002200C 0x08192A3B\n";                     // Write OPTKEY1 to Flash Option Key Register
            cmd += "w4 0x4002200C 0x4C5D6E7F\n";                     // Write OPTKEY2 to Flash Option Key Register
            cmd += "w4 0x40022020 0xFFEFF8AA\n";                     // Set boot bank = 0 in option byte
            cmd += "w4 0x40022014 0x00020000\n";                     // Start options modification (OPTSTRT=1 in Flash Control Register)
            cmd += "sleep 250\n";
            cmd += "w4 0x40022014 0x08000000\n";                     // Force option byte re-load (OBL_LAUNCH=1 in Flash Control Register)
            cmd += "sleep 250\n";
            cmd += "r\n";
            cmd += QString("loadbin %1cts_fw.bin, 0\n").arg(path);   // Load the firmware binary at offset 0
            cmd += QString("verifybin %1cts_fw.bin, 0\n").arg(path); // Verify the flash contents at offset 0
            cmd += "g\n";                                            // Start the CPU core
            cmd += "qc\n";                                           // Quit J-Link Commander

            out << cmd;
            fileCmdCts.close();
        }
        else
        {
            retVal = false;
        }

        return retVal;
    }
}
