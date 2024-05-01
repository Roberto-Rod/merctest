#ifndef SOURCEBOARDCOMMS_HPP
#define SOURCEBOARDCOMMS_HPP

#include "registercontrol.hpp"

#include <QObject>
#include <QFile>

namespace comms
{
    class SerialComms;

    enum class FileMethod
    {
        BOOTLOADER_XMODEM,
        MAIN_FW_FDOWNLOAD
    };

    class SourceBoardComms : public QObject, public RegisterControl
    {
        Q_OBJECT

    public:
        SourceBoardComms(SerialComms* pComms, QObject* parent = 0);

        ~SourceBoardComms();

        SerialComms* serialComms();

        bool isReady() const;

        bool waitReady() const;

        bool readRegister(uint32_t addr, uint32_t& data) const;

        bool writeRegister(uint32_t addr, uint32_t data) const;

        bool readRegister(uint16_t addr, uint32_t& data) const;

        bool writeRegister(uint16_t addr, uint32_t data) const;

        bool sendCommand(QString cmd, QString& resp, QString term = ">") const;

        bool sendCommand(QString cmd, QString term = ">") const;

        bool breakInBootloader() const;

        bool sendFile(QFile& file, QString destName, FileMethod method = FileMethod::MAIN_FW_FDOWNLOAD) const;

        static const QString  kBootloaderTerminator;
        static const QString  kStandardTerminator;

    private:
        static const uint32_t kMaxRegister = 0x00FFFFFF;    //!< Maximum register address
        static const int32_t  kReadTimeout = 1000;          //!< Serial read timeout (milliseconds)
        static const int32_t  kReadTimeoutBreakIn = 100;    //!< Serial read timeout (milliseconds) for breaking into bootloader
        static const int32_t  kReadTimeoutSendFile = 15000; //!< Serial read timeout (milliseconds) for file sending commands
        static const int32_t  kWaitReadyTimeout = 30000;    //!< Time to wait for source board to be ready
        static const int32_t  kFileChunkSize = 128;         //!< Number of bytes per chunk of file to send
        static const uint32_t kFileChunkDelay = 100u;       //!< Number of milliseconds to wait between sending file chunks
        static const uint8_t  kBreakinAttempts = 20u;       //!< Number of attempt to break-in by sending ctrl-c

        uint32_t m_regOffset;                               //!< The offset to be used when 16-bit address methods are called

        SerialComms* m_pSerialComms;

        bool sendFileBootloaderXmodem(QFile& file, QString destName) const;

        bool sendFileMainFwFDownload(QFile& file, QString destName) const;
    };

}

#endif // SOURCEBOARDCOMMS_HPP
