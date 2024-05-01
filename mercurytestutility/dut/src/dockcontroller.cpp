#include "dockcontroller.hpp"
#include "testcasedockcontroller.hpp"
#include "testharnessdockcontroller.hpp"
#include "testjig.hpp"
#include "registercomms.hpp"
#include "registergpio.hpp"
#include "dockcontrollercomms.hpp"
#include "pacontrolmonitor.hpp"
#include "assyrevision.hpp"

#include <climits>

namespace dut
{
    DockController::DockController(testjig::TestJig* parentJig) :
        DeviceUnderTest(parentJig)
    {
        m_variants.append(new Variant("Ventura Dock Controller", "PCB0068"));        // The single variant
        m_pTopLevelTestCase = new testsuite::TestCaseDockController(this);
        m_pTestHarness = new testjig::TestHarnessDockController(parentJig->registerComms(), this);
        m_pRevision = new common::AssyRevision("Null Assembly","");

        // Make the dock controller comms objects
        m_pDockComms[0] = new comms::DockControllerComms(parentJig->registerComms(), kDockCh1Offset, this);
        m_pDockComms[1] = new comms::DockControllerComms(parentJig->registerComms(), kDockCh2Offset, this);

        // Create PA control/monitor objects - one per channel
        m_pPaCtrlMon[0] = new hardware::PaControlMonitor(m_pDockComms[0], this);
        m_pPaCtrlMon[1] = new hardware::PaControlMonitor(m_pDockComms[1], this);

        // Map enumerated ports to comms object indices
        m_portMap[hardware::Port::A] = 0;
        m_portMap[hardware::Port::B] = 1;
    }

    DockController::~DockController()
    {
    }

    bool DockController::connect()
    {
        return isConnected();
    }

    void DockController::disconnect()
    {
    }

    bool DockController::isConnected() const
    {
        return true;
    }

    DutState DockController::state() const
    {
        // Only two possible states for this DUT - either No Comms (with jig)
        // or Ready
        DutState retVal = DutState::NO_COMMS;

        if (isConnected())
        {
            retVal = DutState::READY;
        }

        return retVal;
    }

    bool DockController::waitReady() const
    {
        // No waiting to do - it's ready or not (here I come)
        return (state() == DutState::READY);
    }

    bool DockController::getFwVersion(QString& vers, hardware::Port port) const
    {
        hardware::RegisterGpio gpio(m_pDockComms[m_portMap[port]], kRegAddrFwVers, kWidthDockRegs);

        // Retrieve version numbers and format version string
        uint32_t maj, min, bld;
        bool retVal = (gpio.getData(kFwMajLsb, kFwMajWidth, maj) &&
                        gpio.getData(kFwMinLsb, kFwMinWidth, min) &&
                        gpio.getData(kFwBuildLsb, kFwBuildWidth, bld));

        vers = QString("v%1.%2.%3").arg(maj).arg(min).arg(bld);

        return retVal;
    }

    bool DockController::getHwVersion(QString& vers, hardware::Port port) const
    {
        hardware::RegisterGpio gpio(m_pDockComms[m_portMap[port]], kRegAddrHwVers, kWidthDockRegs);

        // Retrieve version numbers and format version string
        uint32_t maj, min;
        bool retVal = (gpio.getData(kHwMajLsb, kHwMajWidth, maj) &&
                        gpio.getData(kHwMinLsb, kHwMinWidth, min));

        vers = QString("v%1.%2").arg(maj).arg(min);

        return retVal;
    }

    bool DockController::getDrawingNumber(uint16_t& num, hardware::Port port) const
    {
        hardware::RegisterGpio gpio(m_pDockComms[m_portMap[port]], kRegAddrHwVers, kWidthDockRegs);

        // Retrieve drawing numbers and cast to 16-bit uint
        uint32_t data;
        bool retVal = (gpio.getData(kHwDwgNumLsb, kHwDwgNumWidth, data));

        num = static_cast<uint16_t>(data);

        return retVal;
    }

    bool DockController::getClockCount1Pps(uint32_t& count, hardware::Port port) const
    {
        return m_pDockComms[m_portMap[port]]->readRegister(kRegAddrClkCnt1Pps, count);
    }

    bool DockController::getEepromMfrDevId(uint16_t& id, hardware::Port port) const
    {
        uint32_t data;
        bool retVal = m_pDockComms[m_portMap[port]]->readRegister(kRegAddrEepromMfrId, data);
        id = static_cast<uint16_t>(data);
        return retVal;
    }

    bool DockController::getEepromUniqueId(uint32_t& id, hardware::Port port) const
    {
        return m_pDockComms[m_portMap[port]]->readRegister(kRegAddrEepromUId, id);
    }

    bool DockController::getBitStatus(uint16_t& status, hardware::Port port) const
    {
        uint32_t status32;
        bool retVal = m_pDockComms[m_portMap[port]]->readRegister(kRegAddrBitStatus, status32);

        status = static_cast<uint16_t>(status32);

        return retVal;
    }

    bool DockController::getBitVoltage(BitVoltage src, uint16_t& millivolts, hardware::Port port) const
    {
        bool retVal = false;

        uint16_t addr = 0u;
        double scaling = 0.0;

        if      (src == BitVoltage::VOLTS_IN) { addr = kRegAddrAdcVIn;    scaling = kScalingVIn; }
        else if (src == BitVoltage::GPS_3V0)  { addr = kRegAddrAdc3V0Gps; scaling = kScalingVInt; }
        else if (src == BitVoltage::INT_3V3)  { addr = kRegAddrAdc3V3Int; scaling = kScalingVInt; }
        else if (src == BitVoltage::INT_5V5)  { addr = kRegAddrAdc5V5Int; scaling = kScalingVInt; }

        if (addr != 0u)
        {
            uint32_t data;
            if (m_pDockComms[m_portMap[port]]->readRegister(addr, data))
            {
                double mv = std::round(static_cast<double>(data) * scaling);

                if (mv <= USHRT_MAX)
                {
                    millivolts = static_cast<uint16_t>(mv);
                    retVal = true;
                }
            }
        }

        return retVal;
    }

    bool DockController::getBitCurrent(uint16_t& milliamps, hardware::Port port) const
    {
        bool retVal = false;

        uint32_t data;
        if (m_pDockComms[m_portMap[port]]->readRegister(kRegAddrAdcCurrIn, data))
        {
            double ma = std::round(static_cast<double>(data) * kScalingCurr);

            if (ma <= USHRT_MAX)
            {
                milliamps = static_cast<uint16_t>(ma);
                retVal = true;
            }
        }

        return retVal;
    }

    bool DockController::getBitTemperature(int8_t& degC, hardware::Port port) const
    {
        bool retVal = false;

        uint32_t data;
        if (m_pDockComms[m_portMap[port]]->readRegister(kRegAddrAdcTemp, data))
        {
            // Sign extend the data
            if (data & 0x8000) { data |= 0xFFFF0000; }

            double temp = std::round(static_cast<double>(static_cast<int32_t>(data)) * kScalingTemp);

            if (temp <= UCHAR_MAX)
            {
                degC = static_cast<int8_t>(temp);
                retVal = true;
            }
        }

        return retVal;
    }

    hardware::PaControlMonitor* DockController::paCtrlMonitor(hardware::Port port) const
    {
        hardware::PaControlMonitor* retVal = 0;

        if (port == hardware::Port::A)      { retVal = m_pPaCtrlMon[0]; }
        else if (port == hardware::Port::B) { retVal = m_pPaCtrlMon[1]; }

        return retVal;
    }
}
