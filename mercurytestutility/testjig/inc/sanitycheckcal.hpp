#ifndef SANITYCHECKCAL_HPP
#define SANITYCHECKCAL_HPP

#include "calibration.hpp"
#include "rfpath.hpp"

#include <cstdint>

#include <QString>
#include <QDate>
#include <QObject>

namespace testjig
{
    struct sanityResult {
          double maxErrNeg;
          double maxErrPos;
    };

    enum  SanityCheckType
    {
        SOUCEBOARD,
        POWERMONITER,
        HIGHBAND_POWERMETER,
        HIGHBAND_SIGGEN,
    };

    class SanityCheckCal : public QObject, public common::Calibration
    {
        Q_OBJECT
    public:

        SanityCheckCal(QString key = "", QObject* parent = 0);

        void setDescription(QString description);

        QString description() const;

        void setTestPowerLevel(double powerLevel);

        bool testRfPower(bool& cancelled);

        bool testRfPower(testjig::SanityCheckType checktype, bool& cancelled,
                         RfPathHighBandModule::RfCableSet  cableSet = RfPathHighBandModule::RfCableSet::UNKNOWN,
                         uint64_t stopFreq = kStopFreqDefault);

        QString measureInstruction(RfPathHighBandModule::RfCableSet cableSet) const;

        QString measurePowerMonitorInstruction() const;

        QString measureTestJigInstruction() const;

        double correctedPower(uint64_t freq, double power) const;

        static double correctedPower(testjig::RfPath& path, uint64_t freq, double power);

        bool storeCal() const;

        bool restoreCal();

        const QVector<common::CalPoint>& calPoints();

        static const uint32_t kCalValidDays = 28u; //!< Number of days cal remains valid for
        static const uint32_t kCalWarnDays  = 2u;  //!< Number of days before expiry to start warning
        static const uint64_t KStopFreqSourceBoard = 3000000000ull;

    private:
        static constexpr double kMaxPathLoss    = 25.0;           //!< Maximum path loss allowed during cable calibration
        static const uint64_t kStartFreqDefault = 20000000ull;    //!< Default start frequency = 20 MHz
        static const uint64_t kStopFreqDefault  = 6000000000ull;  //!< Default stop frequency = 6000 MHz
        static const uint64_t kStepFreqDefault  = 6500000ull * 3; //!< Default step frequency = 19.5 MHz (921 / 3 steps)

        QVector<common::CalPoint> m_calPoints;

        QString m_description;

        uint64_t m_startFreq;
        uint64_t m_stopFreq;
        uint64_t m_stepFreq;
        double m_powerLevel; //!< Signal generator power to use (dBm) when calibrating path
    };

}

Q_DECLARE_METATYPE(testjig::sanityResult)

#endif // SANITYCHECKCAL_HPP
