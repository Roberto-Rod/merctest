#include "dummysignalgenerator.hpp"

#include <QDebug>

namespace te
{
    DummySignalGenerator::DummySignalGenerator() :
        TestEquipment(),
        m_freq(0),
        m_power(-100),
        m_enabled(false)
    {
        m_detected = true;
    }

    DummySignalGenerator::DummySignalGenerator(const QString& resourceName) :
        DummySignalGenerator()
    {
        m_resourceName = resourceName;
        connect();
    }

    DummySignalGenerator::~DummySignalGenerator()
    {
        qDebug() << "Delete DummySignalGenerator";
    }

    bool DummySignalGenerator::reset()
    {
        return true;
    }

    bool DummySignalGenerator::connect()
    {
        bool retVal = false;

        if (m_resourceName == "GPIB0::19::INSTR")
        {
            // Apply dummy details
            setDetails("Agilent Technologies,N5181A,012346,v1.1.5_build_5389723785(main)");

            retVal = true;
        }
        else
        {
            // Simulate a connection failure and reset the details
            resetDetails();
        }

        return retVal;
    }

    bool DummySignalGenerator::setFrequency(uint64_t freq)
    {
        m_freq = freq;
        return true;
    }

    bool DummySignalGenerator::setFrequency(uint64_t freq, QString unit)
    {
        (void)unit;
        m_freq = freq;
        return true;
    }

    bool DummySignalGenerator::getFrequency(uint64_t& freq)
    {
        freq = m_freq;
        return true;
    }

    bool DummySignalGenerator::setOutputPower(double power)
    {
        m_power = power;
        return true;
    }

    bool DummySignalGenerator::getOutputPower(double& power)
    {
        power = m_power;
        return true;
    }

    bool DummySignalGenerator::setRfEnabled(bool en)
    {
        m_enabled = en;
        return true;
    }

    bool DummySignalGenerator::getRfEnabled(bool& en)
    {
        en = m_enabled;
        return true;
    }
}
