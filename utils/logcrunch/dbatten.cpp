#include "testobj.h"
#include "dbatten.h"
#include "parseutil.h"
#include "execobjlist.h"


// m_i is used as an array of structures index.

DbAtten::DbAtten( parseUtil * puPtr ) :
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


void DbAtten::getData()
{

    this->getallLogData<QVector <SDVECT>, SDVECT >(&ms_dVect);

}


// called from templates.h to store data in  vector fields.
void DbAtten::getLogdata(QString S1, SDVECT oldData, SDVECT &attData)
{
    this->getAttenExpected(S1);
    this->getAttenActual(S1);

    oldData.Tname = this->getTestName();
    oldData.expected = ms_tempdVect.expected;
    oldData.actual.addVal( ms_tempdVect.actual );
    attData = oldData;
}



bool  DbAtten::setParseUtilCfg()
{
    this->m_puPtr->setCfgDefaults();
    return true;
}




QString DbAtten::getTestName()
{
    //QString tn = "RF Paths:";
    QString tn= DbAtten::testName;
    return tn;
}


QString DbAtten::getTestName(bool& startOfLine)
{
    //QString tn = "RF Paths:";
    QString tn= DbAtten::testName;
    startOfLine= DbAtten::startOfLine;
    return tn;
}



void DbAtten::getAttenExpected(QString S1)
{

    QString token;
    bool result;
    // !! Requires S1;
    result = m_puPtr->getFirstFloatIncludingColon(token, S1);

    if(result == true)
    {
        ms_tempdVect.expected = token;
    }

}

void DbAtten::getAttenActual(QString S1)
{
    QString token;
    bool result;
    double a;

    result = m_puPtr->getFirstFloat(token, S1);
    if( result == true )
    {
        a = token.toDouble();
        ms_tempdVect.actual= a;
    }

}



// Crunch vector of doubles to calculate mean and SD
// for power and 3 frequency rejection ratios

 void DbAtten::getStats(QTextStream * tsPtr)
 {
     for(uint16_t i=0 ; i < ms_dVect.size();  i++)
     {
         SDSTAT temp;
         SDVECT attp = ms_dVect.at(i);

        temp.Tname =       attp.Tname;
        temp.expected =     attp.expected;
        temp.actual.mean=   attp.actual.calcMean();
        temp.actual.sd=    attp.actual.calcSD();

        ms_dStat.append( temp);

     }

     this->logOutput(&ms_dStat, tsPtr);

 }



 void  DbAtten::logOutput(  QVector <SDSTAT> * AttdPtr, QTextStream * outFile )
 {

     outFile->setRealNumberPrecision(2);
     SDSTAT temp;
     temp = AttdPtr->at(0);
     // output assembly name
     *outFile << "\n" << temp.Tname << "\n";

     for (int i=0 ; i < AttdPtr->size() ; i++)
     {
         temp = AttdPtr->at(i);
         *outFile << "\nAttenuator Expected : Actual (Mean,S.D.)" << "(" << temp.expected << ")" << ": (" << temp.actual.mean << "," <<  temp.actual.sd << ")";
     }

}

