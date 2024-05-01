#include "sbddscal.h"
#include "dbrfpath.h"
#include "testobj.h"
#include "parseutil.h"
#include "sbddscal.h"
#include "execobjlist.h"


// m_i is used as an array of structures index.

sbDdsCal::sbDdsCal( parseUtil * puPtr ) :
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


void sbDdsCal::getData()
{
    setParseUtilCfg();
    this->getallLogData<QVector <SDVECT>, SDVECT >(&ms_dVect);

}


// called from templates.h to store data in  vector fields.
void sbDdsCal::getLogdata(QString S1, SDVECT oldData, SDVECT &rfdata)
{
    this->getFreq(S1);
    this->getASF(S1);
    oldData.Tname = getTestName();
    oldData.Freq = ms_tempdVect.Freq;
    oldData.ASF.addVal( ms_tempdVect.ASF );
    rfdata = oldData;
}




bool  sbDdsCal::setParseUtilCfg()
{
    parseUtil::parseCfg pcfg;
    pcfg.CfgEot = parseUtil::NEWLINE;
    pcfg.CfgTestName = false;
    pcfg.CfgEotS = "";
    this->m_puPtr->configParseUtil(&pcfg);
    return true;
}



QString sbDdsCal::getTestName()
{
    QString tn= sbDdsCal::testName;
    return tn;
}



QString sbDdsCal::getTestName(bool& startOfLine)
{
    QString tn= sbDdsCal::testName;
    startOfLine= sbDdsCal::startOfline;
    return tn;
}






// Get frequency string

void sbDdsCal::getFreq(QString S1)
{
    QString token;
    m_puPtr->getStringCsv(token,S1,0);
    ms_tempdVect.Freq = token;
}


// Get power, convert to double and store
// in vector of doubles for crunching later.

 void sbDdsCal::getASF(QString S1)
 {
     QString token;
     bool result;
     double a;
     result = m_puPtr->getStringCsv(token,S1,1);
     if( result == true )
     {
         a = token.toInt();
         ms_tempdVect.ASF = a;
     }
 }




// Crunch vector of doubles to calculate mean and SD
// for ASF

 void sbDdsCal::getStats(QTextStream * tsPtr)
 {

     for(uint16_t i=0 ; i < ms_dVect.size();  i++)
     {
        SDSTAT temp;
        SDVECT rfp = ms_dVect.at(i);
        temp.Tname =      rfp.Tname;
        temp.Freq =       rfp.Freq;
        temp.ASF.mean = rfp.ASF.calcMean();
        temp.ASF.sd =   rfp.ASF.calcSD();

        ms_dStat.append( temp);

     }

     this->logOutput( &ms_dStat, tsPtr);


 }







 void sbDdsCal::logOutput(  QVector <SDSTAT> * statPtr, QTextStream * outFile )
 {

     outFile->setRealNumberPrecision(2);
     SDSTAT temp;
     temp = statPtr->at(0);

     // Output can be copied straight to testcasesourceboardddscalcheck.cpp

     *outFile << "ddsLimits m_ddslimits[dds_size] = {";

     for (int i=0 ; i < statPtr->size() ; i++)
     {

         temp = statPtr->at(i);
         int y = temp.Freq.toInt();
         y /= 1e6;
         QString freqstr = QString::number(y);

         double  mean = temp.ASF.mean;
         double sd = temp.ASF.sd;

         uint16_t sdl, sdu;
         sdl = static_cast <int> (mean - (4 * sd));
         sdu = static_cast <int> (mean + (4 * sd));
         QString sdlstr = QString::number(sdl);
         QString sdustr = QString::number(sdu);
         *outFile << "\n       { " << freqstr << ",  " <<  sdlstr << ",  " << sdustr << ",  },";

     }
     *outFile  << "\n};";

 }

