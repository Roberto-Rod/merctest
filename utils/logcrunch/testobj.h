#ifndef TESTOBJ_H
#define TESTOBJ_H

#include "parseutil.h"
#include "datainterface.h"
//#include "dbrfpath.h"
//#include "dbmbrfpath.h"
#include <QList>

class testObj
{
public:
    testObj();
    // QList <testObj> * getTestObj(QString assy, dataInterface ** specificTestPtr,  parseUtil * puPtr );

    virtual void getData()=0;
    virtual void getStats()=0;

};

#endif // TESTOBJ_H
