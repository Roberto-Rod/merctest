#include "pmdsterm.h"
#include "pmdsopen.h"


PmDSopen::PmDSopen(parseUtil * puPtr ) :
    PmDSterm(puPtr)
{


}



QString PmDSopen::getTestName()
{
    //QString tn = "RF Paths:";
    QString tn= PmDSopen::testName;
    return tn;
}


bool PmDSopen::getDifference()
{
     ms_tempdRaw.difference = 1.0;
     return true;
}



bool  PmDSopen::setParseUtilCfg()
{
    parseUtil::parseCfg pcfg;
    pcfg.CfgEot = parseUtil::DASHES;
    pcfg.CfgTestName = true;
    pcfg.CfgEotS = "";
    this->m_puPtr->configParseUtil(&pcfg);
    return true;
}




void PmDSopen::logOutput(  QVector <SDSTAT> * statPtr, QTextStream * outFile )
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
           //*outFile << "\n Difference: Mean/ S.D.             " << temp.difference.mean     << ", " << temp.difference.sd;
        }

    }


}
