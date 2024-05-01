#include "logfile.h"
#include "parseutil.h"


// Set directory filter to show  all .csv and .txt files.
// initialise  list of files.
// Initialise file count and get number of files to be processed.

logfile::logfile(QString path) :
                m_dir(path),
                m_path(path)
{   
    QStringList filters;
    filters << "*.csv" << "*.txt";
    m_dir.setNameFilters(filters);
    m_filelist = m_dir.entryInfoList();
    m_numfiles  = m_dir.count();
    m_filecount=0;
}



// If more files are to be processed, get the next file in the directory instantiated
// in the constructor. Open file and pass back file pointer as output parameter.

 bool logfile::getNextFile(QTextStream ** txtPtr )
 {
     QFile *fp;
     QTextStream *tp;
     bool result = false;
     QString temp;
     if (m_filecount ==  m_numfiles)
     {
         return result;
     }

     if (!m_filelist.isEmpty())
     {
       QFileInfo fileInfo = m_filelist.at(m_filecount);
       m_filecount ++;
       temp = fileInfo.fileName();

       QString abpath = m_dir.absolutePath();
       QString fileName = abpath+"/"+temp;

        fp = new QFile( fileName );
        fp->open(  QIODevice::ReadOnly  | QIODevice::Text );
        tp = new QTextStream(fp);
        * txtPtr  = tp;

        this->extractHeader(tp);

       result = true;
     }
     return result;
 }




 bool logfile::extractHeader(QTextStream * txtPtr)
 {

     bool result= true;
     QString S1 =  txtPtr->readLine();

     // Note the raw DDS cal is an anomaly, so
     // process as a special case.
     if( S1.compare("file_version,0.1")==0)
     {
         extractDdsHeader( txtPtr);
         return result;
     }
     QString S2 =  txtPtr->readLine();
     QString S3 =  txtPtr->readLine();
     QString asyStr =  txtPtr->readLine();
     QString snStr =  txtPtr->readLine();
     QRegularExpression serialNumRx("^Serial Number:\\s{5}([^$]+)");
     //QRegularExpression asyRx("^Device Under Test:\\s{1,3}([^$]+)");  // Complete line
     QRegularExpression asyRx("^Device Under Test:\\s{1,3}([^ ]+)");
     QRegularExpressionMatch match1 = asyRx.match( asyStr );
     QRegularExpressionMatch match2 = serialNumRx.match( snStr );

     if(match1.hasMatch())
     {
          QString asy =  match1.captured(1);
          m_hdr.dut = asy;
     }
     else
     {
         result = false;
     }

     if( match2.hasMatch() )
     {
         QString sncap = match2.captured(2);
         m_hdr.serialNum = sncap;
     }
     else
     {
         result = false;
     }

     return result;
 }




 bool logfile::extractDdsHeader(QTextStream * txtPtr)
 {

     bool result= true;
     QString S1 =  txtPtr->readLine();

     QString token1, token2;
     parseUtil::createParseUtil()->getStringCsv(token1, S1, 0);
     parseUtil::createParseUtil()->getStringCsv(token2, S1, 1);

     m_hdr.dut = "PCB0044";
     if(token1.compare("serial")==0)
     {
         m_hdr.serialNum = token2;
     }
     else
     {
          m_hdr.serialNum = "NOSERIALNUMBER";
     }

     return result;
 }




   bool logfile::getDUT(QString& dut)
   {
     dut = m_hdr.dut;
     return true;
   }


