#ifndef CALIBRATIONSTATUS_HPP
#define CALIBRATIONSTATUS_HPP

#include <QIcon>

namespace common
{
    class CalibrationStatus
    {
    public:
        virtual ~CalibrationStatus() {}

        virtual QDate calExpiry() const = 0;

        virtual bool isCalExpired() const = 0;

        virtual bool isCalNearlyExpired() const = 0;

        QIcon calStatusIcon() const;

        QString calStatusText() const;
    };
}

#endif // CALIBRATIONSTATUS_HPP
