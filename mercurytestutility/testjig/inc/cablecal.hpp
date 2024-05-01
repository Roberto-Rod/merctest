#ifndef CABLECAL_HPP
#define CABLECAL_HPP

#include "calibration.hpp"
#include "rfpath.hpp"

#include <cstdint>

#include <QString>
#include <QDate>
#include <QObject>

namespace testjig
{
    class CableCal : public QObject, public common::Calibration
    {
        Q_OBJECT
    public:
        CableCal(QString key = "", QObject* parent = 0);

        void setDescription(QString description);

        QString description() const;

        void setTestPowerLevel(double powerLevel);

        bool calibrate(bool& cancelled, const QVector<common::CalPoint>& correction);

        bool calibrate(bool& cancelled);

        double correctedPower(uint64_t freq, double power) const;

        static double correctedPower(testjig::RfPath& path, uint64_t freq, double power);

        bool storeCal() const;

        bool restoreCal();

        const QVector<common::CalPoint>& calPoints();

        static const uint32_t kCalValidDays     = 28u;           //!< Number of days cal remains valid for
        static const uint32_t kCalWarnDays      = 2u;            //!< Number of days before expiry to start warning

    private:
        static constexpr double kMaxPathLoss    = 25.0;          //!< Maximum path loss allowed during cable calibration
        static const uint64_t kStartFreqDefault = 20000000ull;   //!< Default start frequency = 20 MHz
        static const uint64_t kStopFreqDefault  = 6000000000ull; //!< Default stop frequency = 6000 MHz
        static const uint64_t kStepFreqDefault  = 6500000ull;    //!< Default stop frequency = 6.5 MHz (921 steps)

        QVector<common::CalPoint> m_calPoints;

        QString m_description;

        uint64_t m_startFreq;
        uint64_t m_stopFreq;
        uint64_t m_stepFreq;
        double m_powerLevel;                                     //!< Signal generator power to use (dBm) when calibrating path
    };

}

#endif // CABLECAL_HPP
