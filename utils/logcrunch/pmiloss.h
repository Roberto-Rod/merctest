#ifndef PmIloss_H
#define PmIloss_H
#include <QString>
#include <QMap>
#include <QVector>
#include "testobj.h"
#include "parseutil.h"
#include "resultstat.h"


class PmIloss : public dataInterface
{
public:
    #include <templates.h>   

    PmIloss( parseUtil * puPtr );

    // Virtual functions (in datainterface) common to all child test extractions
    void getData();
    void getStats(QTextStream * outfile);    


protected:    
    QString testName =  "Insertion Loss:";
    bool startOfLine = true;
    QString getTestName();
    QString getTestName(bool& startOfLine);
    bool getFreq(QString& S1);
    bool getLoss(QString& S1);
    void getAttenExpected();
    void getAttenActual();
    bool setParseUtilCfg();


    struct MeanSD
    {
        double mean;
        double sd;
    };


    struct SDRAW{       
        QString Freq;
        double loss;
    };

    struct SDVECT{
        QString Tname;       
        QString Freq;
        resultStat Loss;
    };


    QVector <SDVECT>   ms_dVect;

    struct SDSTAT{
        QString Tname;       
        QString Freq;
        MeanSD Loss;
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

#endif // PmIloss_H
