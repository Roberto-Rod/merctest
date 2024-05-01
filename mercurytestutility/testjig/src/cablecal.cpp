#include "cablecal.hpp"
#include "rfpath.hpp"
#include "settings.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "messagedialog.hpp"

#include <QtWidgets>

namespace testjig
{
    CableCal::CableCal(QString key, QObject* parent) :
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

    void CableCal::setDescription(QString description)
    {
        m_description = description;
    }

    QString CableCal::description() const
    {
        return m_description;
    }

    void CableCal::setTestPowerLevel(double powerLevel)
    {
        m_powerLevel = powerLevel;
    }

    bool CableCal::calibrate(bool& cancelled, const QVector<common::CalPoint>& correction)
    {
        // Perform the calibration
        bool retVal = calibrate(cancelled);

        // If that worked then apply the correction
        if (retVal && !cancelled)
        {
            retVal = common::Calibration::correctCal(m_calPoints, correction);
        }

        return retVal;
    }

    bool CableCal::calibrate(bool& cancelled)
    {
        bool retVal = true;
        cancelled = true;

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
            QProgressDialog progressDialog("Calibrating path...", "Cancel", 1, numPoints);
            progressDialog.setWindowModality(Qt::ApplicationModal);
            progressDialog.setMinimumDuration(0);
            progressDialog.setFixedSize(progressDialog.sizeHint());

            // Clear the cal points
            m_calPoints.clear();

            // Loop through all of the frequencies - stop if user presses cancel
            for (uint64_t freq = m_startFreq; (freq <= m_stopFreq) && !cancelled; freq += m_stepFreq)
            {
                common::CalPoint point;
                point.freq = freq;
                bool pointAccepted = false;

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
                        retVal &= te::TestEquipmentStore::powerMeter()->getPower(point.loss);
                        point.loss *= -1;
                        point.loss += m_powerLevel;
                        qApp->processEvents();
                    }

                    // Is this point acceptable?
                    if (point.loss <= kMaxPathLoss)
                    {
                        pointAccepted = true;
                    }
                    else
                    {
                        gui::MessageDialog pointDialog(QString("Loss too high (%1 dB)"
                                                        "<P>Check cables and press OK to retry, otherwise cancel the calibration")
                                                        .arg(point.loss),
                                                        gui::MessageDialogType::ERR);
                        if (!pointDialog.exec()) { cancelled = true; }
                    }
                } while(!cancelled && !pointAccepted);

                m_calPoints.append(point);
                progressDialog.setValue(progressDialog.value() + 1);

                cancelled |= progressDialog.wasCanceled();
            }
        }

        // Disable signal generator RF output
        retVal &= te::TestEquipmentStore::signalGenerator()->setRfEnabled(false);

        return retVal;
    }

    double CableCal::correctedPower(uint64_t freq, double power) const
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

    double CableCal::correctedPower(RfPath& path, uint64_t freq, double power)
    {
        CableCal cal(path.key());

        return cal.correctedPower(freq, power);
    }

    bool CableCal::storeCal() const
    {
        return common::Settings::instance()->setCalPoints(m_calKey, m_calPoints);
    }

    bool CableCal::restoreCal()
    {
        return common::Settings::instance()->getCalPoints(m_calKey, m_calPoints);
    }

    const QVector<common::CalPoint>& CableCal::calPoints()
    {
        return m_calPoints;
    }
}
