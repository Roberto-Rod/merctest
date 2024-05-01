#ifndef SBDDSCAL_H
#define SBDDSCAL_H

#include <QString>
#include <QMap>
#include <QVector>
#include "testobj.h"
#include "parseutil.h"
#include "resultstat.h"


class sbDdsCal : public dataInterface
{
public:
    #include <templates.h>

    sbDdsCal( parseUtil * puPtr );
    virtual bool  setParseUtilCfg();

    // Virtual functions (in datainterface) common to all child test extractions
    void getData();
    void getStats(QTextStream * outfile);


protected:
    QString testName =  "freq_Hz,asf";
    bool startOfline = true;
    QString getTestName();
    QString getTestName(bool& startOfLine);

    void getFreq(QString S1);
    void getASF(QString S1);


    struct MeanSD
    {
        double mean;
        double sd;
    };


    struct SDRAW{
        QString Freq;
        uint16_t ASF;
    };

    struct SDVECT{
        QString Tname;
        QString Freq;
        resultStat ASF;
    };


    QVector <SDVECT>   ms_dVect;

    struct SDSTAT{
        QString Tname;
        QString Freq;
        MeanSD ASF;
    };

    QVector <SDSTAT>   ms_dStat;


    // Called from the template to store data in a vector.
    void getLogdata(QString line, SDVECT oldData, SDVECT &rfdata);
    virtual void logOutput(  QVector <SDSTAT> * RfPdPtr, QTextStream * ts );
    SDRAW ms_tempdVect;
    parseUtil * m_puPtr;
private:

    bool m_endOfRfTest;
    uint16_t m_irecs;

};

#endif // SBDDSCAL_H
