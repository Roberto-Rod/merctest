#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QFile.h>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>

class logfile
{
public:
    struct hdr {
        QString dut;
        QString serialNum;
    };

    logfile(QString path);
    bool getNextFile(QTextStream **filePtr);
    bool getDUT(QString& dut);
    QString getPath()  {
        return m_path;
    }

protected:
    bool extractHeader(QTextStream *txtPtr);
    bool extractDdsHeader(QTextStream *txtPtr);

private:      
    QFileInfoList m_filelist;
    QDir m_dir;
    QString m_path;
    uint16_t  m_i=0;
    uint16_t m_numfiles=0;
    uint16_t m_filecount=0;
    hdr m_hdr;
};

#endif // LOGFILE_H
