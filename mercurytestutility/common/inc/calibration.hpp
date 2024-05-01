#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include "calibrationstatus.hpp"

#include <QString>
#include <QDate>
#include <QVector>

namespace common
{
    struct CalPoint
    {
        uint64_t freq;
        double loss;
    };

    class Calibration : public CalibrationStatus
    {
    public:
        Calibration(QString key = "");

        ~Calibration() {}

        void setCalKey(QString key);

        void setWarnDays(uint32_t days);

        bool isCalKeyValid() const;

        void setCalExpiry(const QDate& expiry) const;

        QDate calExpiry() const;

        /**
         * @brief isCalExpired
         *
         * Tests if calibration expiry date has passed by testing m_calExpiry against
         * the current system date. Calibration is deemed valid up to and including
         * the calibration expiry date.
         *
         * @return true if calibration expiry date has passed
         */
        bool isCalExpired() const;

        static bool isCalExpired(const QDate& date);

        /**
         * @brief isCalNearlyExpired
         * @return true if calibration expiry date is within period defined by kCalWarningWeeks
         */
        bool isCalNearlyExpired() const;

        bool isCalNearlyExpired(const QDate& date) const;

        QString status() const;

        static bool correctCal(QVector<CalPoint>& calPoints, const QVector<CalPoint>& correctionPoints);

    protected:
        QString m_calKey;

        uint32_t m_warnDays;

        static const uint32_t kWarnDaysDefault = 14u;    //!< Number of days before expiry to start warning
    };
}

#endif // CALIBRATION_HPP
