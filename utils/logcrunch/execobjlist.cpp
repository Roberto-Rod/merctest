#include "datainterface.h"
#include "execobjlist.h"
#include "dbrfpath.h"
#include "dbmbrfpath.h"
#include "dbatten.h"
#include "pmiloss.h"
#include "pmdsterm.h"
#include "pmdsopen.h"
#include  "sbddscal.h"


execObjList::execObjList()
{

}


// Set object list to the one created in getTestObj()

 void execObjList::setObjList( QString assembly, QList <dataInterface *> * interfaceListPtr )
 {
     m_assembly = assembly;
     m_objList = interfaceListPtr;
 }


// object factory.
// Create list of test objects depending on assembly. Parse utility is added
// to each individual test object.

QList <dataInterface *> * execObjList::getTestObj(QString assy, parseUtil * puPtr)
{
     QList <dataInterface *> * diListPtr = new QList <dataInterface *>();      
     dataInterface * specificTestPtr = NULL;

     // HB Daughter board ("Multiplier")
     if( assy.compare("ASY0129")==0)
     {
         DbRfPath * z =  new DbRfPath(puPtr);
         specificTestPtr = z;
         diListPtr->append(specificTestPtr);
         DbAtten * z1 = new DbAtten(puPtr);
         diListPtr->append(z1);
     }

     // LB Daughter board ("Doubler")
     if( assy.compare("ASY0033")==0)
     {
         DbMbRfPath * z =  new DbMbRfPath(puPtr);
         specificTestPtr = z;
         diListPtr->append(specificTestPtr);
         DbAtten * z1 = new DbAtten(puPtr);
         diListPtr->append(z1);
     }

     // PM HB or LB or MB
     if( assy.compare("ASY0130")==0 || assy.compare("ASY0020")==0  || assy.compare("ASY0021")==0 )
     {
         // Add insertion loss test
         PmIloss * z = new PmIloss(puPtr);
         specificTestPtr = z;
         // Add Directivity / Sensitivity : Terminated test.
         diListPtr->append(specificTestPtr);
         PmDSterm * z1 = new PmDSterm(puPtr);
         diListPtr->append(z1);
         PmDSopen * z2 = new PmDSopen(puPtr);
         diListPtr->append(z2);

     }


     if( assy.compare("PCB0044")==0)
     {
         sbDdsCal * z = new  sbDdsCal(puPtr);
         specificTestPtr = z;
         diListPtr->append(specificTestPtr);
     }


     return diListPtr;
 }


// get all the data from all the tests in question from one log file

void execObjList::getAllData()
{      
     for(int i =0 ; i < m_objList->size(); i++)
     {
         dataInterface * toPtr = m_objList->at(i);
         toPtr->getData();
     }

}


// Generate stats for all the log files in the directory.

void execObjList::getAllStats()
{
    QFile * outfile =  new QFile( this->m_assembly );
    outfile->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ts(outfile);


    for(int i =0 ; i < m_objList->size(); i++)
    {
        dataInterface * toPtr = m_objList->at(i);
        toPtr->getStats(&ts);
    }

}



