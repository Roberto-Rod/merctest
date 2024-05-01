#ifndef DbDataResult_H
#define DbDataResult_H
#include <QString>
#include <QMap>
#include <QVector>
#include "testobj.h"
#include "parseutil.h"
#include "resultstat.h"


class DbRfPath : public dataInterface
{
public:
    #include <templates.h>   

    DbRfPath( parseUtil * puPtr );
    virtual bool  setParseUtilCfg();

    // Virtual functions (in datainterface) common to all child test extractions
    void getData();
    void getStats(QTextStream * outfile);
    virtual void getfRrs();


protected:    
    QString testName =  "RF Paths:";
    bool startOfline = true;
    QString getTestName();
    QString getTestName(bool& startOfLine);
    bool getPath(QString &path);
    void getFreq();
    void getPower();
    // MB doubler Path1 has slightly different log to all the rest
    // hence the virtual function
    //virtual void getfRrs();
    void getAttenExpected();
    void getAttenActual();


    struct MeanSD
    {
        double mean;
        double sd;
    };


    struct SDRAW{
        QString Rfpath;
        QString Freq;
        double Power;
        double Rr1;
        double Rr2;
        double Rr3;
    };

    struct SDVECT{
        QString Tname;
        QString Rfpath;
        QString Freq;
        resultStat Power;
        resultStat Rr1;
        resultStat Rr2;
        resultStat Rr3;
    };


    QVector <SDVECT>   ms_dVect;

    struct SDSTAT{
        QString Tname;
        QString Rfpath;
        QString Freq;
        MeanSD Power;
        MeanSD Rr1;
        MeanSD Rr2;
        MeanSD Rr3;
    };

    QVector <SDSTAT>   ms_dStat;

    struct Att{
        QString expected;
        resultStat atten;
    };

    struct AttDone{
        QString expected;
        MeanSD atten;
    };


    // Called from the template to store data in a vector.
    void getLogdata(QString line, SDVECT oldData, SDVECT &rfdata);
    virtual void logOutput(  QVector <SDSTAT> * RfPdPtr, QTextStream * ts );
    SDRAW ms_tempdVect;
    parseUtil * m_puPtr;
private:

    bool m_endOfRfTest;
    uint16_t m_irecs;

};

#endif // DbDataResult_H
