#ifndef TESTJIGDETAILS_HPP
#define TESTJIGDETAILS_HPP

#include <QObject>
#include <QMap>
#include <QString>
#include <QList>

namespace comms
{
    class RegisterControl;
}

namespace testjig
{
    enum class JigType
    {
        NO_JIG,
        MERCURY_JIG_1,
        MERCURY_JIG_2,
        MERCURY_JIG_3
    };

    class TestJigDetails : public QObject
    {
        Q_OBJECT

    public:
        TestJigDetails(comms::RegisterControl* pRegs, QObject* parent = 0);

        JigType connectedJigType() const;

        QString connectedJigString() const;

        bool firmwareVersions(QList<QString>& versions) const;

    private:
        static const uint16_t kAddressFpga1DwgNr   = 0x0000;    //!< FPGA 1 Drawing Number Register
        static const uint16_t kAddressFpga1Version = 0x0001;    //!< FPGA 1 Version Code Register
        static const uint16_t kAddressFpga2DwgNr   = 0x4000;    //!< FPGA 2 Drawing Number Register
        static const uint16_t kAddressFpga2Version = 0x4001;    //!< FPGA 2 Version Code Register
        static const uint16_t kAddressFpga3DwgNr   = 0x8000;    //!< FPGA 3 Drawing Number Register
        static const uint16_t kAddressFpga3Version = 0x8001;    //!< FPGA 3 Version Code Register
        static const uint32_t kJig1Fpga1DwgNr      = 56u;       //!< Mercury Test Jig 1 (PCB0078) FPGA 1 Drawing Number
        static const uint32_t kJig1Fpga2DwgNr      = 57u;       //!< Mercury Test Jig 1 (PCB0078) FPGA 2 Drawing Number
        static const uint32_t kJig1Fpga3DwgNr      = 58u;       //!< Mercury Test Jig 1 (PCB0078) FPGA 3 Drawing Number

        QMap<JigType, QString> m_typeMap;                       //!< Map jig types to strings

        comms::RegisterControl* m_pRegs;                        //!< Pointer to the register control object to use
    };
}

#endif // TESTJIGDETAILS_HPP
