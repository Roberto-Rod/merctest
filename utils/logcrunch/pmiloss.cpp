#include "testobj.h"
#include "parseutil.h"
#include "pmiloss.h"
#include "execobjlist.h"


// m_i is used as an array of structures index.

PmIloss::PmIloss( parseUtil * puPtr ) :
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


void PmIloss::getData()
{
    setParseUtilCfg();
    this->getallLogData<QVector <SDVECT>, SDVECT >(&ms_dVect);

}


// called from templates.h to store data in  vector fields.
void PmIloss::getLogdata(QString S1, SDVECT oldData, SDVECT &rfdata)
{  
    this->getFreq(S1);
    this->getLoss(S1);

    oldData.Tname = getTestName();
    oldData.Freq = ms_tempdVect.Freq;
    oldData.Loss.addVal(ms_tempdVect.loss);
    rfdata = oldData;
}

bool  PmIloss::setParseUtilCfg()
{
    this->m_puPtr->setCfgDefaults();
    return true;
}



QString PmIloss::getTestName()
{
    QString tn= PmIloss::testName;
    return tn;
}



QString PmIloss::getTestName(bool& startOfLine)
{
    //QString tn = "RF Paths:";
    QString tn= PmIloss::testName;
    startOfLine= PmIloss::startOfLine;
    return tn;
}




bool PmIloss::getFreq(QString& S1)
{
    QString token;
    bool result;
    uint16_t x=3;
    result = m_puPtr->getTokenX(token, x, S1);

    if(result == true)
    {
        ms_tempdVect.Freq = token;
    }
    return result;
}



// Get path string

bool PmIloss::getLoss(QString& S1)
{
    QString token;
    bool result;
    result = m_puPtr->getFirstFloat(token, S1);

    double a = 0.0;
    if(result == true)
    {
        a = token.toDouble();
        ms_tempdVect.loss = a;
    }
    return result;
}


// Crunch vector of doubles to calculate mean and SD
// for power and 3 frequency rejection ratios

 void PmIloss::getStats(QTextStream * tsPtr)
 {

     for(uint16_t i=0 ; i < ms_dVect.size();  i++)
     {
        SDSTAT temp;
        SDVECT tempV =     ms_dVect.at(i);
        temp.Tname =       tempV.Tname;
        temp.Freq =        tempV.Freq;
        temp.Loss.mean =   tempV.Loss.calcMean();
        temp.Loss.sd =     tempV.Loss.calcSD();

        ms_dStat.append( temp);
        //this->logOutput( &m_rfpd, tsPtr);

     }

     this->logOutput( &ms_dStat, tsPtr);

 }




 void PmIloss::logOutput(  QVector <SDSTAT> * statPtr, QTextStream * outFile )
 {

     outFile->setRealNumberPrecision(2);
     SDSTAT temp;
     temp = statPtr->at(0);
     // output assembly name
     *outFile << "\n" << temp.Tname << "\n";

     for (int i=0 ; i < statPtr->size() ; i++)
     {
          temp = statPtr->at(i);

         *outFile << "\nloss at "  << temp.Freq << " MHz:  ";
         {
             *outFile << "Insertion Loss  Mean/S.D:   " << temp.Loss.mean << ", " << temp.Loss.sd;
         }

     }


 }





