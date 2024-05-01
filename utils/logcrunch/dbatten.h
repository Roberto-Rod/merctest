#ifndef DbAtten_H
#define DbAtten_H
#include <QString>
#include <QMap>
#include <QVector>
#include "testobj.h"
#include "parseutil.h"
#include "resultstat.h"

// Attenuation test lines to parse MB & HB
// (just more lines for HB).

//--------------
//-- **FAIL** --
//--------------
//  Attenuator Test:
//  Attenuation Setting 0 (0.00dB): Pass ( 0.01dB )
//----------
//-- Pass --
//----------


class DbAtten : public dataInterface
{
public:
    #include <templates.h>   

    DbAtten(  parseUtil * puPtr );

    // Virtual functions (in datainterface) common to all child test extractions
    void getData();
    void getStats(QTextStream *outfile);


protected:
    QString testName = "Attenuator Test:";
    bool startOfLine = true;
    QString getTestName();
    QString getTestName(bool& startOfLine);
    bool setParseUtilCfg();


    // The line is fed in from getLogdata()
    void getAttenExpected(QString line);
    void getAttenActual(QString line);

    struct MeanSD
    {
        double mean;
        double sd;
    };


    struct SDRAW{
        QString expected;
        double actual;
    };

    struct SDVECT{
        QString Tname;
        QString expected;
        resultStat actual;
    };

    QVector <SDVECT>  ms_dVect;

    struct SDSTAT{
        QString Tname;
        QString expected;
        MeanSD actual;
    };

    QVector <SDSTAT>  ms_dStat;

    // Called from the template to store data in a vector.
    void getLogdata(QString line, SDVECT oldData, SDVECT &Attdata);
    void logOutput( QVector <SDSTAT> * SDSTAT, QTextStream * ts );
    SDRAW ms_tempdVect;
    QString m_assembly;
    parseUtil * m_puPtr;


private:    
    bool m_endOfRfTest;
    uint16_t m_irecs;

};

#endif // DbAtten_H
