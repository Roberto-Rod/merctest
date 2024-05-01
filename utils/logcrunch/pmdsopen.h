#ifndef PMDSOPEN_H
#define PMDSOPEN_H
#include "pmdsterm.h"


class PmDSopen : public PmDSterm
{
public:
    PmDSopen(parseUtil * puPtr );
    bool setParseUtilCfg();
    bool getDifference();
    QString getTestName();
    void logOutput(  QVector <SDSTAT> * statPtr, QTextStream * outFile );

protected:
     QString testName =  "Active RF Input, Open Output Port:";

};

#endif // PMDSOPEN_H
