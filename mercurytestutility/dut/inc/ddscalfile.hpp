#ifndef DDSCALFILE_HPP
#define DDSCALFILE_HPP

#include <QFile>
#include <QTextStream>

#include <cstdint>

namespace dut
{
    class DdsCalFile : public QFile
    {
        Q_OBJECT

    public:
        DdsCalFile(QString dutSerial = "000000", double powerLevel = 0.0, QObject* parent = 0);

        bool open(QString directory);
        bool openRo(QString directory);
        bool endOfStream(QTextStream fileStr);
        bool removeHeader();
        bool getFreqAndAsf(uint16_t  &freq, uint16_t& asf);

        bool addPoint(uint64_t freq, uint32_t asf);

    private:
        static const uint8_t    kFileVersionMajor = 0u;
        static const uint8_t    kFileVersionMinor = 1u;

        QString m_dutSerial;
        double m_powerLevel;
        QTextStream m_calStream;
    };
}

#endif // DDSCALFILE_HPP
