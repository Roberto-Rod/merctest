#ifndef XMODEMWRAPPER_HPP
#define XMODEMWRAPPER_HPP

#ifdef __cplusplus

#include "serialcomms.hpp"

#include <QObject>
#include <QFile>

namespace comms
{
    class XModemWrapper : public QObject
    {
        Q_OBJECT

    public:
        static XModemWrapper* instance();

        void setSerialComms(SerialComms* pSerialComms);

        bool sendFile(QFile& file);

        char inByte(uint16_t timeout, bool& timedOut);

        void outByte(char c);

        void packetSent();

    signals:
        void progress(int bytesSent);

    protected:
        XModemWrapper(QObject* parent = 0);

    private:
        static const int32_t kPacketSize = 128ul;  //!< Packet size in bytes (fixed in xmodem.c implementation)

        static XModemWrapper* m_pXmodemWrapper;

        SerialComms* m_pSerialComms;

        int32_t m_bytesSent;
    };
}

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

    int _inbyte(unsigned short timeout); // msec timeout

    void _outbyte(int c);

    int xmodemTransmit(unsigned char *src, int srcsz);

    void packetSent();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // XMODEMWRAPPER_HPP
