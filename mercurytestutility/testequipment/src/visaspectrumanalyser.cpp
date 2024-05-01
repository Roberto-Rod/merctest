#include "visaspectrumanalyser.hpp"
#include "convert.hpp"

#include <QDebug>
#include <QStringList>

namespace te
{
    // The expected models; any models which *contain* the text in any one of the strings will pass
    const QStringList VisaSpectrumAnalyser::kExpectedModels = QStringList() << QString("N9342")
                                                                            << QString("N9000")
                                                                            << QString("N9010")
                                                                            << QString("N9020")
                                                                            << QString("N9030")
                                                                            << QString("N9038")
                                                                            <<  QString("N9344");

    const QStringList VisaSpectrumAnalyser::kResetCmds = QStringList()
                                                              << QString("INST:SEL SA")
                                                              << QString("*RST")
                                                              << QString("*CLS")             /* Reset the device and clear status */
                                                              << QString(":INIT:CONT 1")
                                                              << QString(":FREQ:START 10.00 MHz")
                                                              << QString(":FREQ:STOP 1550.00 MHz")
                                                              << QString(":BAND:RES:AUTO ON")
                                                              << QString(":BAND:VID:AUTO ON")
                                                              << QString(":POW:ATT:AUTO ON")
                                                              << QString(":DISP:WIND:TRAC:Y:RLEV 20dBm")
                                                              << QString(":FORM REAL");
                                                              //<< QString(":FORM:BORD SWAP");

    const QString VisaSpectrumAnalyser::kSetFrequency  = QString(":FREQ:CENT %1 HZ");
    const QString VisaSpectrumAnalyser::kGetFrequency  = QString(":FREQ:CENT?");
    const QString VisaSpectrumAnalyser::kSetStartFrequency  = QString(":FREQ:START %1 MHZ");
    const QString VisaSpectrumAnalyser::kSetStopFrequency  = QString(":FREQ:STOP %1 MHZ");
    const QString VisaSpectrumAnalyser::kSetSpan       = QString(":FREQ:SPAN %1 HZ");
    const QString VisaSpectrumAnalyser::kGetSpan       = QString(":FREQ:SPAN?");
    const QString VisaSpectrumAnalyser::kSetResBw      = QString(":BAND:RES %1 HZ");
    const QString VisaSpectrumAnalyser::kGetResBw      = QString(":BAND:RES?");
    const QString VisaSpectrumAnalyser::kSetRefLevel   = QString(":DISP:WIND:TRAC:Y:RLEV %1 DBM");
    const QString VisaSpectrumAnalyser::kGetRefLevel   = QString(":DISP:WIND:TRAC:Y:RLEV?");
    const QString VisaSpectrumAnalyser::kSetDbPerDiv   = QString(":DISP:WIND:TRAC:Y:PDIV %1");
    const QString VisaSpectrumAnalyser::kGetDbPerDiv   = QString(":DISP:WIND:TRAC:Y:PDIV?");
    const QString VisaSpectrumAnalyser::kSetContTrig   = QString(":INIT:CONT %1");
    const QString VisaSpectrumAnalyser::kGetContTrig   = QString(":INIT:CONT?");
    const QString VisaSpectrumAnalyser::kTriggerSweep  = QString(":INIT:IMM");
    const QString VisaSpectrumAnalyser::kMarkerPeak    = QString(":CALC:MARK1:MAX");
    const QString VisaSpectrumAnalyser::kMarkerPeakNext    = QString(":CALC:MARK1:MAX:NEXT");
    const QString VisaSpectrumAnalyser::kGetMarkerFreq = QString(":CALC:MARK1:X?");
    const QString VisaSpectrumAnalyser::kGetMarkerPwr  = QString(":CALC:MARK1:Y?");

    VisaSpectrumAnalyser::VisaSpectrumAnalyser(const QString& resourceName) :
        VisaTestEquipment(kExpectedModels, resourceName)
    {
        initDbDivMap();
    }

    VisaSpectrumAnalyser::~VisaSpectrumAnalyser()
    {
        qDebug() << "Delete VisaSpectrumAnalyser";
    }

    bool VisaSpectrumAnalyser::reset()
    {
        bool retVal = false;

        // Before resetting the equipment (re-)connect to it
        if (connect())
        {
            // Loop through all the commands in the setup string list
            QString cmd;
            foreach(cmd, kResetCmds)
            {
                // Send the setup command
                if (m_pVisaResource->sendCommand(cmd))
                {
                    // Wait for Operation Complete
                    retVal = m_pVisaResource->waitOpc();
                }
                if (!retVal) { break; }
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setFrequency(uint64_t freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetFrequency.arg(freq)))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setStartFrequency(uint64_t freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetStartFrequency.arg(freq)))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setStopFrequency(uint64_t freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetStopFrequency.arg(freq)))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getFrequency(uint64_t& freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetFrequency, resp))
            {
                // Attempt conversion of response string to 64-bit unsigned integer
                retVal = common::Convert::strFloatToUInt64(resp, freq);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setSpan(uint64_t span)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetSpan.arg(span)))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getSpan(uint64_t& span)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetSpan, resp))
            {
                // Attempt conversion of response string to 64-bit unsigned integer
                retVal = common::Convert::strFloatToUInt64(resp, span);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setResBw(uint64_t resBw)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetResBw.arg(resBw)))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getResBw(uint64_t& resBw)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetResBw, resp))
            {
                // Attempt conversion of response string to 64-bit unsigned integer
                retVal = common::Convert::strFloatToUInt64(resp, resBw);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setRefLevel(double refLevel)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetRefLevel.arg(refLevel)))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getRefLevel(double& refLevel)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetRefLevel, resp))
            {
                // Attempt conversion of response string to double
                refLevel = resp.toDouble(&retVal);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setDbPerDiv(DbPerDiv scale)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetDbPerDiv.arg(m_dbDivMap[scale])))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getDbPerDiv(DbPerDiv& scale)
    {
        bool retVal = false;

        scale = DbPerDiv::UNKNOWN;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetDbPerDiv, resp))
            {
                // Command worked
                retVal = true;

                // Now convert string response to the enum...
                scale = m_dbDivMap.key(resp, DbPerDiv::UNKNOWN);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::setContTrigger(bool cont)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            if (m_pVisaResource->sendCommand(kSetContTrig.arg(cont ? "1" : "0")))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getContTrigger(bool& cont)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetContTrig, resp))
            {
                // Check for expected responses
                if (resp == "1")
                {
                    cont   = true;
                    retVal = true;
                }
                else if (resp == "0")
                {
                    cont   = false;
                    retVal = true;
                }
            }
        }

        return retVal;
    }

     bool VisaSpectrumAnalyser::getPeak(uint64_t& freq, double& power)
     {
          bool retVal = false;

         if(markerFindPeak())
         {
             QString resp;

             // Read the marker power
             if (m_pVisaResource->sendCommand(kGetMarkerPwr, resp))
             {
                 // Attempt conversion of response string to double
                 power = resp.toDouble(&retVal);

                 // Now read the marker frequency
                 if (m_pVisaResource->sendCommand(kGetMarkerFreq, resp))
                 {
                     // Finally, attempt conversion of result to 64-bit unsigned integer
                     retVal = common::Convert::strFloatToUInt64(resp, freq);
                 }
             }
         }

         return retVal;
     }

    bool VisaSpectrumAnalyser::getPeak(bool next, uint64_t& freq, double& power)
    {
        bool retVal = false;
        bool peaksearch = false;

        if(next == true)
        {
            peaksearch = markerFindPeakNext();
        }
        else
        {
            // Perform the peak search
            peaksearch = markerFindPeak();
        }

        if(peaksearch == true)
        {
            QString resp;

            // Read the marker power
            if (m_pVisaResource->sendCommand(kGetMarkerPwr, resp))
            {
                // Attempt conversion of response string to double
                power = resp.toDouble(&retVal);

                // Now read the marker frequency
                if (m_pVisaResource->sendCommand(kGetMarkerFreq, resp))
                {
                    // Finally, attempt conversion of result to 64-bit unsigned integer
                    retVal = common::Convert::strFloatToUInt64(resp, freq);
                }
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getPeakPower(double& power)
    {
        bool retVal = false;

        if (markerFindPeak())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetMarkerPwr, resp))
            {
                // Attempt conversion of response string to double
                power = resp.toDouble(&retVal);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getPeakPower(bool next, double& power)
    {
        bool retVal = false;
        bool peakSearch;

        if(next == true)
        {
            peakSearch = markerFindPeakNext();
        }
        else
        {
            peakSearch = markerFindPeak();
        }

        if (peakSearch)
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetMarkerPwr, resp))
            {
                // Attempt conversion of response string to double
                power = resp.toDouble(&retVal);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getPeakFrequency(uint64_t& freq)
    {
        bool retVal = false;

        if (markerFindPeak())
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetMarkerFreq, resp))
            {
                // Attempt conversion of response string to 64-bit unsigned integer
                retVal = common::Convert::strFloatToUInt64(resp, freq);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::getPeakFrequency(bool next, uint64_t& freq)
    {
        bool retVal = false;
        bool peakSearch;

        if(next == true)
        {
            peakSearch = markerFindPeakNext();
        }
        else
        {
            peakSearch = markerFindPeak();
        }

        if (peakSearch)
        {
            QString resp;
            if (m_pVisaResource->sendCommand(kGetMarkerFreq, resp))
            {
                // Attempt conversion of response string to 64-bit unsigned integer
                retVal = common::Convert::strFloatToUInt64(resp, freq);
            }
        }

        return retVal;
    }

    bool VisaSpectrumAnalyser::markerFindPeak()
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Trigger a sweep
            if (m_pVisaResource->sendCommand(kTriggerSweep))
            {
                // Wait for Operation Complete
                if (m_pVisaResource->waitOpc())
                {
                    // Set marker 1 to peak
                    if (m_pVisaResource->sendCommand(kMarkerPeak))
                    {
                        // Wait for Operation Complete
                        retVal = m_pVisaResource->waitOpc();
                    }
                }
            }
        }

        return retVal;
    }

//  Mark next peak  (private function)

    bool VisaSpectrumAnalyser::markerFindPeakNext()
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
                    // Set marker 1 to peak
                    if (m_pVisaResource->sendCommand(kMarkerPeakNext))
                    {
                        // Wait for Operation Complete
                        retVal = m_pVisaResource->waitOpc();
                    }
        }
        return retVal;
    }

    void VisaSpectrumAnalyser::initDbDivMap()
    {
        // Initialise the dB per division map
        m_dbDivMap[DbPerDiv::DB_1]  = "DIV1";
        m_dbDivMap[DbPerDiv::DB_2]  = "DIV2";
        m_dbDivMap[DbPerDiv::DB_5]  = "DIV5";
        m_dbDivMap[DbPerDiv::DB_10] = "DIV10";
    }
}
