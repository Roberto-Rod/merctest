#include "psumodule.hpp"
#include "testcasepsumodule.hpp"
#include "testharnesspsumodule.hpp"
#include "testjig.hpp"
#include "registercomms.hpp"
#include "registergpio.hpp"
#include "assyrevision.hpp"

namespace dut
{
    PsuModule::PsuModule(testjig::TestJig* parentJig) :
        DeviceUnderTest(parentJig),
        m_pPsuComms(new comms::PsuComms(parentJig->registerComms(), this))
    {
        m_variants.append(new Variant("PSU Module", "PCB0049/PCB0058"));  // The generic variant
        m_variants.append(new PsuModuleVariant("Manpack PSU Module", "PCB0049", PsuModuleType::MANPACK));
        m_variants.append(new PsuModuleVariant("Vehicle PSU Module", "PCB0058", PsuModuleType::VEHICLE));
        m_pRevision = new common::AssyRevision("Null Assembly","");

        m_pTopLevelTestCase = new testsuite::TestCasePsuModule(this);
        m_pTestHarness = new testjig::TestHarnessPsuModule(parentJig->registerComms(), this);
    }

    PsuModule::~PsuModule()
    {
    }

    bool PsuModule::connect()
    {
        bool retVal = false;

        // Reset the testjig I2C master
        if (m_pPsuComms->resetI2cMaster())
        {
            // Reset the DUT registers
            retVal = m_pPsuComms->writeRegister(kRegAddrReset, kResetMagicNumber);

            // Delay to allow the register reset to complete
            QThread::sleep(kResetPostDelay);
        }

        return retVal;
    }

    void PsuModule::disconnect()
    {
    }

    bool PsuModule::isConnected() const
    {
        return true;
    }

    DutState PsuModule::state() const
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

    bool PsuModule::waitReady() const
    {
        // No waiting to do - it's ready or not (here I come)
        return (state() == DutState::READY);
    }

    PsuModuleType PsuModule::type() const
    {
        PsuModuleType retVal(PsuModuleType::UNKNOWN);

        const PsuModuleVariant* variant = dynamic_cast<const PsuModuleVariant*>(selectedVariant());

        if (variant != 0)
        {
            retVal = variant->m_type;
        }

        return retVal;
    }

    bool PsuModule::getSwVersion(uint8_t& major, uint8_t& minor) const
    {
        bool retVal = false;

        hardware::RegisterGpio gpio(m_pPsuComms, static_cast<uint16_t>(kRegAddrSwVersion) & 0xFF, kPsuRegsWidth);

        uint32_t maj32, min32;
        if ((gpio.getData(kSwVersMajLsb, kSwVersMajWidth, maj32)) &&
             (gpio.getData(kSwVersMinLsb, kSwVersMinWidth, min32)))
        {
            major = static_cast<uint8_t>(maj32 & 0xFF);
            minor = static_cast<uint8_t>(min32 & 0xFF);
            retVal = true;
        }

        return retVal;
    }

    bool PsuModule::getType(PsuModuleType& type) const
    {
        bool retVal = false;

        hardware::RegisterGpio gpio(m_pPsuComms, static_cast<uint16_t>(kRegAddrSwVersion) & 0xFF, kPsuRegsWidth);

        bool val;
        if (gpio.getData(kSwVersTypeBit, val))
        {
            if (val) { type = PsuModuleType::VEHICLE; }
            else       { type = PsuModuleType::MANPACK; }

            retVal = true;
        }

        return retVal;
    }

    bool PsuModule::setPsuEnable(bool en) const
    {
        hardware::RegisterGpio gpio(m_pPsuComms, static_cast<uint16_t>(kRegAddrControl) & 0xFF, kPsuRegsWidth);

        return gpio.setData(kControlPsuEnBit, en);
    }

    bool PsuModule::setVoltage(uint16_t millivolts) const
    {
        bool retVal = true;

        uint16_t val = millivoltsToRegister(millivolts, kVoltsOutSetScale);

        retVal &= m_pPsuComms->writeRegister(kRegAddrVOutSetMsb, static_cast<uint8_t >((val>> 8) & 0xFF));
        retVal &= m_pPsuComms->writeRegister(kRegAddrVOutSetLsb, static_cast<uint8_t>((val) & 0xFF));

        return retVal;
    }

    bool PsuModule::setFanEnable(Fan fan, bool en) const
    {
        uint16_t regAddr = 0u;

        if (fan == Fan::FAN_1)      { regAddr = static_cast<uint16_t>(kRegAddrFan1Ctrl) & 0xFF; }
        else if (fan == Fan::FAN_2) { regAddr = static_cast<uint16_t>(kRegAddrFan2Ctrl) & 0xFF; }

        hardware::RegisterGpio gpio(m_pPsuComms, regAddr, kPsuRegsWidth);

        return gpio.setData(kFanCtrlFanEnBit, en);
    }

    bool PsuModule::setFanEnable(bool en) const
    {
        return (setFanEnable(Fan::FAN_1, en) && setFanManual(Fan::FAN_2, en));
    }

    bool PsuModule::setFanManual(Fan fan, bool manual) const
    {
        uint16_t regAddr = 0u;

        if (fan == Fan::FAN_1)      { regAddr = static_cast<uint16_t>(kRegAddrFan1Ctrl) & 0xFF; }
        else if (fan == Fan::FAN_2) { regAddr = static_cast<uint16_t>(kRegAddrFan2Ctrl) & 0xFF; }

        hardware::RegisterGpio gpio(m_pPsuComms, regAddr, kPsuRegsWidth);

        // Set bit (invert manual as '1' is auto, '0' is manual)
        return gpio.setData(kFanCtrlManualBit, !manual);
    }

    bool PsuModule::setFanManual(bool manual) const
    {
        return (setFanManual(Fan::FAN_1, manual) && setFanManual(Fan::FAN_2, manual));
    }

    bool PsuModule::setShutdownMode(ShutdownMode mode) const
    {
        hardware::RegisterGpio gpio(m_pPsuComms, static_cast<uint16_t>(kRegAddrControl) & 0xFF, kPsuRegsWidth);

        uint32_t data = static_cast<uint32_t>(mode);
        return gpio.setData(kControlShdnLsb, kControlShdnWidth, data);
    }

    bool PsuModule::readVoltage(VoltageSource source, uint16_t& millivolts) const
    {
        bool retVal = false;

        uint8_t regAddrLsb = 0ul;
        uint16_t scale = 0ul;
        bool singleByte = false;

        if (source == VoltageSource::INPUT)         { regAddrLsb = kRegAddrVInMeasLsb;  scale = kVoltsInMeasScale; }
        else if (source == VoltageSource::OUTPUT)   { regAddrLsb = kRegAddrVOutMeasLsb; scale = kVoltsOutMeasScale; }
        else if (source == VoltageSource::RAIL_3V3) { regAddrLsb = kRegAddr3V3Meas;     scale = kRail3V3MeasScale; singleByte = true; }
        else if (source == VoltageSource::RAIL_12V) { regAddrLsb = kRegAddr12VMeas;     scale = kRail12VMeasScale; singleByte = true; }

        if (scale != 0ul)
        {
            uint8_t data;

            // Read the LSB
            retVal = m_pPsuComms->readRegister(regAddrLsb, data);
            uint16_t regVal = static_cast<uint16_t>(data);

            // If this is a 16-bit reading then read MSB
            if (!singleByte)
            {
                retVal &= m_pPsuComms->readRegister(--regAddrLsb, data);
                regVal |= static_cast<uint16_t>(data) << 8;
            }

            millivolts = registerToMillivolts(regVal, scale);
        }

        return retVal;
    }

    bool PsuModule::readCurrent(uint16_t& milliamps) const
    {
        bool retVal = false;

        uint8_t data;

        // Read the register
        retVal = m_pPsuComms->readRegister(kRegAddrCurrMeas, data);
        uint16_t regVal = static_cast<uint16_t>(data);

        // Use the register to millivolts method as conversion to milliamps is equivalent
        milliamps = registerToMillivolts(regVal, kCurrOutMeasScale);

        return retVal;
    }

    bool PsuModule::readTemperature(TemperatureSource source, int8_t& degreesC) const
    {
        bool retVal = false;

        uint8_t regAddr = 0u;

        if (source == TemperatureSource::LOCAL)           { regAddr = kRegAddrTempLocal; }
        else if (source == TemperatureSource::REMOTE)     { regAddr = kRegAddrTempRemote; }
        else if (source == TemperatureSource::THERMISTOR) { regAddr = kRegAddrThermistor; }

        if (regAddr != 0u)
        {
            // Read the register
            uint8_t data;
            retVal = m_pPsuComms->readRegister(regAddr, data);

            degreesC = static_cast<int8_t>(data);
        }

        return retVal;
    }

    bool PsuModule::readFanTacho(Fan fan, uint8_t& tachoReading) const
    {
        bool retVal = false;

        uint8_t regAddr = 0u;

        if (fan == Fan::FAN_1)      { regAddr = kRegAddrTacho1; }
        else if (fan == Fan::FAN_2) { regAddr = kRegAddrTacho2; }

        if (regAddr != 0u)
        {
            // Read the tacho register
            retVal = m_pPsuComms->readRegister(regAddr, tachoReading);
        }

        return retVal;
    }

    uint16_t PsuModule::millivoltsToRegister(uint16_t millivolts, uint16_t millivoltsPerLsb)
    {
        double val = static_cast<double >(millivolts) / static_cast<double>(millivoltsPerLsb);
        val = std::round(val);

        return static_cast<uint16_t>(val);
    }

    uint16_t PsuModule::registerToMillivolts(uint16_t regVal, uint16_t millivoltsPerLsb)
    {
        // Note: multiplying 16-bit * 16-bit could overflow 16-bit results so perform multiplication
        // in 32-bit and then limit to 16-bit range. In-system, there should never be an overflow as
        // the top of the 16-bit unsigned range gives 65.535 Volts - higher than any voltages in system.
        uint32_t millivolts = static_cast<uint32_t >(regVal) * static_cast<uint32_t>(millivoltsPerLsb);

        if (millivolts > LONG_MAX)
        {
            millivolts = LONG_MAX;
        }

        return millivolts;
    }

    bool PsuModule::enableFan1Alarm(bool en) const
    {
        // Set the Tacho Enable bit in Fan 1 Control Register and
        // set the Fan 1 Fail bit in Alert Control Register 2
        hardware::RegisterGpio fanCtrl(m_pPsuComms, static_cast<uint16_t>(kRegAddrFan1Ctrl) & 0xFF, kPsuRegsWidth);
        hardware::RegisterGpio alertCtrl(m_pPsuComms, static_cast<uint16_t>(kRegAddrAlertCtrl2) & 0xFF, kPsuRegsWidth);

        return (fanCtrl.setData(kFanCtrlTachoEnBit, en) &&
                 alertCtrl.setData(kAlertCtrl2Fan1, en));
    }

    bool PsuModule::clearAlarms() const
    {
        // Perform a dummy write to alarm 1 register to clear alarms
        return m_pPsuComms->writeRegister(kRegAddrAlarm1, static_cast<uint8_t>(0u));
    }
}
