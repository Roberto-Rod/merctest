#include "visasignalgenerator.hpp"

#include <QDebug>

namespace te
{
    // The expected models; any models which *contain* the text in any one of the strings will pass
    const QStringList VisaSignalGenerator::kExpectedModels = QStringList() << QString("N5171")
                                                                           << QString("N5172")
                                                                           << QString("N5181")
                                                                           <<  QString("N5182A")
                                                                           <<  QString("N5182B");

    const QStringList VisaSignalGenerator::kResetCmds = QStringList() << QString("*RST")
                                                                      << QString("*CLS");

    const QString VisaSignalGenerator::kSetFreqCmd   = QString(":FREQ:CW %1 HZ");     /* Set Carrier Frequency Value */
    const QString VisaSignalGenerator::kSetFreqAndUnitCmd  = QString(":FREQ:CW %1 %2");     /* Set Carrier Frequency Value */
    const QString VisaSignalGenerator::kGetFreqCmd   = QString(":FREQ:CW?");          /* Get Carrier Frequency Value */
    const QString VisaSignalGenerator::kSetRfLevCmd  = QString(":POW:LEV %1 DBM");
    const QString VisaSignalGenerator::kGetRfLevCmd  = QString(":POW:LEV?");
    const QString VisaSignalGenerator::kRfEnableCmd  = QString(":OUTP:STAT ON");
    const QString VisaSignalGenerator::kRfDisableCmd = QString(":OUTP:STAT OFF");
    const QString VisaSignalGenerator::kGetRfEnCmd   = QString(":OUTP:STAT?");

    VisaSignalGenerator::VisaSignalGenerator(const QString& resourceName) :
        VisaTestEquipment(kExpectedModels, resourceName)
    {
    }

    VisaSignalGenerator::~VisaSignalGenerator()
    {
        // Disable the RF output when the object is deleted
        (void)setRfEnabled(false);

        qDebug() << "Delete VisaSignalGenerator";
    }

    bool VisaSignalGenerator::reset()
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

    bool VisaSignalGenerator::setFrequency(uint64_t freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Formulate the set frequency command
            QString cmd = kSetFreqCmd.arg(freq);

            // Send the command
            if (m_pVisaResource->sendCommand(cmd))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSignalGenerator::setFrequency(uint64_t freq, QString unit)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Formulate the set frequency command
            QString cmd = kSetFreqAndUnitCmd.arg(freq).arg(unit);

            // Send the command
            if (m_pVisaResource->sendCommand(cmd))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSignalGenerator::getFrequency(uint64_t& freq)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;

            // Send the command and get the response
            if (m_pVisaResource->sendCommand(kGetFreqCmd, resp))
            {
                // The format of the response is purely the frequency in Hertz (e.g. "+1.0000000E+08")
                // Ref. N5161A/62A/81A/82A/83A SCPI Command Reference
                // http://cp.literature.agilent.com/litweb/pdf/N5180-90004.pdf

                // The response is in floating point format, first convert string to double
                double freqDbl = resp.toDouble(&retVal);

                if (retVal)
                {
                    // If double conversion was successful then round and covert to uint64_t
                    freq = static_cast<uint64_t>(freqDbl + 0.5f);
                }
            }
        }

        return retVal;
    }

    bool VisaSignalGenerator::setOutputPower(double power)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Formulate the set output power command
            QString cmd = kSetRfLevCmd.arg(power);

            // Send the command
            if (m_pVisaResource->sendCommand(cmd))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSignalGenerator::getOutputPower(double& power)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;

            // Send the command and get the response
            if (m_pVisaResource->sendCommand(kGetRfLevCmd, resp))
            {
                // The format of the response is purely the power in dBm (e.g. "+1.0000000E+01")
                // Ref. N5161A/62A/81A/82A/83A SCPI Command Reference
                // http://cp.literature.agilent.com/litweb/pdf/N5180-90004.pdf

                // The response is in floating point format, convert string to double
                power = resp.toDouble(&retVal);
            }
        }

        return retVal;
    }

    bool VisaSignalGenerator::setRfEnabled(bool en)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            // Formulate the output enable/disable command
            QString cmd = en ? kRfEnableCmd : kRfDisableCmd;

            // Send the command
            if (m_pVisaResource->sendCommand(cmd))
            {
                // Wait for Operation Complete
                retVal = m_pVisaResource->waitOpc();
            }
        }

        return retVal;
    }

    bool VisaSignalGenerator::getRfEnabled(bool& en)
    {
        bool retVal = false;

        if (m_pVisaResource->isSessionOpen())
        {
            QString resp;

            // Send the command and get the response
            if (m_pVisaResource->sendCommand(kGetRfEnCmd, resp))
            {
                // The format of the response is "0" or "1"
                // Ref. N5161A/62A/81A/82A/83A SCPI Command Reference
                // http://cp.literature.agilent.com/litweb/pdf/N5180-90004.pdf

                // Check the response against the expected patterns
                if (resp == "1")
                {
                    en = true;
                    retVal = true;
                }
                else if (resp == "0")
                {
                    en = false;
                    retVal = true;
                }
            }
        }

        return retVal;
    }
}
