#include "ctsboard.hpp"
#include "testcasectsboard.hpp"
#include "testharnessctsboard.hpp"
#include "testjig.hpp"
#include "assyrevision.hpp"
#include "ctsboardcomms.hpp"
#include "registercomms.hpp"
#include "directserialcomms.hpp"

#include <limits>

namespace dut
{
    bool CtsBoard::BitStatus::allOk() const
    {
        return m_procInitOk &&
               m_bspOk &&
               m_synthOk &&
               m_flashMissionFileOk &&
               m_missionOk &&
               m_tamperDeviceOk &&
               m_internalFlashOk;
    }

    void CtsBoard::BitStatus::clear()
    {
        m_procInitOk = false;
        m_bspOk = false;
        m_synthOk = false;
        m_flashMissionFileOk = false;
        m_missionOk = false;
        m_tamperDeviceOk = false;
        m_internalFlashOk = false;
    }

    CtsBoard::CtsBoard(testjig::TestJig* parentJig) :
        m_pDutComms(new comms::CtsBoardComms(new comms::DirectSerialComms(this), this))
    {
        m_variants.append(new Variant("Confidence Test Set Board", "PCB0114")); // The single variant
        m_pTopLevelTestCase = new testsuite::TestCaseCtsBoard(this);
        m_pTestHarness = new testjig::TestHarnessCtsBoard(parentJig->registerComms(), this);
        m_pRevision = new common::AssyRevision("Null Assembly","");
    }

    CtsBoard::~CtsBoard()
    {
    }

    bool CtsBoard::connect()
    {
        bool retVal(false);

        // Disconnect current session
        disconnect();

        // Can we communicate with the DUT
        if (m_pDutComms->serialComms()->openPort(common::Settings::instance()->ctsPortName()))
        {
            // We've opened a port to the DUT - check that it is connected
            retVal = isConnected();
        }

        // If something went wrong then close the port
        if (!retVal)
        {
            m_pDutComms->serialComms()->closePort();
        }

        return retVal;
    }

    void CtsBoard::disconnect()
    {
        m_pDutComms->serialComms()->closePort();
    }

    bool CtsBoard::isConnected() const
    {
        return m_pDutComms->serialComms()->isPortOpen();
    }

    DutState CtsBoard::state() const
    {
        // First assume that we cannot communicate with the jig/dut
        DutState retVal = DutState::NO_COMMS;

        if (isConnected())
        {
            // We have comms - promote state to no power
            retVal = DutState::NO_POWER;

            // Is the power enabled?
            bool en(false);

            if (m_pTestHarness->isPowerEnabled(en) && en)
            {
                // If we got this far then assume the board is at least initialising
                retVal = DutState::INITIALISING;

                // Now see if the DUT is ready
                if (m_pDutComms->isReady())
                {
                    // If we get this far then the board is ready
                    retVal = DutState::READY;
                }
            }
        }

        return retVal;
    }

    bool CtsBoard::waitReady() const
    {
        DutState currState(state());

        // If state is ready then immediately return true
        bool retVal(currState == DutState::READY);

        // Only if state is INITIALISING is it worth waiting
        if (currState == DutState::INITIALISING)
        {
            // Now wait for it to be ready
            retVal = m_pDutComms->waitReady();
        }

        return retVal;
    }

    bool CtsBoard::getBitStatus(BitStatus& status) const
    {
        QString resp;
        bool retVal(false);

        if (m_pDutComms->sendCommand("bs", resp))
        {
            QStringList parts(resp.split(", "));
            QString part;

            status.clear();
            retVal = true;

            foreach(part, parts)
            {
                QStringList elem(part.split(" "));
                if (elem.length() == 2)
                {
                    bool ok;
                    uint32_t value(elem.at(1).toUInt(&ok));

                    if (ok && value <= 1)
                    {
                        bool state(value);

                        if (elem.at(0).compare("prc") == 0)
                        {
                            status.m_procInitOk = state;
                        }
                        else if (elem.at(0).compare("bsp") == 0)
                        {
                            status.m_bspOk = state;
                        }
                        else if (elem.at(0).compare("ifl") == 0)
                        {
                            status.m_internalFlashOk = state;
                        }
                        else if (elem.at(0).compare("flm") == 0)
                        {
                            status.m_flashMissionFileOk = state;
                        }
                        else if (elem.at(0).compare("mis") == 0)
                        {
                            status.m_missionOk = state;
                        }
                        else if (elem.at(0).compare("syn") == 0)
                        {
                            status.m_synthOk = state;
                        }
                        else if (elem.at(0).compare("tmp") == 0)
                        {
                            status.m_tamperDeviceOk = state;
                        }
                    }
                    else
                    {
                        retVal = false;
                    }
                }
            }
        }

        return retVal;
    }

    bool CtsBoard::getVersion(QString& version) const
    {
        return m_pDutComms->sendCommand("nn", version);
    }

    bool CtsBoard::getBatteryVoltage(uint16_t& millivolts) const
    {
        return m_pDutComms->sendCommand("vb", millivolts);
    }

    bool CtsBoard::getTamperVoltage(uint16_t& millivolts) const
    {
        return m_pDutComms->sendCommand("vt", millivolts);
    }

    bool CtsBoard::getProcessorTemperature(int16_t& degreesC) const
    {
        return m_pDutComms->sendCommand("pt", degreesC);
    }

    bool CtsBoard::getChargeStatus(ChargeStatus& status) const
    {
        return m_pDutComms->sendCommand("cs", *reinterpret_cast<uint8_t*>(&status));
    }

    bool CtsBoard::getChargerFaultStatus(bool& fault) const
    {
        return m_pDutComms->sendCommand("cf", fault);
    }

    bool CtsBoard::getRfPowerReading(uint16_t& millivolts) const
    {
        return m_pDutComms->sendCommand("rm", millivolts);
    }

    bool CtsBoard::getRfPowerReading(float& dBm) const
    {
        return m_pDutComms->sendCommand("rd", dBm);
    }

    bool CtsBoard::getTamperStatus(TamperChannel ch, bool& tampered) const
    {
        return m_pDutComms->sendCommand(QString("ms %1").arg(static_cast<uint8_t>(ch)), tampered);
    }

    bool CtsBoard::getUsbPowerStatus(bool& present) const
    {
        return m_pDutComms->sendCommand("up", present);
    }

    bool CtsBoard::setInterfaceOverride(bool override) const
    {
        return m_pDutComms->sendCommand(QString("io %1").arg(static_cast<uint8_t>(override)));
    }

    bool CtsBoard::setInterfaceLed(Led led) const
    {
        return m_pDutComms->sendCommand(QString("il %1").arg(1 << static_cast<uint8_t>(led)));
    }

    bool CtsBoard::isButtonPressed(Button button) const
    {
        uint8_t mask;
        bool ok(m_pDutComms->sendCommand("ib", mask));
        return ok && (mask & (1 << static_cast<uint8_t>(button)));
    }

    bool CtsBoard::resetTamperDevice() const
    {
        return m_pDutComms->sendCommand("mr");
    }

    bool CtsBoard::setRfPath(RfPath path) const
    {
        return m_pDutComms->sendCommand(QString("rp %1").arg(static_cast<uint8_t>(path)));
    }

    bool CtsBoard::setIfPath(IfPath path) const
    {
        return m_pDutComms->sendCommand(QString("ri %1").arg(static_cast<uint8_t>(path)));
    }

    bool CtsBoard::setRfFrequency(uint32_t freq_kHz) const
    {
        return m_pDutComms->sendCommand(QString("rf %1").arg(freq_kHz));
    }

    bool CtsBoard::setLoFrequency(uint32_t freq_kHz) const
    {
        return m_pDutComms->sendCommand(QString("rl %1").arg(freq_kHz));
    }

    bool CtsBoard::enableRfPower(bool en) const
    {
        return m_pDutComms->sendCommand(QString("re %1").arg(en ? "1" : "0"));
    }

    bool CtsBoard::writeSerialNumber(uint32_t sn) const
    {
        return m_pDutComms->sendCommand(QString("sw %1").arg(sn));
    }

    bool CtsBoard::readSerialNumber(uint32_t& sn) const
    {
        return m_pDutComms->sendCommand("sr", sn);
    }

    bool CtsBoard::killDcPower() const
    {
        return m_pDutComms->sendCommand("k");
    }
}
