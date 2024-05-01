#ifndef TEMPLATES_H
#define TEMPLATES_H


template <class T, class W >
void getallLogData(T *dptr )
{

    bool result;    
    QString tn = getTestName();
    result = m_puPtr->findResults(tn);

    // Set to 0 for each log file
    uint16_t i=0;
    while(1 && result)
    {
        bool result;
        // Stores one Path & Freq set of results
        QString S1;
        // Readline to determine if end of test
        // if not end of test pass readline string to
        // getPath(QString)

        result = m_puPtr->endOfTest(S1);

        if (result)
        {
            m_irecs = dptr->size();
            m_endOfRfTest = true;
            break;
        }

        W m_temp;

        // If end of test for the first file, store the results from
        // subsequent files. Use read-modify-write on data read by vector
        // index.

        // READ
        if(m_endOfRfTest)
        {
           if(i < m_irecs )
           {
               m_temp = (*dptr)[i];
           }

        }

        W outRfdata;
        // MODIFY
        getLogdata(S1, m_temp, outRfdata);

        // Results from the first file, continue until
        // end of test reached.
        if(!m_endOfRfTest)
        {
            dptr->append(outRfdata);
        }

        // Store the results from the second or subsequent files.

        // WRITE
         else if(i < m_irecs )
        {
            (*dptr)[i]= outRfdata;
            i++;
        }
    };

}



#endif // TEMPLATES_H
