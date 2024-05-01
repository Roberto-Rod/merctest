#ifndef PmDSterm_H
#define PmDSterm_H
#include <QString>
#include <QMap>
#include <QVector>
#include "testobj.h"
#include "parseutil.h"
#include "resultstat.h"


class PmDSterm : public dataInterface
{
public:
    #include <templates.h>   

    PmDSterm( parseUtil * puPtr );

    // Virtual functions (in datainterface) common to all child test extractions
    void getData();
    void getStats(QTextStream * outfile);    


protected:    
    QString testName =  "Active RF Input, Terminated Output Port:";
    bool startOfLine = false;
    virtual QString getTestName();
    //virtual void logOutput(  QVector <SDSTAT> * RfPdPtr, QTextStream * ts );
    virtual bool setParseUtilCfg();
    virtual bool getDifference();
    QString getTestName(bool& startOfLine);
    bool getFreq(QString& S1);
    bool getForwardPower();
    bool getReversePower();   


    struct MeanSD
    {
        double mean;
        double sd;
    };


    struct SDRAW{       
        QString Freq;
        double forwardPower;
        double reversePower;
        double difference;
    };

    struct SDVECT{
        QString Tname;       
        QString Freq;
        resultStat forwardPower;
        resultStat reversePower;
        resultStat difference;
    };

    QVector <SDVECT>   ms_dVect;

    struct SDSTAT{
        QString Tname;       
        QString Freq;
        MeanSD forwardPower;
        MeanSD reversePower;
        MeanSD difference;
    };

    QVector <SDSTAT>   ms_dStat;

    // Called from the template to store data in a vector.
    void getLogdata(QString line, SDVECT oldData, SDVECT &rfdata);
    virtual void logOutput(  QVector <SDSTAT> * RfPdPtr, QTextStream * ts );
    SDRAW ms_tempdRaw;
    parseUtil * m_puPtr;
private:
    bool m_endOfRfTest;
    uint16_t m_irecs;

};

#endif // PmDSterm_H
