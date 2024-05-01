#include "registercomms.hpp"
#include "serialcomms.hpp"
#include "settings.hpp"

namespace comms {
    RegisterComms::RegisterComms(SerialComms* pComms, QObject* parent) :
        QObject(parent),
        m_pSerialComms(pComms)
    {
    }

    RegisterComms::~RegisterComms()
    {
    }

    SerialComms* RegisterComms::serialComms()
    {
        return m_pSerialComms;
    }

    bool RegisterComms::sendPacket(const QByteArray& bytesIn, QByteArray& bytesOut) const
    {
        bool retVal = false;

        if (m_pSerialComms != 0)
        {
            if (m_pSerialComms->isPortOpen())
            {
                QByteArray packet(bytesIn);
                appendChecksum(packet);

                // Send the tx packet
                if (m_pSerialComms->write(packet))
                {
                    // Read the rx packet
                    if (m_pSerialComms->read(bytesOut, 7u))
                    {
                        // Check the checksum on the returned data (method also chops the checksum bytes off the array)
                        retVal = testChecksum(bytesOut);
                    }
                }
            }
        }

        return retVal;
    }

    bool RegisterComms::writeRegister(uint16_t addr, uint32_t data) const
    {
        bool retVal = false;

        QByteArray txPacket, rxPacket;

        // Generate packet contents exclduing the checksum
        txPacket.append(static_cast<char>(FpgaCommand::CMD_WRITE));
        txPacket.append(static_cast<char>((addr >> 8)  & 0x00FF));
        txPacket.append(static_cast<char>((addr)       & 0x00FF));
        txPacket.append(static_cast<char>((data >> 24) & 0x000000FF));
        txPacket.append(static_cast<char>((data >> 16) & 0x000000FF));
        txPacket.append(static_cast<char>((data >> 8)  & 0x000000FF));
        txPacket.append(static_cast<char>((data)       & 0x000000FF));

        if (sendPacket(txPacket, rxPacket))
        {
            if (rxPacket.length() == 5)
            {
                if (rxPacket.at(0) == static_cast<uint8_t>(FpgaResponse::RESP_OK))
                {
                    retVal = true;
                }
            }
        }

        return retVal;
    }

    bool RegisterComms::readRegister(uint16_t addr, uint32_t& data) const
    {
        bool retVal = false;

        QByteArray txPacket, rxPacket;

        // Generate packet contents exclduing the checksum
        txPacket.append(static_cast<char>(FpgaCommand::CMD_READ));
        txPacket.append(static_cast<char>((addr >> 8) & 0x00FF));
        txPacket.append(static_cast<char>((addr)      & 0x00FF));

        if (sendPacket(txPacket, rxPacket))
        {
            if (rxPacket.length() == 5)
            {
                if (rxPacket.at(0) == static_cast<uint8_t>(FpgaResponse::RESP_OK))
                {
                    data  = (static_cast<uint32_t>(rxPacket.at(1)) << 24) & 0xFF000000;
                    data |= (static_cast<uint32_t>(rxPacket.at(2)) << 16) & 0x00FF0000;
                    data |= (static_cast<uint32_t>(rxPacket.at(3)) << 8)  & 0x0000FF00;
                    data |= (static_cast<uint32_t>(rxPacket.at(4)))       & 0x000000FF;

                    retVal = true;
                }
            }
        }

        return retVal;
    }

    void RegisterComms::appendChecksum(QByteArray& bytes)
    {
        uint16_t cksum = 0;

        // Sum (modulo-16) the packet contents
        for (int i = 0; i < bytes.length(); i++)
        {
            cksum += static_cast<uint16_t>(bytes.at(i)) & 0x00FF;
        }

        // Take two's complement of sum
        cksum = ~cksum + 1u;

        // Put the checksum at the end of the packet
        bytes.append(static_cast<int8_t>((cksum >> 8) & 0x00FF));
        bytes.append(static_cast<int8_t>(cksum & 0x00FF));
    }

    bool RegisterComms::testChecksum(QByteArray& bytes)
    {
        bool retVal = false;

        // Make sure there are more than 2 bytes in the packet before proceeding
        if (bytes.length() > 2)
        {
            uint16_t cksum = 0;

            // The final two bytes are the checksum as a 16-bit unsigned integer.
            // Add all the bytes together in the packet then add in the 16-bit checksum
            for (int i = 0; i < bytes.length() - 2; i++)
            {
                cksum += static_cast<uint16_t>(bytes.at(i)) & 0x00FF;
            }

            cksum += (static_cast<uint16_t>(bytes.at(bytes.length() - 2)) << 8) & 0xFF00;
            cksum += (static_cast<uint16_t>(bytes.at(bytes.length() - 1)))      & 0x00FF;

            retVal = (cksum == 0);

            // Chop the two checksum bytes off
            bytes.chop(2);
        }

        return retVal;
    }
}
