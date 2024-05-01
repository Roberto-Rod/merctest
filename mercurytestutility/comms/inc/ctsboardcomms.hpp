#ifndef CTSBOARDCOMMS_HPP
#define CTSBOARDCOMMS_HPP

#include <QObject>
#include <QFile>

namespace comms
{
    class SerialComms;

    class CtsBoardComms : public QObject
    {
        Q_OBJECT

    public:
        CtsBoardComms(SerialComms* pComms, QObject* parent = 0);

        ~CtsBoardComms();

        SerialComms* serialComms();

        bool isReady() const;

        bool waitReady() const;

        /** @brief Send a command and return string response.
         * @param cmd The command to send.
         * @param resp The returned string.
         * @return true if command was sent successfully and a response
         * was received.
         */
        bool sendCommand(QString cmd, QString& resp) const;

        /** @brief Send a command and return unsigned integer response.
         * An unsigned integer is expected before any spaces, anything after the
         * first space is ignored.
         * @param cmd The command to send.
         * @param resp The returned unsigned integer.
         * @return true if command was sent successfully and the response
         * could be converted to unsigned integer.
         */
        bool sendCommand(QString cmd, uint32_t& resp) const;

        /** @brief Send a command and return integer response.
         * An integer is expected before any spaces, anything after the
         * first space is ignored.
         * @param cmd The command to send.
         * @param resp The returned integer.
         * @return true if command was sent successfully and the response
         * could be converted to integer.
         */
        bool sendCommand(QString cmd, int32_t& resp) const;

        /** @brief Send a command and return 16-bit unsigned integer response.
         * @param cmd The command to send.
         * @param resp The returned unsigned integer.
         * @return true if command was sent successfully and the response
         * could be converted to 16-bit unsigned integer.
         */
        bool sendCommand(QString cmd, uint16_t& resp) const;

        /** @brief Send a command and return 16-bit integer response.
         * @param cmd The command to send.
         * @param resp The returned integer.
         * @return true if command was sent successfully and the response
         * could be converted to 16-bit integer.
         */
        bool sendCommand(QString cmd, int16_t& resp) const;

        /** @brief Send a command and return 8-bit unsigned integer response.
         * @param cmd The command to send.
         * @param resp The returned unsigned integer.
         * @return true if command was sent successfully and the response
         * could be converted to 8-bit unsigned integer.
         */
        bool sendCommand(QString cmd, uint8_t& resp) const;

        /** @brief Send a command and return float response.
         * @param cmd The command to send.
         * @param resp The returned float.
         * @return true if command was sent successfully and the response
         * could be converted to float.
         */
        bool sendCommand(QString cmd, float& resp) const;

        /** @brief Send a command and return boolean response.
         * @param cmd The command to send.
         * @param resp The returned boolean.
         * @return true if command was sent successfully and the response
         * could be converted to boolean.
         */
        bool sendCommand(QString cmd, bool& resp) const;

        /** @brief Send a command and check for a positive response.
         * Expected response as per kResponseOk constant.
         * @param cmd The command to send.
         * @return true if command was sent successfully and OK response
         * was received.
         */
        bool sendCommand(QString cmd) const;

        /** @brief Send a command and check for an expected response.
         * @param cmd The command to send.
         * @param expect The expected response.
         * @return true if command was sent successfully and expected response
         * was received.
         */
        bool sendCommandExpectResponse(QString cmd, QString expect) const;

        static const QString kTerminator; //!< The final part of the CTS prompt.
        static const QString kResponseOk; //!< The string returned by CTS to indicate success.

    private:
        static const uint32_t kMaxRegister = 0x00FFFFFF;    //!< Maximum register address.
        static const int32_t  kReadTimeout = 500;           //!< Serial read timeout (milliseconds).
        static const int32_t  kWaitReadyTimeout = 10000;     //!< Time to wait for CTS board to be ready (milliseconds).

        SerialComms* m_pSerialComms;
    };
}

#endif // CTSBOARDCOMMS_HPP
