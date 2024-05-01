#include "testobj.h"
#include "parseutil.h"
#include "dbrfpath.h"
#include "execobjlist.h"


// m_i is used as an array of structures index.

DbRfPath::DbRfPath( parseUtil * puPtr ) :
    dataInterface(),   
    m_puPtr(puPtr),  
    m_endOfRfTest(false),
    m_irecs(0)
{

}

//  calls function that is defined in templates.h
//  template <class T, class W >
//  void getallLogData(T *dptr )

// Where T is the structure to store data
// and W is the vector of data structures.


void DbRfPath::getData()
{
    this->getallLogData<QVector <SDVECT>, SDVECT >(&ms_dVect);

}


// called from templates.h to store data in  vector fields.
void DbRfPath::getLogdata(QString S1, SDVECT oldData, SDVECT &rfdata)
{
    this->getPath(S1);
    this->getFreq();
    this->getPower();
    this->getfRrs();   

    m_puPtr->readLine();

    oldData.Tname = getTestName();
    oldData.Rfpath = ms_tempdVect.Rfpath;
    oldData.Freq = ms_tempdVect.Freq;
    oldData.Power.addVal( ms_tempdVect.Power );
    oldData.Rr1.addVal( ms_tempdVect.Rr1 );
    oldData.Rr2.addVal( ms_tempdVect.Rr2 );
    oldData.Rr3.addVal( ms_tempdVect.Rr3 );
    rfdata = oldData;
}




bool  DbRfPath::setParseUtilCfg()
{
    this->m_puPtr->setCfgDefaults();
    return true;
}


QString DbRfPath::getTestName()
{
    //QString tn = "RF Paths:";
    QString tn= DbRfPath::testName;
    return tn;
}



QString DbRfPath::getTestName(bool& startOfLine)
{
    //QString tn = "RF Paths:";
    QString tn= DbRfPath::testName;
    startOfLine= DbRfPath::startOfline;
    return tn;
}





// Get path string

bool DbRfPath::getPath(QString& S1)
{
    QString token;
    bool result;
    result = m_puPtr->getTokenAfterColon(token, S1);

    if(result == true)
    {
        ms_tempdVect.Rfpath = token;
    }
    return result;
}




// Get frequency string

void DbRfPath::getFreq()
{
    QString token;
    m_puPtr->getTokenAfterColon(token);
    ms_tempdVect.Freq = token;
}


// Get power, convert to double and store
// in vector of doubles for crunching later.

 void DbRfPath::getPower()
 {
     QString token;
     bool result;
     double a;     
     result = m_puPtr->getFirstFloat(token);
     if( result == true )
     {
         a = token.toDouble();
         ms_tempdVect.Power = a;
     }
 }


 // Get all 3 frequency rejection ratios, convert to double and store
 // in vector of doubles for crunching later.

 void DbRfPath::getfRrs()
 {
     QString token;
     bool result;
     double a;
     result = m_puPtr->getFirstFloat(token);
     if(result == true)
     {
         a = token.toDouble();                
         ms_tempdVect.Rr1 = a;
     }
      result = m_puPtr->getFirstFloat(token);
     if(result == true)
     {
         a = token.toDouble();
         ms_tempdVect.Rr2 = a;
     }
     result = m_puPtr->getFirstFloat(token);

     if(result == true)
     {
         a = token.toDouble();
         ms_tempdVect.Rr3 = a;
     }
 }



// Crunch vector of doubles to calculate mean and SD
// for power and 3 frequency rejection ratios

 void DbRfPath::getStats(QTextStream * tsPtr)
 {

     for(uint16_t i=0 ; i < ms_dVect.size();  i++)
     {
        SDSTAT temp;
        SDVECT rfp = ms_dVect.at(i);
        temp.Tname =      rfp.Tname;
        temp.Rfpath =     rfp.Rfpath;
        temp.Freq =       rfp.Freq;
        temp.Power.mean = rfp.Power.calcMean();
        temp.Power.sd =   rfp.Power.calcSD();
        temp.Rr1.mean =   rfp.Rr1.calcMean();
        temp.Rr1.sd =     rfp.Rr1.calcSD();
        temp.Rr2.mean =   rfp.Rr2.calcMean();
        temp.Rr2.sd =     rfp.Rr2.calcSD();
        temp.Rr3.mean =   rfp.Rr3.calcMean();
        temp.Rr3.sd =     rfp.Rr3.calcSD();

        ms_dStat.append( temp);
        //this->logOutput( &m_rfpd, tsPtr);

     }

     this->logOutput( &ms_dStat, tsPtr);


 }







 void DbRfPath::logOutput(  QVector <SDSTAT> * statPtr, QTextStream * outFile )
 {

     outFile->setRealNumberPrecision(2);
     SDSTAT temp;
     temp = statPtr->at(0);
     // output assembly name
     *outFile << "\n" << temp.Tname << "\n";

     for (int i=0 ; i < statPtr->size() ; i++)
     {
          temp = statPtr->at(i);

         *outFile << "\nRf Path "  << temp.Rfpath;
         *outFile << "\nOutput Frequency " << temp.Freq;
         {
             *outFile << "\nOutput Power Mean/S.D:             " << temp.Power.mean << ", " << temp.Power.sd;
             *outFile << "\n Nx Fundamental Rejection Mean/S.D: " << temp.Rr1.mean   << ", " << temp.Rr1.sd;
             *outFile << "\n Nx Fundamental Rejection Mean/S.D: " << temp.Rr2.mean   << ", " << temp.Rr2.sd;
             *outFile << "\n Nx Fundamental Rejection Mean/S.D: " << temp.Rr3.mean   << ", " << temp.Rr3.sd;
         }

     }


 }





