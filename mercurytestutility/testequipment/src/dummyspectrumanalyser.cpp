#include "dummyspectrumanalyser.hpp"

#include <QDebug>

namespace te
{
    DummySpectrumAnalyser::DummySpectrumAnalyser() :
        TestEquipment(),
        m_centreFrequency(0ull),
        m_span(0ull),
        m_resBw(0ull),
        m_refLevel(0.0),
        m_scale(DbPerDiv::DB_10),
        m_contTrigger(false)
    {
        m_detected = true;
    }

    DummySpectrumAnalyser::DummySpectrumAnalyser(const QString& resourceName)
    {
        m_resourceName = resourceName;
        connect();
    }

    DummySpectrumAnalyser::~DummySpectrumAnalyser()
    {
        qDebug() << "Delete DummySpectrumAnalyser";
    }

    bool DummySpectrumAnalyser::reset()
    {
        m_centreFrequency = 0ull;
        m_span = 0ull;
        m_resBw = 0ull;

        return true;
    }

    bool DummySpectrumAnalyser::connect()
    {
        bool retVal = false;

        if (m_resourceName == "TCPIP0::192.168.0.17::inst0::INSTR")
        {
            // Apply dummy details
            setDetails("Keysight Technologies,N9342C,012345,v1.2");

            retVal = true;
        }
        else
        {
            // Simulate a connection failure and reset the details
            resetDetails();
        }

        return retVal;
    }

    bool DummySpectrumAnalyser::setFrequency(uint64_t freq)
    {
        m_centreFrequency = freq;
        return true;
    }

    bool DummySpectrumAnalyser::setStartFrequency(uint64_t freq)
    {
        // m_centreFrequency = freq;
        (void)freq;
        return true;
    }

    bool DummySpectrumAnalyser::setStopFrequency(uint64_t freq)
    {
        // m_centreFrequency = freq;
        (void)freq;
        return true;
    }

    bool DummySpectrumAnalyser::getFrequency(uint64_t& freq)
    {
        freq = m_centreFrequency;
        return true;
    }

    bool DummySpectrumAnalyser::setSpan(uint64_t span)
    {
        m_span = span;
        return true;
    }

    bool DummySpectrumAnalyser::getSpan(uint64_t& span)
    {
        span = m_span;
        return true;
    }

    bool DummySpectrumAnalyser::setResBw(uint64_t resBw)
    {
        m_resBw = resBw;
        return true;
    }

    bool DummySpectrumAnalyser::getResBw(uint64_t& resBw)
    {
        resBw = m_resBw;
        return true;
    }

    bool DummySpectrumAnalyser::setRefLevel(double refLevel)
    {
        m_refLevel = refLevel;

        return true;
    }

    bool DummySpectrumAnalyser::getRefLevel(double& refLevel)
    {
        refLevel = m_refLevel;

        return true;
    }

    bool DummySpectrumAnalyser::setDbPerDiv(DbPerDiv scale)
    {
        m_scale = scale;

        return true;
    }

    bool DummySpectrumAnalyser::getDbPerDiv(DbPerDiv& scale)
    {
        scale = m_scale;

        return true;
    }

    bool DummySpectrumAnalyser::setContTrigger(bool cont)
    {
        m_contTrigger = cont;

        return true;
    }

    bool DummySpectrumAnalyser::getContTrigger(bool& cont)
    {
        cont = m_contTrigger;

        return true;
    }

    bool DummySpectrumAnalyser::getPeak(uint64_t& freq, double& power)
    {
        bool retVal = true;

        retVal &= getPeakFrequency(freq);
        retVal &= getPeakPower(power);

        return true;
    }

    bool DummySpectrumAnalyser::getPeakPower(double& power)
    {
        // Return an arbitrary dummy figure...
        power = 10.0;
        return true;
    }

    bool DummySpectrumAnalyser::getPeakFrequency(uint64_t& freq)
    {
        // Return an arbitrary dummy figure...
        freq = m_centreFrequency + 10ull;
        return true;
    }

    // Annoying "dummy" routines!
    bool DummySpectrumAnalyser::getPeak(bool next, uint64_t& freq, double& power)
    {
        bool retVal = true;

        (void)next;
        retVal &= getPeakFrequency(freq);
        retVal &= getPeakPower(power);

        return true;
    }

    bool DummySpectrumAnalyser::getPeakPower(bool next, double& power)
    {
        // Return an arbitrary dummy figure...
        power = 10.0;
        (void)next;
        return true;
    }

    bool DummySpectrumAnalyser::getPeakFrequency(bool next, uint64_t& freq)
    {
        // Return an arbitrary dummy figure...
        freq = m_centreFrequency + 10ull;
        (void)next;
        return true;
    }

}
