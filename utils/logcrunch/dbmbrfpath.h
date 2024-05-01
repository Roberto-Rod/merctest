#ifndef MBDBDATARESULT_H
#define MBDBDATARESULT_H

#include "testobj.h"
#include "parseutil.h"
#include "dbrfpath.h"


class DbMbRfPath : public DbRfPath
{
public:
    DbMbRfPath( parseUtil * puPtr );
    void getfRrs();
    bool setParseUtilCfg();
    void getallAttenuationData();   
    void logOutput(  QVector <SDSTAT> * statPtr, QTextStream * ts );

private:

};

#endif // MBDBDATARESULT_H
