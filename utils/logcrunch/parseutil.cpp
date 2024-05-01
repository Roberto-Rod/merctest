#include "parseutil.h"
#include <QString>
#include <QTextStream>
#include <QRegularExpression>


parseUtil *parseUtil::m_parseUtilPtr = 0;


parseUtil::parseUtil()
{
    m_parseCfg.CfgEot = DASHES;
    m_parseCfg.CfgTestName = false;
    m_parseCfg.CfgEotS = "";
}


 parseUtil * parseUtil::createParseUtil()
 {
     if(parseUtil::m_parseUtilPtr == 0)
     {
         parseUtil::m_parseUtilPtr = new parseUtil();
     }
     return    parseUtil::m_parseUtilPtr;
 }


bool parseUtil::setFilePtr(QTextStream *txtPtr)
{
    m_textPtr = txtPtr;
    return true;
}


void parseUtil::setCfgDefaults()
{
    m_parseCfg.CfgEot = DASHES;
    m_parseCfg.CfgTestName = false;
    m_parseCfg.CfgEotS = "";
}




bool parseUtil::configParseUtil(parseCfg * parseCfgptr)
{
    m_parseCfg.CfgEot = parseCfgptr->CfgEot;
    m_parseCfg.CfgTestName = parseCfgptr->CfgTestName;
    m_parseCfg.CfgEotS = parseCfgptr->CfgEotS;
}


bool parseUtil::endOfTest(QString& token)
{
    bool result = false;
    QString S1 = this->readLine();
    token = S1;
    // Customised end of test (non default)
    //
    if( m_parseCfg.CfgEot == SPACETOKEN )
    {
        QString rxs = "^\\s+(.+)";
        QRegularExpression rxp(rxs);
        QRegularExpressionMatch rxpm = rxp.match(S1);
        QString cap1 = rxpm.captured(1);
        if(cap1.startsWith(m_parseCfg.CfgEotS))
        {
            result = true;
        }

    }


    if( m_parseCfg.CfgEot == NEWLINE )
    {
        QString rxs = "^(.+)";
        QRegularExpression rxp(rxs);
        QRegularExpressionMatch rxpm = rxp.match(S1);
        QString cap1 = rxpm.captured(1);
        if(m_parseCfg.CfgEotS.compare(cap1)==0)
        {
            result = true;
        }

    }

    // If end of test "--------" will be read
    // Might be (10 or 14) chars depending on "PASS" or "**FAIL**"
    else if (S1.startsWith("-------"))
    {
        result = true;
    }
    return result;

}


bool parseUtil::getTokenAfterColon(QString& token)
{
    bool result;
    QString S1 = this->readLine();
    result = getTokenAfterColon(token,S1);
    return result;

}




// Note this function is different in that the string is passed to it,
// this is because the previous test is usually testing for end of test.

bool parseUtil::getTokenAfterColon(QString& token, QString S1)
{
    bool result =false;

    QString  Rxs = "^([^:]+):\\s([^\\s]+)";

    QRegularExpression Rx(Rxs);
    QRegularExpressionMatch match = Rx.match(S1);

    // Capture token after colon
    if( match.hasMatch() )
    {
        token = match.captured(2);
        result = true;
    }

     return result;
}




bool parseUtil::getnumberAfterColon(QString& token)
{
    bool result;
    QString S1 = this->readLine();
    result = getnumberAfterColon(token,S1);
    return result;

}


bool parseUtil::getnumberAfterColon(QString& token, QString S1)
{

    bool result =false;

    QString  Rxs = "^([^:]+):\\s(\\d+)";

    QRegularExpression Rx(Rxs);
    QRegularExpressionMatch match = Rx.match(S1);

    // Capture token after colon
    if( match.hasMatch() )
    {
        token = match.captured(2);
        result = true;
    }

     return result;

}


bool  parseUtil::getFirstFloatIncludingColon(QString& token)
{
    bool result;
    QString S1 = this->readLine();
    result = getFloat(token, S1, false );

    return result;
}


// Get first float in parenthisis, ignoring the colon.

bool  parseUtil::getFirstFloatIncludingColon(QString& token, QString S1)
{
    bool result;   
    result = getFloat(token, S1, false );

    return result;
}



bool parseUtil::getFirstFloat(QString& token)
{
    bool result;
    QString S1 = this->readLine();
    result = getFirstFloat(token, S1);
    return result;
}


// Get first float in parenthisis after the colon

 bool parseUtil::getFirstFloat(QString& token, QString S1)
 {
     bool result;
     result = getFloat(token, S1, true );
     return result;
 }



 // Get first floating point number after first parenthisis '('
 // e.g. Output Power: **FAIL** ( -1.3dBm, min=7.1dBm, max=40dBm )


 bool  parseUtil::getTokenX(QString& token, uint16_t x, QString S1)
 {
       bool result = false;
       QString z;

       // Replace parenthises with commas to make
       // it easier to split into strings
       // Check for presence of parenthises first.
       if( S1.indexOf('(') !=-1 && S1.indexOf(')') != -1)
       {
           S1.replace('(',' ');
           S1.replace(')',' ');
           S1.replace(',',' ');
           S1.replace('=',' ');

           //  !QStringList fields = S1.split(QRegularExpression("\\W+"),QString::SkipEmptyParts);
           QStringList fields = S1.split(QRegularExpression("[^a-zA-Z0-9_\\.]"),QString::SkipEmptyParts);
           // get second field if there are more than one field.
           if( x  < fields.size()  )
           {
               z  = fields[x];
               token = z;
               result = true;
           }
       }
       else
       {
           return result;
       }

     return result;
 }




bool parseUtil::getStringCsv(QString& token, QString S1, uint16_t csvNum )
{
    bool result = false;
    QStringList fields = S1.split(",");


    // Store field index csvNum
    if( csvNum  < fields.size()  )
    {
        token  = fields[csvNum];
        result = true;
    }
    return result;
}




// Get first floating point number after first parenthisis '('
// e.g. Output Power: **FAIL** ( -1.3dBm, min=7.1dBm, max=40dBm )

  bool parseUtil::getFloat( QString& token, QString S1,  bool afterColon )
  {
      bool result = false;          
      QString z;

      // Remove stuff before ':' if required
      if(afterColon == true)
      {
         int16_t  index =  S1.indexOf(":");
         if ( index != -1)
         {
              S1.remove(0,index);
         }

      }
      // Replace parenthises with commas to make
      // it easier to split into strings
      // Check for presence of parenthises first.
      if( S1.indexOf('(') !=-1 && S1.indexOf(')') != -1)
      {
          S1.replace('(',',');
          S1.replace(')',',');
          QStringList fields = S1.split(",");
          // get second field if there are more than one field.
          if( 1  < fields.size()  )
          {
              z  = fields[1];
          }
      }
      else
      {
          return result;
      }

      // From the start of the line capture everything until letter 'd'.
      // (first letter of the unit description e.g. dBm).
      //  QString Rxs= "^([^d]+)";

      //  From the start of line match 0 or more spaces
      // capture '-'(if it exists), any digit and any decimal dot one or more times.
      QString Rxs ="^\\s*(-?[\\d\\.]+)";
      QRegularExpression rx(Rxs);
      QRegularExpressionMatch match = rx.match(z);
      if(match.hasMatch())
      {
          QString z2 = match.captured(1);
          token = z2;
          result = true;
      }
      else
      {
         QString z3 = rx.errorString();
         uint16_t v = rx.patternErrorOffset();
      }
    return result;
  }

  QString  parseUtil::readLine()
  {
      QString s =  m_textPtr->readLine();
      return s;
  }


 bool parseUtil::endOfData()
 {
     bool result= false;
     result = m_textPtr->atEnd();
     return result;
 }


 // Read lines in the log file until the required test is found
 // e.g. "RF Paths:"


bool parseUtil::findResults(QString test)
{
    bool startOfLine = true;
    if(m_parseCfg.CfgTestName == true)
    {
        startOfLine = false;
    }
    bool result;
    result = findResults( test, startOfLine );
    return result;
}



bool parseUtil::findResults(QString test, bool startOfLine )
{
    bool found= false;
    while ( !this->endOfData())
    {       
        QString S1 = this->readLine();
        // Test must start in column 0
        QString Rxs= "(^"+test+"$)";
        // Test does not start in column  Zero
        if (startOfLine == false )
        {
             Rxs= "^[\\W]+("+test+"$)";
        }
        QRegularExpression Rx(Rxs);
        QRegularExpressionMatch match = Rx.match( S1 );
        if( match.hasMatch() )
        {
            QString testa  = match.captured(1);
            if( testa.compare(test)==0 )
            {
                found = true;
                break;
            }
        }
    }
    return found;
}


