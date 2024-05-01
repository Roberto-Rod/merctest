#include "calibration.hpp"
#include "settings.hpp"

namespace common
{
    Calibration::Calibration(QString key) :
        m_calKey(key),
        m_warnDays(kWarnDaysDefault)
    {
    }

    void Calibration::setCalKey(QString key)
    {
        m_calKey = key;
    }

    void Calibration::setWarnDays(uint32_t days)
    {
        m_warnDays = days;
    }

    bool Calibration::isCalKeyValid() const
    {
        // If the cal key is not an empty string then it is valid
        return (m_calKey != "");
    }

    void Calibration::setCalExpiry(const QDate& expiry) const
    {
        // If we've got a valid key then save this cal expiry date
        if (isCalKeyValid())
        {
            common::Settings::instance()->setCalExpiry(m_calKey, expiry);
        }
    }

    QDate Calibration::calExpiry() const
    {
        QDate calExpiry;

        // Retrieve the cal expiry date. If key is not valid then let the method
        // being called set a default expired date
        common::Settings::instance()->getCalExpiry(m_calKey, calExpiry);

        return calExpiry;
    }

    bool Calibration::isCalExpired(const QDate& date)
    {
        return (date < QDate::currentDate());
    }

    bool Calibration::isCalExpired() const
    {
        return isCalExpired(calExpiry());
    }

    bool Calibration::isCalNearlyExpired(const QDate& date) const
    {
        return (date <= QDate::currentDate().addDays(m_warnDays));
    }

    bool Calibration::isCalNearlyExpired() const
    {
        return isCalNearlyExpired(calExpiry());
    }

    QString Calibration::status() const
    {
        QString status;

        if (isCalExpired())
        {
            status = "** EXPIRED **";
        }
        else
        {
            status = "Valid";

            if (isCalNearlyExpired())
            {
                status += " (" + QString::number(QDate::currentDate().daysTo(calExpiry())) + " days to expiry)";
            }
        }

        return status;
    }

    bool Calibration::correctCal(QVector<CalPoint>& calPoints, const QVector<CalPoint>& correctionPoints)
    {
        bool retVal = false;

        if (calPoints.length() == correctionPoints.length())
        {
            retVal = true;

            // Loop around all cal points unless any of the frequencies don't match,
            // in which case set return value to false and stop looping
            for (int i = 0; (i < calPoints.length()) && retVal; i++)
            {
                if (calPoints[i].freq == correctionPoints[i].freq)
                {
                    // Correct the cal point with the correction point
                    // Lower the cal point's loss by the loss in the
                    // correction point
                    calPoints[i].loss -= correctionPoints[i].loss;
                }
                else
                {
                    retVal = false;
                }
            }
        }

        return retVal;
    }
}
