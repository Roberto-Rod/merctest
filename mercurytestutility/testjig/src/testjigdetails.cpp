#include "testjigdetails.hpp"
#include "registercontrol.hpp"

namespace testjig
{
    TestJigDetails::TestJigDetails(comms::RegisterControl* pRegs, QObject* parent) :
        QObject(parent),
        m_pRegs(pRegs)
    {
        m_typeMap[JigType::NO_JIG]        = "<No Known Test Jig Connected>";
        m_typeMap[JigType::MERCURY_JIG_1] = "Mercury Test Jig 1";
        m_typeMap[JigType::MERCURY_JIG_2] = "Mercury Test Jig 2";
        m_typeMap[JigType::MERCURY_JIG_3] = "Mercury Test Jig 3";
    }

    JigType TestJigDetails::connectedJigType() const
    {
        JigType type = JigType::NO_JIG;

        if (m_pRegs != 0)
        {
            // Attempt read of 3 drawing number locations
            uint32_t data[3];

            // If a read fails then ensure data is reset to 0 for subsequent testing of
            // values against drawing numbers
            if (!m_pRegs->readRegister(kAddressFpga1DwgNr, data[0])) { data[0] = 0u; }
            if (!m_pRegs->readRegister(kAddressFpga2DwgNr, data[1])) { data[1] = 0u; }
            if (!m_pRegs->readRegister(kAddressFpga3DwgNr, data[2])) { data[2] = 0u; }

            if ((data[0] == kJig1Fpga1DwgNr) &&
                 (data[1] == kJig1Fpga2DwgNr) &&
                 (data[2] == kJig1Fpga3DwgNr))
            {
                type = JigType::MERCURY_JIG_1;
            }
        }

        return type;
    }

    QString TestJigDetails::connectedJigString() const
    {
        // Map the enumerated type to string
        return m_typeMap[connectedJigType()];
    }

    bool TestJigDetails::firmwareVersions(QList<QString>& versions) const
    {
        bool retVal = false;

        if (m_pRegs != 0)
        {
            uint32_t data[3];
            retVal = true;

            retVal &= m_pRegs->readRegister(kAddressFpga1Version, data[0]);
            retVal &= m_pRegs->readRegister(kAddressFpga2Version, data[1]);
            retVal &= m_pRegs->readRegister(kAddressFpga3Version, data[2]);

            if (retVal)
            {
                versions.clear();

                for (int i = 0; i < 3; i++)
                {
                    uint8_t  major = static_cast<uint8_t> ((data[i] >> 24) & 0x000000FF);
                    uint8_t  minor = static_cast<uint8_t> ((data[i] >> 16) & 0x000000FF);
                    uint16_t build = static_cast<uint16_t>((data[i]) & 0x0000FFFF);

                    versions.append(QString("%1.%2.%3").arg(major).arg(minor).arg(build));
                }
            }
        }

        return retVal;
    }
}
