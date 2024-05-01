#include "calibrationstatus.hpp"

#include <QDate>

namespace common
{
    QIcon CalibrationStatus::calStatusIcon() const
    {
        // Create the icon object to represent cal status
        QIcon calIcon;

        if (isCalExpired())
        {
            calIcon.addFile(":icons/16x16/dialog-error-4.png");
        }
        else if (isCalNearlyExpired())
        {
            calIcon.addFile(":icons/16x16/dialog-warning.png");
        }

        return calIcon;
    }

    QString CalibrationStatus::calStatusText() const
    {
        // Create the string object to represent cal status
        QString calToolTip;

        if (isCalExpired())
        {
            calToolTip = "Calibration Expired on " + calExpiry().toString("dd/MM/yyyy");
        }
        else if (isCalNearlyExpired())
        {
            int64_t daysToExpiry = QDate::currentDate().daysTo(calExpiry());

            if (daysToExpiry == 0ull)
            {
                calToolTip = "Calibration Expires Today";
            }
            else if (daysToExpiry == 1ull)
            {
                calToolTip = "Calibration Expires Tomorrow";
            }
            else
            {
                calToolTip = "Calibration Expires on " + calExpiry().toString("dd/MM/yyyy");
            }
        }

        return calToolTip;
    }
}
