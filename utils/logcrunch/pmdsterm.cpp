#include "testobj.h"
#include "parseutil.h"
#include "pmdsterm.h"
#include "execobjlist.h"


// m_i is used as an array of structures index.

PmDSterm::PmDSterm( parseUtil * puPtr ) :
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


void PmDSterm::getData()
{
    setParseUtilCfg();
    this->getallLogData<QVector <SDVECT>, SDVECT >(&ms_dVect);

}


// called from templates.h to store data in  vector fields.
void PmDSterm::getLogdata(QString S1, SDVECT oldData, SDVECT &rfdata)
{  
    this->getFreq(S1);
    this->getForwardPower();
    this->getReversePower();
    this->getDifference();

    m_puPtr->readLine();

    oldData.Tname = getTestName();
    oldData.Freq = ms_tempdRaw.Freq;
    oldData.forwardPower.addVal( ms_tempdRaw.forwardPower );
    oldData.reversePower.addVal(  ms_tempdRaw.reversePower );
    oldData.difference.addVal( ms_tempdRaw.difference );

    rfdata = oldData;
}




bool  PmDSterm::setParseUtilCfg()
{
    parseUtil::parseCfg pcfg;
    pcfg.CfgEot = parseUtil::SPACETOKEN;
    pcfg.CfgTestName = true;
    pcfg.CfgEotS = "\"Good\" difference points:";
    this->m_puPtr->configParseUtil(&pcfg);
    return true;
}


QString PmDSterm::getTestName()
{
    //QString tn = "RF Paths:";
    QString tn= PmDSterm::testName;
    return tn;
}


QString PmDSterm::getTestName(bool& startOfLine)
{
    //QString tn = "RF Paths:";
    QString tn= PmDSterm::testName;
    startOfLine= PmDSterm::startOfLine;
    return tn;
}



bool getFreq(QString& S1);
bool getForwardPower(QString& S1);
void getReversePower();
void getDifference();


// Get frequency uses pre-read line(S1) as the previous function
// was for "endofTest()".
bool PmDSterm::getFreq(QString& S1)
{
    QString token;
    bool result;
    uint16_t x=2;
    result = m_puPtr->getTokenAfterColon(token,S1);
    //result = m_puPtr->getTokenX(token, x, S1);

    if(result == true)
    {
        ms_tempdRaw.Freq = token;
    }
    return result;
}



// Get path string, getFirstFloat(token) defaults
// to reading the next line within the function.

bool PmDSterm::getForwardPower()
{
    QString token;
    bool result;
    result = m_puPtr->getFirstFloat(token);

    double a = 0.0;
    if(result == true)
    {
        a = token.toDouble();
        ms_tempdRaw.forwardPower = a;
    }
    return result;
}



bool PmDSterm::getReversePower()
{
    QString token;
    bool result;
    result = m_puPtr->getFirstFloat(token);

    double a = 0.0;
    if(result == true)
    {
        a = token.toDouble();
        ms_tempdRaw.reversePower = a;
    }
    return result;
}


bool PmDSterm::getDifference()
{
    QString token;
    bool result;   
    result = m_puPtr->getnumberAfterColon(token);

    double a = 0.0;
    if(result == true)
    {
        a = token.toDouble();
        ms_tempdRaw.difference = a;
    }
    return result;
}




// Crunch vector of doubles to calculate mean and SD
// for power and 3 frequency rejection ratios

 void PmDSterm::getStats(QTextStream * tsPtr)
 {

     for(uint16_t i=0 ; i < ms_dVect.size();  i++)
     {
        SDSTAT temp;
        SDVECT tempV =     ms_dVect.at(i);
        temp.Tname =       tempV.Tname;
        temp.Freq =        tempV.Freq;
        temp.forwardPower.mean =   tempV.forwardPower.calcMean();
        temp.forwardPower.sd =     tempV.forwardPower.calcSD();
        temp.reversePower.mean =   tempV.reversePower.calcMean();
        temp.reversePower.sd =     tempV.reversePower.calcSD();
        temp.difference.mean =     tempV.difference.calcMean();
        temp.difference.sd =       tempV.difference.calcSD();

        ms_dStat.append( temp);
        //this->logOutput( &m_rfpd, tsPtr);

     }

     this->logOutput( &ms_dStat, tsPtr);

 }




 void PmDSterm::logOutput(  QVector <SDSTAT> * statPtr, QTextStream * outFile )
 {

     outFile->setRealNumberPrecision(2);
     outFile->setRealNumberNotation(QTextStream::FixedNotation);
     outFile->setFieldWidth(5);
     SDSTAT temp;
     temp = statPtr->at(0);
     // output assembly name
     *outFile << "\n" << "Directivity / Sensitivity: " << "\n";

     *outFile << "\n" << temp.Tname << "\n";

     for (int i=0 ; i < statPtr->size() ; i++)
     {
          temp = statPtr->at(i);

         *outFile << "\nFrequency "  << temp.Freq << " MHz:  ";
         {
            *outFile << "\n Forward Reading: Mean/S.D.         " << temp.forwardPower.mean   << ", " << temp.forwardPower.sd;
            *outFile << "\n Reverse Reading: Mean/S.D.         " << temp.reversePower.mean   << ", " << temp.reversePower.sd;
            *outFile << "\n Difference: Mean/ S.D.             " << temp.difference.mean     << ", " << temp.difference.sd;
         }

     }


 }






