#ifndef DUMMYSPECTRUMANALYSER_HPP
#define DUMMYSPECTRUMANALYSER_HPP

#include "spectrumanalyser.hpp"
#include "testequipment.hpp"

namespace te
{
    class DummySpectrumAnalyser : public ISpectrumAnalyser,
                                  public TestEquipment
    {
    public:
        DummySpectrumAnalyser();
        DummySpectrumAnalyser(const QString& resourceName);
        ~DummySpectrumAnalyser();

        ResourceType resourceType() { return SpectrumAnalyser; }
        QString resourceTypeString() { return QObject::tr("Spectrum Analyser"); }

        bool reset();
        bool connect();

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
        uint64_t m_centreFrequency;
        uint64_t m_span;
        uint64_t m_resBw;
        double   m_refLevel;
        DbPerDiv m_scale;
        bool     m_contTrigger;
    };
}

#endif // DUMMYSPECTRUMANALYSER_HPP
