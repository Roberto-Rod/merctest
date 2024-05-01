#include "sourceboard.hpp"
#include "registercontrol.hpp"
#include "registercomms.hpp"
#include "registergpio.hpp"
#include "directserialcomms.hpp"
#include "sourceboardcomms.hpp"
#include "settings.hpp"
#include "mercurytestjig1.hpp"
#include "testharnesssourceboard.hpp"
#include "dutpowercontrol.hpp"
#include "testcasesourceboard.hpp"
#include "logging.hpp"
#include "testequipmentstore.hpp"
#include "pacontrolmonitor.hpp"
#include "assyrevision.hpp"

#include <climits>
#include <QThread>

namespace dut
{
    SourceBoard::SourceBoard(testjig::TestJig* parentJig) :
        DeviceUnderTest(parentJig),
        m_pDutComms(new comms::SourceBoardComms(new comms::DirectSerialComms(this), this))
    {
        m_variants.append(new Variant("DDS Source Board", "PCB0044")); // The single variant
        m_pTopLevelTestCase = new testsuite::TestCaseSourceBoard(this);
        m_pTestHarness = new testjig::TestHarnessSourceBoard(parentJig->registerComms(), this);
        m_pRevision = new common::AssyRevision("DDS Source Board","");

        // Create PA control/monitor objects - note that there is only one port supported at present
        // so reuse port A pointer for port B
        m_pPaCtrlMon[0] = new hardware::PaControlMonitor(m_pDutComms, this);
        m_pPaCtrlMon[1] = m_pPaCtrlMon[0];
    }

    SourceBoard::~SourceBoard()
    {
    }

    bool SourceBoard::connect()
    {
        bool retVal = false;

        // Disconnect current session
        disconnect();

        // Can we communicate with the DUT
        if (m_pDutComms->serialComms()->openPort(common::Settings::instance()->srcPortName()))
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

    void SourceBoard::disconnect()
    {
        m_pDutComms->serialComms()->closePort();
    }

    bool SourceBoard::isConnected() const
    {
        return m_pDutComms->serialComms()->isPortOpen();
    }

    DutState SourceBoard::state() const
    {
        // First assume that we cannot communicate with the jig/dut
        DutState retVal = DutState::NO_COMMS;

        if (isConnected())
        {
            // We have comms - promote state to no power
            retVal = DutState::NO_POWER;

            // Is the power enabled?
            bool en = false;
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

    bool SourceBoard::waitReady() const
    {
        DutState currState = state();

        // If state is ready then immediately return true
        bool retVal = (currState == DutState::READY);

        // Only if state is INITIALISING is it worth waiting
        if (currState == DutState::INITIALISING)
        {
            // Now wait for it to be ready
            retVal = m_pDutComms->waitReady();
        }

        return retVal;
    }

    bool SourceBoard::breakInBootloader() const
    {
        bool retVal = false;

        // Power cycle the DUT then...
        if (m_pTestHarness->powerCycle(2))
        {
            // wait a bit, then...
              QThread::msleep(750);

            // ...attempt break in
            retVal = m_pDutComms->breakInBootloader();
        }

        return retVal;
    }

    bool SourceBoard::exitBootloader() const
    {
        // Send the exit command
        return m_pDutComms->sendCommand("x", "Starting main firmware...");
    }

    bool SourceBoard::unlockBootloader() const
    {
        QString serial;

        bool retVal = false;

        if (readBootloaderSerial(serial))
        {
            // The decimal serial number (which will be 8 digits [0-9]) is treated as if it is
            // a hex number so s/n 12345678 is treated as 0x12345678.
            uint32_t code = serial.toUInt(&retVal, 16);

            if (retVal)
            {
                // XOR the "hexified" serial number with the magic number for tip-top security
                code ^= kBlUnlockMagic;

                retVal = m_pDutComms->sendCommand(QString("u %1").arg(code, 8, 16, QChar('0')),
                                                   comms::SourceBoardComms::kBootloaderTerminator);
            }
        }

        return retVal;
    }

    bool SourceBoard::sendBootloaderConfig(QString config) const
    {
        return m_pDutComms->sendCommand("cw " + config, comms::SourceBoardComms::kBootloaderTerminator);
    }

    bool SourceBoard::blSetMechSwitchTamper() const
    {
        return m_pDutComms->sendCommand("tk", comms::SourceBoardComms::kBootloaderTerminator);
    }

    bool SourceBoard::readBootloaderConfig(QString& config) const
    {
         return m_pDutComms->sendCommand("cr", config, comms::SourceBoardComms::kBootloaderTerminator);
    }

    bool SourceBoard::readBootloaderSerial(QString& serial) const
    {
        return m_pDutComms->sendCommand("sr", serial, comms::SourceBoardComms::kBootloaderTerminator);
    }

    bool SourceBoard::sendBootloaderSerial(QString serial) const
    {
        bool retVal = false;

        if (serial.length() <= 8)
        {
            // Write the serila number and zero-pad to 8 characters
            retVal = m_pDutComms->sendCommand("sw " + serial.rightJustified(8, '0'), comms::SourceBoardComms::kBootloaderTerminator);
        }

        return retVal;
    }

    bool SourceBoard::sendBootloaderDdsStore() const
    {
        // Bootloader command to store dds_cal.csv to top of flash (32K). This is the application area.
        return m_pDutComms->sendCommand("ks 1 ", comms::SourceBoardComms::kBootloaderTerminator);
    }

    bool SourceBoard::sendBootloaderFileList(QString& filelist) const
    {
        return m_pDutComms->sendCommand("fl", filelist, comms::SourceBoardComms::kBootloaderTerminator);
    }

    bool SourceBoard::resetTamperState() const
    {
        // Reset both sources
        return (resetTamperState(TamperSource::EXT_SWITCH, true) && resetTamperState(TamperSource::LIGHT_SENSOR, false));
    }

     bool SourceBoard::resetTamperState(TamperSource source) const
     {
         bool retVal;
         retVal = resetTamperState(source, true);
         return retVal;
     }

    // Note only set / clear TEB in Tamper register, test bootloader /config
    // bytes have set up the register correctly
    bool SourceBoard::resetTamperState(TamperSource source, bool chargeT1Cap) const
    {
        bool retVal = false;
        QString regAddr;
        (void)chargeT1Cap;

        // Set the register address which controls the appropriate tamper source
        if      (source == TamperSource::EXT_SWITCH)   { regAddr = "14 "; }
        else if (source == TamperSource::LIGHT_SENSOR) { regAddr = "15 "; }

        if (!regAddr.isEmpty())
        {
            QString resp;
            retVal = true;

            // Test configuration set up by bootloader (ignore TEB bit)
            retVal &= m_pDutComms->sendCommand("tr " + regAddr, resp, comms::SourceBoardComms::kBootloaderTerminator);
            retVal &= (resp == "25"  || resp == "a5");
            resp = "25";

            // Disable the tamper detection to clear the tamper state
            retVal &= m_pDutComms->sendCommand("tw " + regAddr + resp, comms::SourceBoardComms::kBootloaderTerminator);

            // The following (commented out) is now replaced by the bootloader "tk" command which wiil
            // enable tamper using the normally closed tamper high configuration in order to charge
            // the capacitor of the low pass filter without causing a tamper.

           /* if(chargeT1Cap &&  regAddr == "14 ")
            {
            // Enable Tamper pull up but tamper on high to let capacitor charge
                resp = "95";
                retVal &= m_pDutComms->sendCommand("tw " + regAddr + resp, comms::SourceBoardComms::kBootloaderTerminator);

                QThread::msleep(2000);

                // Clear tamper, set mode to tamper low.
                resp = "25";
                // Disable the tamper detection to clear the tamper state
                retVal &= m_pDutComms->sendCommand("tw " + regAddr + resp, comms::SourceBoardComms::kBootloaderTerminator);
            }  */

            // Re-enable tamper detection
            resp = "a5";
            retVal &= m_pDutComms->sendCommand("tw " + regAddr + resp, comms::SourceBoardComms::kBootloaderTerminator);

            // Read the control register to check the IC is working
            retVal &= m_pDutComms->sendCommand("tr " + regAddr, resp, comms::SourceBoardComms::kBootloaderTerminator);
            retVal &= (resp == "a5");
        }

        return retVal;
    }

    bool SourceBoard::clearTamperState() const
    {
        bool retVal = false;
        QString regAddr;
        QString resp;

        retVal = true;
        bool tamperState;

        // Disable tamper detection channel 1
        regAddr = "14";
        retVal &= m_pDutComms->sendCommand("tw " + regAddr + " 25", comms::SourceBoardComms::kBootloaderTerminator);

        // Read the control register to check the IC is working
        retVal &= m_pDutComms->sendCommand("tr " + regAddr, resp, comms::SourceBoardComms::kBootloaderTerminator);
        retVal &= (resp == "25");

        // Check tamper state for channel 1 is clear
        getLatchedTamperState(dut::SourceBoard::TamperSource::EXT_SWITCH, tamperState);
        retVal &= !tamperState;

        // Disable tamper detection for channel 2
        regAddr = "15";
        retVal &= m_pDutComms->sendCommand("tw " + regAddr + " 25", comms::SourceBoardComms::kBootloaderTerminator);

        // Read the control register to check the IC is working
        retVal &= m_pDutComms->sendCommand("tr " + regAddr, resp, comms::SourceBoardComms::kBootloaderTerminator);
        retVal &= (resp == "25");

        // Check tamper state for channel 2 is clear
        getLatchedTamperState(dut::SourceBoard::TamperSource::LIGHT_SENSOR, tamperState);
        retVal &= !tamperState;

        return retVal;
    }

    bool SourceBoard::getFreshTamperState(TamperSource source, bool& state) const
    {
        // Reset the tamper state and then re-get it
        return (resetTamperState(source) && getLatchedTamperState(source, state));
    }

    bool SourceBoard::getLatchedTamperState(TamperSource source, bool& state) const
    {
        bool retVal = true;

        QString resp;

        // Read the tamper status
        retVal &= m_pDutComms->sendCommand("tr 0f", resp, comms::SourceBoardComms::kBootloaderTerminator);

        // Convert the response to an 8-bit unsigned
        bool ok;
        uint8_t val = resp.toUShort(&ok, 16);

        // Now get the tamper status from the respective one of the two LSBs
        if      (source == TamperSource::EXT_SWITCH)   { state = ((val & 0x02) != 0u); }
        else if (source == TamperSource::LIGHT_SENSOR) { state = ((val & 0x01) != 0u); }

        return retVal;
    }

    bool SourceBoard::getOscillatorDisabledState(bool& state) const
    {
        bool retVal = true;

        QString resp;

        // Read the tamper status
        retVal &= m_pDutComms->sendCommand("tr 01", resp, comms::SourceBoardComms::kBootloaderTerminator);

        // Convert the response to an 8-bit unsigned
        bool ok;
        uint8_t val = resp.toUShort(&ok, 16);

        // Read the ST bit from the 10s seconds / seconds register
        state = ((val & 0x80));

        return retVal;
    }

    bool SourceBoard::getFpgaVersion(QString& version) const
    {
        return m_pDutComms->sendCommand("fpga version", version, comms::SourceBoardComms::kStandardTerminator);
    }

    bool SourceBoard::getClockCountPps(uint32_t& count) const
    {
        return m_pDutComms->readRegister(kRegClockCountPps, count);
    }

    bool SourceBoard::setTcxoTrimDac(uint16_t dacVal) const
    {
        uint32_t val = static_cast<uint32_t>(dacVal) & 0x0000FFFF;
        return m_pDutComms->writeRegister(kRegTcxoTrimDacI2cWrite,  val);
    }

    bool SourceBoard::readDockRegister(hardware::Port port, uint16_t addr, uint32_t& data) const
    {
        bool retVal = false;
        uint32_t offset = 0u;

        if (port == hardware::Port::A) { offset = kRegOffsetDockA; }

        if (offset != 0)
        {
            // If there is a dock comms fault between DUT and the Test Jig then
            // readRegister will return false, but we want the return value of this method
            // to represent the status of the comms between the PC and the DUT. So, perform the
            // dock read, if it fails then zero the data and then return the status of the
            // write method.
            if (!m_pDutComms->readRegister(offset + addr, data))
            {
                data = 0u;
            }

            // Use the write method to determine PC to Jig comms status. The dock write will be
            // ignore by the Test Jig on the dock port.
            retVal = m_pDutComms->writeRegister(offset + addr, data);
        }

        return retVal;
    }

    bool SourceBoard::getDaughterAttLUT(uint16_t i, uint16_t& att) const
    {
        // Get Doubler table attenuation table setting
        hardware::RegisterGpio gpio(m_pDutComms, kRegDblAttenuator + i);
        uint32_t att32;
        bool retVal = gpio.getData(kDblAttLsb,  kDblAttWidth, att32);
        att = static_cast<uint16_t>(att32 & 0x1FF);

        return retVal;
    }

    bool SourceBoard::setDaughterAttLUT(uint16_t i, uint32_t& att32) const
    {
        // Get Doubler table attenuation table setting
        hardware::RegisterGpio gpio(m_pDutComms, kRegDblAttenuator + i);
        bool retVal = gpio.setData(kDblAttLsb,  kDblAttWidth, att32);

        return retVal;
    }

    bool SourceBoard::getDaughterId(uint8_t& id) const
    {
        // Get ID
        hardware::RegisterGpio gpio(m_pDutComms, kRegDgtrCtrl);
        uint32_t id32;
        bool retVal = gpio.getData(kDgtrCtrlBitDgtrIdLsb, kDgtrCtrlDgtrIdWidth, id32);
        id = static_cast<uint8_t>(id32 & 0xFF);

        return retVal;
    }

    bool SourceBoard::enableDaughter5V5(bool en) const
    {
        // Enable/disable 5V5 supply
        hardware::RegisterGpio gpio(m_pDutComms, kRegDgtrCtrl);
        return gpio.setData(kDgtrCtrlBit5V5En, en);
    }

    bool SourceBoard::getDaughter5V5Good(bool& good) const
    {
        // Get 5V5 PGOOD state
        hardware::RegisterGpio gpio(m_pDutComms, kRegDgtrCtrl);
        return gpio.getData(kDgtrCtrlBit5V5Gd, good);
    }

    bool SourceBoard::disableBlankSources() const
    {
        // Disable all blanking sources
        return m_pDutComms->writeRegister(kRegBlankCtrl, 0x0);
    }

    bool SourceBoard::setBlankOutMask(bool en) const
    {
        // Set the EXT_BLANK_MASK_EXT bit
        hardware::RegisterGpio gpio(m_pDutComms, kRegBlankCtrl);

        return gpio.setData(kBlankCtrlBitExtMaskExt, en);
    }

    bool SourceBoard::setBlankIntMask(bool en) const
    {
        // Set the INT_BLANK_MASK_EXT bit
        hardware::RegisterGpio gpio(m_pDutComms, kRegBlankCtrl);

        return gpio.setData(kBlankCtrlBitIntMaskExt, en);
    }

    bool SourceBoard::initialiseSynth() const
    {
        bool retVal = false;

        // Set TCXO trim DAC to mid-scale (+1.65V)
        // for reasonable 10MHz accuracy
        if (setTcxoTrimDac(0x8ce4))
        {
            retVal = true;

            // Set synth to 3240 MHz
            retVal &= m_pDutComms->writeRegister(kRegSynthCtrl,   0x00000003);
            retVal &= m_pDutComms->writeRegister(kRegDdsCtrl,     0x00000001);
            retVal &= m_pDutComms->writeRegister(kRegSynthWrite,  0x00580005);
            retVal &= m_pDutComms->writeRegister(kRegSynthWrite,  0x008a003c);
            retVal &= m_pDutComms->writeRegister(kRegSynthWrite,  0x000004b3);
            retVal &= m_pDutComms->writeRegister(kRegSynthWrite,  0x62005e42);
            retVal &= m_pDutComms->writeRegister(kRegSynthWrite,  0x00008641);
            retVal &= m_pDutComms->writeRegister(kRegSynthWrite,  0x00510000);
            retVal &= m_pDutComms->writeRegister(kRegDdsCtrl,     0x00000000);
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 3, 0x01052120);
            retVal &= m_pDutComms->writeRegister(kRegDdsIoUpdate, 0x00000000);
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 3, 0x00052120);
            retVal &= m_pDutComms->writeRegister(kRegDdsBase,     0x00010108);
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 1, 0x00800900);
            retVal &= m_pDutComms->writeRegister(kRegDdsIoUpdate, 0x00000000);
        }

        return retVal;
    }

    bool SourceBoard::getDdsSyncClkCnt(uint32_t& count) const
    {
        return m_pDutComms->readRegister(kRegDdsSyncClkCnt, count);
    }

    uint32_t SourceBoard::expectedDdsSyncClkCnt()
    {
        // AD9914 sync clock frequency is synth frequency divided by 24
        return static_cast<uint32_t>(kSynthFreq / 24ull);
    }

    bool SourceBoard::setRfOutputPort(RfPort port) const
    {
        // 0 = Port 1, 1 = Port 2
        bool val = (port == PORT_2);

        hardware::RegisterGpio gpio(m_pDutComms, kRegRfCtrl);

        return gpio.setData(kRfCtrlBitSrcPort, val);
    }

    bool SourceBoard::setRfAttenuation(RfAtt att) const
    {
        // Attenuation bits are 13:12 in RF control register
        // “00”= 31dB
        // “01”= 15dB
        // “10”= 16dB
        // “11”= 0dB

        uint32_t val = 0x0000;

        if (att == RfAtt::DB_31)      { val = 0x0000; }
        else if (att == RfAtt::DB_15) { val = 0x1000; }
        else if (att == RfAtt::DB_16) { val = 0x2000; }
        else if (att == RfAtt::DB_0)  { val = 0x3000; }

        return m_pDutComms->setBitField(kRegRfCtrl, 0x3000, val);
    }

    bool SourceBoard::setRfBlank(bool blank) const
    {
        // Blank control is bit 9 in RF Control register.
        // 0 = Not Blanking, 1 = Blanking
        uint32_t val = 0u;
        if (blank) { val = 0x200; }

        return m_pDutComms->setBitField(kRegRfCtrl, 0x200, val);
    }

    bool SourceBoard::setDaughterOutputPort(RfPort port) const
    {
        // 0 = Port 1, 1 = Port 2
        bool val = (port == PORT_2);

        hardware::RegisterGpio gpio(m_pDutComms, kRegRfCtrl);

        return gpio.setData(kRfCtrlBitDgtrPort, val);
    }

    bool SourceBoard::setDaughterRfPath(uint8_t path) const
    {
        hardware::RegisterGpio gpio(m_pDutComms, kRegRfCtrl);

        return gpio.setData(kRfCtrlBitDgtrPathLsb, kRfCtrlDgtrPathWidth, static_cast<uint32_t>(path) & 0xFF);
    }

    bool SourceBoard::setDaughterAttenuation(uint8_t quarterDb)
    {
        hardware::RegisterGpio gpio(m_pDutComms, kRegRfCtrl);

        return gpio.setData(kRfCtrlBitDgtrAttenLsb, kRfCtrlDgtrAttenWidth, static_cast<uint32_t>(quarterDb) & 0xFF);
    }

    bool SourceBoard::getDaughterAttenuation(uint32_t& att)
    {
        hardware::RegisterGpio gpio(m_pDutComms, kRegRfCtrl);

        return gpio.getData(kRfCtrlBitDgtrAttenLsb, kRfCtrlDgtrAttenWidth, att);
    }

    bool  SourceBoard::setDdsCwUsingAsf(uint64_t freq, uint32_t  asf) const
     {
        bool retVal = false;

        // DDS frequency tuning word = f_out * 2^32 / f_dac
        // Perform multiplication in 64-bit uint first to maintain precision
        uint64_t ftw = freq * (1ull + ULONG_MAX);
        ftw /= kSynthFreq;

        // Test that the output fits into 32-bits
        if (ftw <= ULONG_MAX)
        {
            retVal = true;

            // Write the FTW and ASF to the DDS and command an IO update
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 0xb, static_cast<uint32_t>(ftw));
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 0xc, (asf << 16));
            retVal &= m_pDutComms->writeRegister(kRegDdsIoUpdate, 0u);
        }

        return retVal;
     }

    bool SourceBoard::setDdsCw(uint64_t freq, uint8_t attQuarterDb) const
    {
        bool retVal = false;

        // DDS frequency tuning word = f_out * 2^32 / f_dac
        // Perform multiplication in 64-bit uint first to maintain precision
        uint64_t ftw = freq * (1ull + ULONG_MAX);
        ftw /= kSynthFreq;

        // Test that the output fits into 32-bits
        if (ftw <= ULONG_MAX)
        {
            retVal = true;

            uint32_t asf = quarterDbToAsf(attQuarterDb);

            // Write the FTW and ASF to the DDS and command an IO update
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 0xb, static_cast<uint32_t>(ftw));
            retVal &= m_pDutComms->writeRegister(kRegDdsBase + 0xc, (asf << 16));
            retVal &= m_pDutComms->writeRegister(kRegDdsIoUpdate, 0u);
        }

        return retVal;
    }

    bool SourceBoard::getBit(SourceBoardBit& bit) const
    {
        bool retVal = false;

        QString cmd = "bit";
        QString resp;

        bit.clear();

        if (m_pDutComms->sendCommand(cmd, resp, comms::SourceBoardComms::kStandardTerminator))
        {
            // The format of the response is
            // EcmBit, RAIL_VIN, <mv>, RAIL_1V2, <mv>, RAIL_1V8, <mv>, RAIL_3V3, <mv>, RAIL_5V5, <mv>, CURRENT, <ma>, DDS TEMP, <degC>, INT TEMP, <degC>, Dock BIT, 0d

            QStringList parts = resp.split(",");

            // Loop for all of the parts, stopping at the penultimate part as we need
            // to read values ahead of the field names. Stop if return value becomes false.
            retVal = true;
            for (int i = 0; ((i + 1) < parts.length()) && retVal; i++)
            {
                QString field = parts.at(i).trimmed();
                QString value = parts.at(i + 1);  // This is where value will be if we are on a valid field

                if      (field == "RAIL_VIN") { bit.m_railVin     = value.toUInt(&retVal); }
                else if (field == "RAIL_1V2") { bit.m_rail1V2     = value.toUInt(&retVal); }
                else if (field == "RAIL_1V8") { bit.m_rail1V8     = value.toUInt(&retVal); }
                else if (field == "RAIL_3V3") { bit.m_rail3V3     = value.toUInt(&retVal); }
                else if (field == "RAIL_5V5") { bit.m_rail5V5     = value.toUInt(&retVal); }
                else if (field == "CURRENT") { bit.m_current     = value.toUInt(&retVal); }
                else if (field == "INT TEMP") { bit.m_intTemp     = value.toInt(&retVal); }
                else if (field == "DDS TEMP") { bit.m_ddsTemp     = value.toInt(&retVal); }
            }

            retVal = true;
        }

        return retVal;
    }

    bool SourceBoard::copyFileToSdCard(QFile& srcFile, QString destFileName, comms::FileMethod method) const
    {
        return m_pDutComms->sendFile(srcFile, destFileName, method);
    }

    bool SourceBoard::copyFileOnSdCard(QString srcFileName, QString destFileName) const
    {
        bool retVal = false;

        // We need to set a long timeout for this operation

        QString cmd = "rm " + srcFileName;

        if (m_pDutComms->sendCommand(cmd))
        {
            cmd = "fcopy " + srcFileName + " " + destFileName;

            if (m_pDutComms->sendCommand(cmd))
            {
                retVal = true;
            }
        }

        return retVal;
    }

    bool SourceBoard::getFileCrc(QString fileName, uint16_t& crc)
    {
        bool retVal = false;

        // We need to set a long timeout for this operation

        QString cmd = "fcrc " + fileName;
        QString resp;

        if (m_pDutComms->sendCommand(cmd, resp, comms::SourceBoardComms::kStandardTerminator))
        {
            resp.remove("0x");
            crc = resp.toUShort(&retVal, 16);
        }

        return retVal;
    }

    uint32_t SourceBoard::quarterDbToAsf(uint8_t quarterDb)
    {
        // ratio = 10^((quarterdB/-4) / 20))
        double ratio = pow(10.0, (static_cast<double>(quarterDb) / -4.0 / 20.0));

        // Amplitide scale factor = 4095 * ratio
        uint32_t asf = static_cast<uint32_t>((ratio * 4095.0) + 0.5);

        return asf;
    }

    QString SourceBoard::imageBriefDescription(common::FirmwareImage img)
    {
        QString str;

        if (img == common::FirmwareImage::SRC_BRD_MAIN)
        {
            str = "ARM Firmware";
        }
        else if (img == common::FirmwareImage::SRC_BRD_BOOT)
        {
            str = "ARM Bootloader";
        }
        else if (img == common::FirmwareImage::SRC_BRD_FPGA)
        {
            str = "FPGA Firmware";
        }

        return str;
    }

    QString SourceBoard::imageFullDescription(common::FirmwareImage img)
    {
        QString str;

        if (img == common::FirmwareImage::SRC_BRD_MAIN)
        {
            str = "SW0027 (Mercury ECM Firmware)";
        }
        else if (img == common::FirmwareImage::SRC_BRD_BOOT)
        {
            str = "SW0012 (Mercury ECM and Ventura MCM Bootloader)";
        }
        else if (img == common::FirmwareImage::SRC_BRD_FPGA)
        {
            str = "SW0008 (DDS Source Board (PCB0044) FPGA Firmware)";
        }

        return str;
    }

    QString SourceBoard::imageDestFileName(common::FirmwareImage img)
    {
        QString str;

        // Only respond for images that have a filename on SD card
        // (bootloader has no filename as it is not on SD card)
        if (img == common::FirmwareImage::SRC_BRD_MAIN)
        {
            str = "lpc_fw.bin";
        }
        else if (img == common::FirmwareImage::SRC_BRD_FPGA)
        {
            str = "fpga.bin";
        }

        return str;
    }

    hardware::PaControlMonitor* SourceBoard::paCtrlMonitor(hardware::Port port) const
    {
        hardware::PaControlMonitor* retVal = 0;

        if (port == hardware::Port::A)      { retVal = m_pPaCtrlMon[0]; }
        else if (port == hardware::Port::B) { retVal = m_pPaCtrlMon[1]; }

        return retVal;
    }

    comms::RegisterControl* SourceBoard::dutRegs() const
    {
        return static_cast<comms::RegisterControl*>(m_pDutComms);
    }
}
