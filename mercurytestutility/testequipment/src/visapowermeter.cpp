#include "visapowermeter.hpp"

#include <QDebug>

#include <cmath>
#include <cstdlib>

namespace te
{
    // The expected models; any models which *contain* the text in any one of the strings will pass
    const QStringList VisaPowerMeter::kExpectedModels = QStringList() << QString("NRP-Z")   <<  QString("NRP18S");

    const QStringList VisaPowerMeter::kResetCmds = QStringList() << QString("INIT:CONT OFF")                  /* Set to triggered mode */
                                                                 << QString("SENS:FUNC \"POW:AVG\"")          /* Set to averaging mode */
                                                                 << QString("SENS:FREQ 1000000000")           /* Default to 1GHz */
                                                                 << QString("SENS:AVER:COUN:AUTO OFF")
                                                                 << QString("SENS:AVER:COUN 16")              /* Average over 16 readings */
                                                                 << QString("SENS:AVER:STAT ON")
                                                                 << QString("SENS:AVER:TCON REP")
                                                                 << QString("SENS:CORR:OFFS 0.0")
                                                                 << QString("SENS:CORR:OFFS:STAT OFF")
                                                                 << QString("SENS:POW:AVG:APER 5e-3")
                                                                 << QString("FORMAT ASCII");

    const QString VisaPowerMeter::kZeroCmd         = QString("CAL:ZERO:AUTO ONCE");
    const QString VisaPowerMeter::kEnableOffsCmd   = QString("SENS:CORR:OFFS:STAT ON");
    const QString VisaPowerMeter::kDisableOffsCmd  = QString("SENS:CORR:OFFS:STAT OFF");
    const QString VisaPowerMeter::kSetOffsCmd      = QString("SENS:CORR:OFFS %1");
    const QString VisaPowerMeter::kGetOffsCmd      = QString("SENS:CORR:OFFS?");
    const QString VisaPowerMeter::kSetFreqCmd      = QString("SENS:FREQ %1");
    const QString VisaPowerMeter::kGetFreqCmd      = QString("SENS:FREQ?");
    const QString VisaPowerMeter::kInitMeas        = QString("INIT:IMM");
    const QString VisaPowerMeter::kFetchMeas       = QString("FETCH?");

    VisaPowerMeter::VisaPowerMeter(const QString& resourceName) :
        VisaTestEquipment(kExpectedModels, resourceName)
    {
    }

    VisaPowerMeter::~VisaPowerMeter()
    {
        qDebug() << "Delete VisaPowerMeter";
    }

    bool VisaPowerMeter::reset()
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
                    // Wait for Operation Complete - for some reason the R&S NRP power meter
                    // doesn't support the common "*OPC?" (Operation Complete) query but
                    // instead has to be continually polled with the "STAT:OPER:COND?" query
                    retVal = m_pVisaResource->waitStatOperCond();
                }
                if (!retVal) { break; }
            }
        }

        return retVal;
    }

    bool VisaPowerMeter::zeroSensor()
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Send the zero command
            if (m_pVisaResource->sendCommand(kZeroCmd))
            {
                // Check the VISA resource for errors
                retVal = m_pVisaResource->systemStatus();
            }
        }

        return retVal;
    }

    bool VisaPowerMeter::setOffset(double offset)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Send the offset command
            if (m_pVisaResource->sendCommand(kSetOffsCmd.arg(offset)))
            {
                QString offsEnStr;
                if (offset == 0.0) { offsEnStr = kDisableOffsCmd; }
                else               { offsEnStr = kEnableOffsCmd; }

                // Send the offset enable command
                if (m_pVisaResource->sendCommand(offsEnStr))
                {
                    // Check the VISA resource for errors
                    retVal = m_pVisaResource->systemStatus();
                }
            }
        }

        return retVal;
    }

    bool VisaPowerMeter::getOffset(double& offset)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;

            if (m_pVisaResource->sendCommand(kGetOffsCmd, resp))
            {
                // The response is the offset in floating point format, let
                // the conversion function set the return value
                offset = resp.toDouble(&retVal);
            }
        }

        return retVal;
    }

    bool VisaPowerMeter::setFrequency(uint64_t freq)
    {
        bool retVal = false;

        // Formulate the command
        QString cmd = kSetFreqCmd.arg(freq);

        // Send the command
        if (m_pVisaResource->sendCommand(cmd))
        {
            // Check the VISA resource for errors
            retVal = m_pVisaResource->systemStatus();
        }

        return retVal;
    }

    bool VisaPowerMeter::getFrequency(uint64_t& freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;

            if (m_pVisaResource->sendCommand(kGetFreqCmd, resp))
            {
                // The response is the frequency in floating point format, let
                // the conversion function set the return value
                double freqDbl = resp.toDouble(&retVal);

                if (retVal)
                {
                    // Round and convert the frquency to 64-bit unsigned
                    freq = static_cast<uint64_t>(freqDbl + 0.5f);
                }
            }
        }

        return retVal;
    }

    bool VisaPowerMeter::getPower(double& power)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Send the initialise meaurement command
            if (m_pVisaResource->sendCommand(kInitMeas))
            {
                // Wait for the measurement to complete
                if (m_pVisaResource->waitStatOperCond())
                {
                    QString resp;

                    // Send the fetch measurement command
                    if (m_pVisaResource->sendCommand(kFetchMeas, resp))
                    {
                        bool convOk;

                        // Convert the measurement (in the first comma separated field)
                        // to double precision floating point
                        double meas = resp.split(",").at(0).toDouble(&convOk);

                        // Get absolute value of measurement
                        meas = std::abs(meas);

                        // Ensure that the conversion was successful and that the measurement is not zero
                        // before performing logarithm function
                        if (convOk && (meas != 0.0))
                        {
                            power = 10.0 * std::log10(meas);
                            power += 30.0;

                            retVal = true;
                        }
                    }
                }
            }
        }

        return retVal;
    }
}
