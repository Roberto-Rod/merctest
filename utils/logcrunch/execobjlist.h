#ifndef DBRESULT_H
#define DBRESULT_H

#include "parseutil.h"
#include "datainterface.h"


class execObjList
{
public:
    execObjList();
    QList <dataInterface *> *getTestObj( QString assy,  parseUtil * puPtr);
    void setObjList( QString assembly, QList <dataInterface *> * pointerToInterfaceListPtr );
    void getAllData();
    void getAllStats();
private:
    dataInterface * m_dIptr;
    QList <dataInterface *> * m_objList;
    QString m_assembly;


};

#endif // DBRESULT_H
