#ifndef TESTJIGCOMMS_HPP
#define TESTJIGCOMMS_HPP

#include "registercontrol.hpp"

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QMap>
#include <QTimer>

namespace comms {
    class SerialComms;

    class RegisterComms : public QObject, public RegisterControl
    {
        Q_OBJECT

    public:

        enum FpgaCommand
        {
            CMD_WRITE = 0,
            CMD_READ
        };

        enum FpgaResponse
        {
            RESP_OK = 0,
            RESP_ERR_CKSUM,
            RESP_ERR_CMD,
            RESP_ERR_ADDR
        };

        RegisterComms(SerialComms* pComms, QObject* parent = 0);

        ~RegisterComms();

        SerialComms* serialComms();

        bool readRegister(uint16_t addr, uint32_t& data) const;

        bool writeRegister(uint16_t addr, uint32_t data) const;

    private:
        SerialComms* m_pSerialComms;

        bool sendPacket(const QByteArray& bytesIn , QByteArray& bytesOut) const;

        static void appendChecksum(QByteArray& bytes);

        static bool testChecksum(QByteArray& bytes);
    };
}

#endif // TESTJIGCOMMS_HPP
