#include "ddscalfile.hpp"

#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDir>

namespace dut
{
    DdsCalFile::DdsCalFile(QString dutSerial, double powerLevel, QObject* parent) :
        QFile(parent),
        m_dutSerial(dutSerial),
        m_powerLevel(powerLevel)
    {
    }

    bool DdsCalFile::openRo(QString directory)
    {
        bool retVal = false;

        // Close file if it is currently open
        //this->QFile::close();

        QDir dir(directory);
        dir.setSorting(QDir::Time);

        // Form the file name
        QString fileName = "pcb0044_" + m_dutSerial + "_dds_cal_";
        fileName += QString::number(m_powerLevel, 'f', 1) + "dBm_";
        fileName += "*";
        fileName += ".csv";

        QStringList fileNameFilters;
        fileNameFilters << fileName;

        dir.setNameFilters(fileNameFilters);
        QFileInfoList list = dir.entryInfoList();
        QFileInfo fileInfo = list.at(0);
        QString afile = directory + "/" + fileInfo.fileName();
        this->QFile::setFileName(afile);

        // Open the file for reading/writing and overwrite existing files
        if (! this->QFile::open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // Return header;
            return retVal;
        }

        this->m_calStream.setDevice(this);
        retVal = true;
        return retVal;
    }

    bool DdsCalFile::removeHeader()
    {
        QString s1, s2, s3, s4;
        s1 = this->m_calStream.readLine();
        s2 = this->m_calStream.readLine();
        s3 = this->m_calStream.readLine();
        s4 = this->m_calStream.readLine();
        return true;
    }

    bool  DdsCalFile::endOfStream(QTextStream fileStr)
    {
        bool endOfStream = true;
        if (fileStr.atEnd() == 0)
        {
             endOfStream = false;
        }
        return endOfStream;
    }

    bool DdsCalFile::getFreqAndAsf(uint16_t& freq, uint16_t& asf)
    {
         bool  result = false;
         QString fs, asfs;
         QRegExp freqLine("^([^,]+),\\s?([^$]+)");

          QString myString = this->m_calStream.readLine();

          if(freqLine.indexIn(myString) != -1)
          {
              result = true;
              fs = freqLine.cap(1);
              asfs = freqLine.cap(2);

              freq = fs.toInt()/1e6;
              asf = asfs.toInt();
          }
          return result;
    }

    bool DdsCalFile::open(QString directory)
    {
        bool retVal = false;

        // Close file if it is currently open
        close();

        // Form the file name
        QString fileName = directory + "/pcb0044_" + m_dutSerial + "_dds_cal_";
        fileName += QString::number(m_powerLevel, 'f', 1) + "dBm_";
        fileName += QDate::currentDate().toString("yyyy-MM-dd");
        fileName += "_" + QTime::currentTime().toString("hh-mm-ss");
        fileName += ".csv";

        setFileName(fileName);

        // Open the file for reading/writing and overwrite existing files

        if (QFile::open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        {
            // Write the header
            QTextStream out(this);

            out << "file_version," << kFileVersionMajor << "." << kFileVersionMinor << endl;
            out << "serial," << m_dutSerial << endl;
            out << endl;
            out << "freq_Hz,asf" << endl;

            retVal = true;
        }

        return retVal;
    }

    bool DdsCalFile::addPoint(uint64_t freq, uint32_t asf)
    {
        bool retVal = false;

        // Is the file open for writing?
        if (isWritable())
        {
            // Write the line
            QTextStream out(this);

            out << freq << "," << asf << endl;

            retVal = true;
        }

        return retVal;
    }
}
