#ifndef DBINTERFACE_H
#define DBINTERFACE_H


#include <QTextStream>


class dataInterface
{
public:
    virtual ~dataInterface() {}
    virtual void getData()=0;
    virtual void getStats(QTextStream * outFile)=0;
};

#endif // DBINTERFACE_H
