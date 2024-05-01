#ifndef PARSEUTIL_H
#define PARSEUTIL_H

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QMap>

#include "datainterface.h"

class parseUtil
{
public:    
    // Configure parse utils
    // Customise parse utility on per test basis
    // CfgEot: set end of test criteria
    // CfgEotS: String to signify end of test.
    // CfgTestName : Configure finding test results

    enum EndofTest { DASHES, SPACETOKEN, NEWLINE };
    struct parseCfg
    {       
        enum EndofTest CfgEot;
        bool CfgTestName;
        QString CfgEotS;
    };

    static parseUtil * createParseUtil();
    parseUtil();
    bool setFilePtr(QTextStream *filePtr);
    bool configParseUtil(parseCfg * ptr);
    void setCfgDefaults();
    bool findResults(QString test);
    bool findResults(QString test, bool startOfLine );
    bool endOfData();
    bool endOfTest(QString& token);    
    bool getStringCsv(QString& token, QString S1, uint16_t csvNum );
    bool getTokenX(QString& token, uint16_t x);
    bool getTokenX(QString& token, uint16_t x, QString S1);
    bool getTokenAfterColon(QString& token);
    bool getTokenAfterColon(QString& token, QString S1);
    bool getFirstFloat(QString& token);
    bool getFirstFloat(QString& token, QString S1);
    bool getFirstFloatIncludingColon(QString& token);
    bool getFirstFloatIncludingColon(QString& token, QString S1 );
    bool getFloat(QString& token, QString S1, bool afterColon = true);
    bool getnumberAfterColon(QString& token);
    bool getnumberAfterColon(QString& token, QString S1);

    QString  readLine();

private:   
    QFile *m_filePtr;
    QTextStream * m_textPtr;
    parseCfg m_parseCfg;
    static  parseUtil *m_parseUtilPtr;

};

#endif // PARSEUTIL_H


