#ifndef VISASPECTRUMANALYSER_HPP
#define VISASPECTRUMANALYSER_HPP

#include "spectrumanalyser.hpp"
#include "visatestequipment.hpp"
#include "visaresource.hpp"

#include <QMap>

namespace te
{
    class VisaSpectrumAnalyser : public ISpectrumAnalyser,
                                 public VisaTestEquipment
    {
    public:
        VisaSpectrumAnalyser(const QString& resourceName = "");
        ~VisaSpectrumAnalyser();

        ResourceType resourceType() { return SpectrumAnalyser; }
        QString resourceTypeString() { return QObject::tr("Spectrum Analyser"); }

        bool reset();

        bool setFrequency(uint64_t freq);
        bool getFrequency(uint64_t& freq);

        bool setStartFrequency(uint64_t freq);
        bool setStopFrequency(uint64_t freq);

        bool setSpan(uint64_t span);
        bool getSpan(uint64_t& span);

        bool setResBw(uint64_t resBw);
        bool getResBw(uint64_t& resBw);

        bool setRefLevel(double refLevel);
        bool getRefLevel(double& refLevel);

        bool setDbPerDiv(DbPerDiv scale);
        bool getDbPerDiv(DbPerDiv& scale);

        bool setContTrigger(bool cont);
        bool getContTrigger(bool& cont);

        bool getPeak(uint64_t& freq, double& power);
        bool getPeakPower(double& power);
        bool getPeakFrequency(uint64_t& freq);

        bool getPeak(bool next, uint64_t& freq, double& power);
        bool getPeakPower(bool next, double& power);
        bool getPeakFrequency(bool next, uint64_t& freq);

    private:
        bool markerFindPeak();
        bool markerFindPeakNext();

        void initDbDivMap();

        static const QStringList kExpectedModels;
        static const QStringList kResetCmds;
        static const QString kSetFrequency;
        static const QString kSetStartFrequency;
        static const QString kSetStopFrequency;
        static const QString kGetFrequency;
        static const QString kSetSpan;
        static const QString kGetSpan;
        static const QString kSetResBw;
        static const QString kGetResBw;
        static const QString kSetRefLevel;
        static const QString kGetRefLevel;
        static const QString kSetDbPerDiv;
        static const QString kGetDbPerDiv;
        static const QString kSetContTrig;
        static const QString kGetContTrig;
        static const QString kTriggerSweep;
        static const QString kMarkerPeak;
        static const QString kMarkerPeakNext;
        static const QString kGetMarkerFreq;
        static const QString kGetMarkerPwr;

        QMap<DbPerDiv, QString> m_dbDivMap;
    };
}

#endif // VISASPECTRUMANALYSER_HPP
