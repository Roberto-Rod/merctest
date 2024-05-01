
#include "execobjlist.h"
#include "dbrfpath.h"
#include "dbmbrfpath.h"

DbMbRfPath::DbMbRfPath(parseUtil * ptr) :
    DbRfPath(ptr)
{


}



bool  DbMbRfPath::setParseUtilCfg()
{
    this->m_puPtr->setCfgDefaults();
    return true;
}



// The MB RfPath 1 only has one frequency rejection test, not three.
// Fill the other (normally tested) results with zero.

// Line read below:
// "2x Fundamental Rejection: Pass ( 45.1dBc, min=29dBc, max=100dBc )"

void DbMbRfPath::getfRrs()
{
    QString pth = ms_tempdVect.Rfpath;

    if (pth.compare("1") == 0 )
    {
        QString token;
        bool result;
        double a;
         result = m_puPtr->getFirstFloat( token );

        if( result == true )
        {
            a = token.toDouble();
            ms_tempdVect.Rr1 =  a ;
        }

        ms_tempdVect.Rr2 = 0;
        ms_tempdVect.Rr3 = 0;
    }
    else
    {
        this->DbRfPath::getfRrs();
    }

}




void DbMbRfPath::logOutput(  QVector <SDSTAT> * RfPdPtr, QTextStream * outFile )
{

    outFile->setRealNumberPrecision(2);
    SDSTAT temp;
    temp = RfPdPtr->at(0);
    // output assembly name
    *outFile << "\n" << temp.Tname << "\n";

    for (int i=0 ; i < RfPdPtr->size() ; i++)
    {
         temp = RfPdPtr->at(i);

        *outFile << "\nRf Path "  << temp.Rfpath;
        *outFile << "\nOutput Frequency " << temp.Freq;
        if( temp.Rfpath.compare("1") == 0 )
        {
            *outFile << "\nOutput Power Mean/S.D:             " << temp.Power.mean << ", " << temp.Power.sd;
            *outFile << "\n2x Fundamental Rejection Mean/S.D: " << temp.Rr1.mean   << ", " << temp.Rr1.sd;
        }
        else
        {
            *outFile << "\nOutput Power Mean/S.D:             " << temp.Power.mean << ", " << temp.Power.sd;
            *outFile << "\nFundamental Rejection Mean/S.D:    " << temp.Rr1.mean   << ", " << temp.Rr1.sd;
            *outFile << "\n3x Fundamental Rejection Mean/S.D: " << temp.Rr2.mean   << ", " << temp.Rr2.sd;
            *outFile << "\n4x Fundamental Rejection Mean/S.D: " << temp.Rr3.mean   << ", " << temp.Rr3.sd;
        }

    }


}

