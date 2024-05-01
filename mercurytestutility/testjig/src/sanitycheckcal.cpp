#include "sanitycheckcal.hpp"
#include "cablecal.hpp"
#include "rfpath.hpp"
#include "settings.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "messagedialog.hpp"

#include <QtWidgets>

namespace testjig
{
    SanityCheckCal::SanityCheckCal(QString key, QObject* parent) :
        QObject(parent),
        common::Calibration(key),
        m_startFreq(kStartFreqDefault),
        m_stopFreq(kStopFreqDefault),
        m_stepFreq(kStepFreqDefault),
        m_powerLevel(0.0)
    {
        // Restore cal using the given key
        restoreCal();
    }

    void SanityCheckCal::setDescription(QString description)
    {
        m_description = description;
    }

    QString SanityCheckCal::description() const
    {
        return m_description;
    }

    void SanityCheckCal::setTestPowerLevel(double powerLevel)
    {
        m_powerLevel = powerLevel;
    }

    QString SanityCheckCal::measurePowerMonitorInstruction() const
    {
        QString retVal = "<B>Please connect the following:</B>"
                         "<P>1. Signal Generator to ";

         { retVal += "Power Meter via Signal Generator cable + Power Meter cable  (Sig Gen cable + Power Meter cable)."; }

        return retVal;
    }

    QString SanityCheckCal::measureInstruction(RfPathHighBandModule::RfCableSet cableSet) const
    {
        QString retVal = "<B>Please connect the following:</B>"
                         "<P>1. Signal Generator to ";

        if(cableSet == RfPathHighBandModule::RfCableSet::POWER_METER_AND_CAB0255)
        {
          retVal += "Power Meter via Power Meter RF cable + CAB0255 (Power Meter cable + CAB055).";
        }
        else
        {
           retVal += "Power Meter via Sig Gen RF cable + CAB0255 (Sig Gen. cable + CAB055).";
        }

        return retVal;
    }

    QString SanityCheckCal::measureTestJigInstruction() const
    {
        QString retVal = "<B>Please connect the following:</B>"
                         "<P>1. Signal Generator to ";

         { retVal += "DUT RF LHS using Sig Gen Cable + CAB0255. Test Rig Spec An.port to Power meter using Spec An. cable"; }

        return retVal;
    }

    bool SanityCheckCal::testRfPower(testjig::SanityCheckType checktype, bool& cancelled,
                      RfPathHighBandModule::RfCableSet  cableSet,
                      uint64_t stopFreq)
    {
        bool retVal = true;
        cancelled = true;

        testjig::sanityResult sr;
        sr.maxErrNeg = 0.0;
        sr.maxErrPos = 0.0;

        m_stopFreq = stopFreq;

        common::Logging::instance()->log("Configure test equipment...", common::Logging::LOG_DEST_PROGRESS);

        // Reset test equipment
        QList<te::TestEquipment*> teList;
        teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
        teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));

        if (te::TestEquipment::resetTe(teList))
        {
            common::Logging::instance()->log("OK", common::Logging::LOG_DEST_PROGRESS);
            cancelled = false;
        }
        else
        {
            common::Logging::instance()->log("**FAILED** (check Power Meter and Signal Generator availability)",
                                              common::Logging::LOG_DEST_PROGRESS);
        }

        // Did the test equipment calls work?
        if (retVal && !cancelled)
        {
            // Set signal generator output power and enable RF output
            retVal &= te::TestEquipmentStore::signalGenerator()->setOutputPower(m_powerLevel);
            retVal &= te::TestEquipmentStore::signalGenerator()->setRfEnabled(true);

            int numPoints = static_cast<int>(((m_stopFreq - m_startFreq) / m_stepFreq)) + 1;

            // Create a progress dialog
            QProgressDialog progressDialog("Measuring Rf ...", "Cancel", 1, numPoints);
            progressDialog.setWindowModality(Qt::ApplicationModal);
            progressDialog.setMinimumDuration(0);
            progressDialog.setFixedSize(progressDialog.sizeHint());

            // Clear the cal points
            m_calPoints.clear();

            // Loop through all of the frequencies - stop if user presses cancel
            for (uint64_t freq = m_startFreq; (freq <= m_stopFreq) && !cancelled; freq += m_stepFreq)
            {
                //common::CalPoint point;
                //point.freq = freq;
                bool pointAccepted = false;
                double power;

                // Keep trying (until user cancels) if measurement is too low
                do
                {
                    // Keep the progress dialog responsive by processing events
                    // between calls to test equipment
                    if (!progressDialog.wasCanceled())
                    {
                        retVal &= te::TestEquipmentStore::signalGenerator()->setFrequency(freq);
                        qApp->processEvents();
                    }

                    if (!progressDialog.wasCanceled())
                    {
                        retVal &= te::TestEquipmentStore::powerMeter()->setFrequency(freq);
                        qApp->processEvents();
                    }

                    if (!progressDialog.wasCanceled())
                    {
                        retVal &= te::TestEquipmentStore::powerMeter()->getPower(power);
                         qApp->processEvents();
                    }

                   double insertionLoss = 0;

                    if(checktype == SanityCheckType::POWERMONITER)
                    {
                        testjig::RfPathPowerMonitor path(testjig::RfPathPowerMonitor::RfCableSet::SIG_GEN_AND_POWER_METER);
                        insertionLoss = m_powerLevel - testjig::CableCal::correctedPower(path, freq, power);
                    }
                     if(checktype == SanityCheckType::SOUCEBOARD)
                      {
                          testjig::RfPathPowerMonitor sigGenPath(testjig::RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM));
                         double  sigGenCableLoss = testjig::CableCal::correctedPower(sigGenPath,  freq, 0.0);

                          testjig::RfPathSourceAssy   path(testjig::RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::LHS, RfPathSourceAssy::TePort::SPECTRUM_ANALYSER));
                          insertionLoss = m_powerLevel - testjig::CableCal::correctedPower(path, freq, power);
                          insertionLoss = insertionLoss - sigGenCableLoss;
                      }
                    else if(checktype == SanityCheckType::HIGHBAND_POWERMETER || checktype == SanityCheckType::HIGHBAND_SIGGEN)
                    {
                         testjig::RfPathHighBandModule path(cableSet);
                         insertionLoss = m_powerLevel - testjig::CableCal::correctedPower(path, freq, power);
                    }

                    // Limit to positive numbers
                    //  if (insertionLoss < 0.0) { insertionLoss = 0.0; }

                    // Round to 1 d.p.
                    insertionLoss = round(insertionLoss * 10.0) / 10.0;

                    // Is this point acceptable?
                    if (insertionLoss <= 20.0 && insertionLoss >= -5.0)
                    {
                        pointAccepted = true;
                        QString  freq1dp = QString::number (freq/1e6,'f',1);
                        common::Logging::instance()->log(QString("%1  %2")
                                                          .arg(freq1dp)
                                                          .arg(insertionLoss),
                                                          common::Logging::LOG_DEST_PROGRESS);
                        if (insertionLoss < sr.maxErrNeg)
                        {
                            sr.maxErrNeg  = insertionLoss;
                        }
                        else if (insertionLoss > sr.maxErrPos)
                        {
                            sr.maxErrPos = insertionLoss;
                        }
                    }
                    else
                    {
                        gui::MessageDialog pointDialog(QString("Incorrect Power Measured  (%1 dB)"
                                                        " at (%2 MHz)"
                                                        "<P>Check cables and press OK to retry, otherwise cancel the calibration")
                                                        .arg(insertionLoss)
                                                        .arg(freq/1e6),
                                                        gui::MessageDialogType::ERR);
                        if (!pointDialog.exec()) { cancelled = true; }
                    }
                } while(!cancelled && !pointAccepted);

                //m_calPoints.append(point);
                progressDialog.setValue(progressDialog.value() + 1);

                cancelled |= progressDialog.wasCanceled();
            }
        }

        // Disable signal generator RF output
        retVal &= te::TestEquipmentStore::signalGenerator()->setRfEnabled(false);

        common::Settings::instance()->setSanityResults(checktype, sr);

        return retVal;
    }

    double SanityCheckCal::correctedPower(uint64_t freq, double power) const
    {
        double corrected = power;

        if (m_calPoints.length() > 0)
        {
            // Search through the cal points and find the 2 points to interpolate between
            int point1 = 0;
            for (int i = 0; i < m_calPoints.length(); i++)
            {
                // Get point 1 which will be the last point whose frequency is lower
                // than the search frequency
                if (m_calPoints.at(i).freq < freq)
                {
                    point1 = i;
                }
            }

            // Now, get point 2:
            // if there is another point above point 1 then select that,
            // otherwise there is only 1 point (i.e. just use a flat-line
            // value after the last point)
            double loss;
            common::CalPoint p1 = m_calPoints.at(point1);

            if (m_calPoints.length() > (point1 + 1))
            {
                int point2 = point1 + 1;

                // Now perform a linear interpolation
                common::CalPoint p2 = m_calPoints.at(point2);

                // Do all of the maths in double types. Allowing signed frequency copes with the case where
                // freq is less than the cal point 1 frequency.
                loss = (((p2.loss - p1.loss) / ((double)p2.freq - (double)p1.freq)) *
                         ((double)freq - (double)p1.freq)) + p1.loss;
            }
            else
            {
                loss = p1.loss;
            }

            // And apply the correction
            corrected = power + loss;
        }

        return corrected;
    }

    double SanityCheckCal::correctedPower(RfPath& path, uint64_t freq, double power)
    {
        SanityCheckCal cal(path.key());

        return cal.correctedPower(freq, power);
    }

    bool SanityCheckCal::storeCal() const
    {
        return common::Settings::instance()->setCalPoints(m_calKey, m_calPoints);
    }

    bool SanityCheckCal::restoreCal()
    {
        return common::Settings::instance()->getCalPoints(m_calKey, m_calPoints);
    }

    const QVector<common::CalPoint>& SanityCheckCal::calPoints()
    {
        return m_calPoints;
    }
}
