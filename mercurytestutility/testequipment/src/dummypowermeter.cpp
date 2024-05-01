#include "dummypowermeter.hpp"

#include <QDebug>

namespace te
{
    DummyPowerMeter::DummyPowerMeter() :
        TestEquipment()
    {
        m_detected = true;
    }

    DummyPowerMeter::DummyPowerMeter(const QString& resourceName)
    {
        m_resourceName = resourceName;
        connect();
    }

    DummyPowerMeter::~DummyPowerMeter()
    {
        qDebug() << "Delete DummyPowerMeter";
    }

    bool DummyPowerMeter::reset()
    {
        return true;
    }

    bool DummyPowerMeter::connect()
    {
        bool retVal = false;

        if (m_resourceName == "RSNRP::0x0003::104502::INSTR")
        {
            // Apply dummy details
            setDetails("ROHDE&SCHWARZ,NRP-Z21,012347,04.16");

            retVal = true;
        }
        else
        {
            // Simulate a connection failure and reset the details
            resetDetails();
        }

        return retVal;
    }

    bool DummyPowerMeter::zeroSensor()
    {
        return true;
    }

    bool DummyPowerMeter::setOffset(double offset)
    {
        m_offset = offset;

        return true;
    }

    bool DummyPowerMeter::getOffset(double& offset)
    {
        offset = m_offset;

        return true;
    }

    bool DummyPowerMeter::setFrequency(uint64_t freq)
    {
        m_freq = freq;

        return true;
    }

    bool DummyPowerMeter::getFrequency(uint64_t& freq)
    {
        freq = m_freq;

        return true;
    }

    bool DummyPowerMeter::getPower(double& power)
    {
        // Keep a static variable and keep reporting 1dB higher each
        // time this method is called
        static double lastResult = -50.0;

        power = lastResult;
        lastResult += 1.0;

        return true;
    }
}
