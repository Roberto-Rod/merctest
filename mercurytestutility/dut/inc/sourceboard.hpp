#ifndef SOURCEBOARD_HPP
#define SOURCEBOARD_HPP

#include "deviceundertest.hpp"
#include "sourceboardbit.hpp"
#include "sourceboardcomms.hpp"
#include "port.hpp"
#include "settings.hpp"

#include <QFile>

namespace comms
{
    class RegisterControl;
    class RegisterComms;
}

namespace hardware
{
    class PaControlMonitor;
}

namespace dut
{
    class SourceBoard : public DeviceUnderTest
    {
        Q_OBJECT
    public:
        enum RfPort
        {
            PORT_1,
            PORT_2
        };

        enum RfAtt
        {
            DB_0,
            DB_15,
            DB_16,
            DB_31
        };

        enum TamperSource
        {
            EXT_SWITCH,
            LIGHT_SENSOR
        };

        SourceBoard(testjig::TestJig* parentJig = 0);

        ~SourceBoard();

        bool connect();

        void disconnect();

        bool isConnected() const;

        DutState state() const;

        bool waitReady() const;

        bool breakInBootloader() const;

        bool exitBootloader() const;

        bool unlockBootloader() const;

        bool sendBootloaderConfig(QString config) const;

        bool readBootloaderConfig(QString& config) const;

        bool readBootloaderSerial(QString& serial) const;

        bool sendBootloaderSerial(QString serial) const;

        bool sendBootloaderDdsStore() const;

        bool sendBootloaderFileList(QString& filelist) const;

        bool resetTamperState() const;

        bool resetTamperState(TamperSource source) const;

        bool resetTamperState(TamperSource source, bool chargeT1Cap) const;

        bool clearTamperState() const;

        bool blSetMechSwitchTamper() const;

        bool getFreshTamperState(TamperSource source, bool& state) const;

        bool getLatchedTamperState(TamperSource source, bool& state) const;

        bool getOscillatorDisabledState(bool& state) const;

        bool getFpgaVersion(QString& version) const;

        bool getClockCountPps(uint32_t& count) const;

        bool setTcxoTrimDac(uint16_t dacVal) const;

        bool readDockRegister(hardware::Port port, uint16_t addr, uint32_t& data) const;

        bool getDaughterId(uint8_t& id) const;

        bool getDaughterAttLUT(uint16_t i, uint16_t& att) const;

         bool setDaughterAttLUT(uint16_t i, uint32_t& att32) const;

        bool enableDaughter5V5(bool en) const;

        bool getDaughter5V5Good(bool& good) const;

        bool disableBlankSources() const;

        bool setBlankOutMask(bool en) const;

        bool setBlankIntMask(bool en) const;

        bool initialiseSynth() const;

        bool getDdsSyncClkCnt(uint32_t& count) const;

        static uint32_t expectedDdsSyncClkCnt();

        bool setRfOutputPort(RfPort port) const;

        bool setRfAttenuation(RfAtt att) const;

        static uint32_t quarterDbToAsf(uint8_t quarterDb);

        bool setRfBlank(bool blank) const;

        bool setDaughterOutputPort(RfPort port) const;

        bool setDaughterRfPath(uint8_t path) const;

        bool setDaughterAttenuation(uint8_t quarterDb);

        bool getDaughterAttenuation(uint32_t& att);

        bool setDdsCw(uint64_t freq, uint8_t attQuarterDb = 0u) const;

        bool setDdsCwUsingAsf(uint64_t freq, uint32_t  asf) const;

        bool getBit(SourceBoardBit& bit) const;

        bool copyFileToSdCard(QFile& srcFile, QString destFileName, comms::FileMethod method = comms::FileMethod::MAIN_FW_FDOWNLOAD) const;

        bool copyFileOnSdCard(QString srcFileName, QString destFileName) const;

        bool getFileCrc(QString fileName, uint16_t& crc);

        static QString imageBriefDescription(common::FirmwareImage img);

        static QString imageFullDescription(common::FirmwareImage img);

        static QString imageDestFileName(common::FirmwareImage img);

        hardware::PaControlMonitor* paCtrlMonitor(hardware::Port port) const;

        comms::RegisterControl* dutRegs() const;

    private:
        static const uint64_t kSynthFreq                = 3240000000ull;    //!< Synth frequency = 3240 MHz
        static const uint32_t kRegClockCountPps         = 0x000005;
        static const uint32_t kRegTcxoTrimDacI2cWrite   = 0x00000c;
        static const uint32_t kRegDdsBase               = 0x000100;
        static const uint32_t kRegDdsCtrl               = 0x000140;
        static const uint32_t kRegDdsSyncClkCnt         = 0x000141;
        static const uint32_t kRegDdsIoUpdate           = 0x000142;
        static const uint32_t kRegSynthCtrl             = 0x000150;
        static const uint32_t kRegSynthWrite            = 0x000151;
        static const uint32_t kRegRfCtrl                = 0x000160;
        static const uint32_t kRegDgtrCtrl              = 0x000161;
        static const uint32_t kRegBlankCtrl             = 0x000162;
        static const uint32_t kRegPaCtrlA               = 0x000170;
        static const uint32_t kRegOffsetDockA           = 0x100000;
        static const uint32_t kRegDblAttenuator         = 0x000600;

        static const uint8_t  kDblAttLsb                = 0u;               //!< Attenuator look up table
        static const uint8_t  kDblAttWidth              = 9u;

        static const uint8_t  kDgtrCtrlBit5V5En         = 0u;               //!< Daughter Board Control register: Enable 5V5 Rail
        static const uint8_t  kDgtrCtrlBit5V5Gd         = 27u;              //!< Daughter Board Control register: 5V5 Rail Good
        static const uint8_t  kDgtrCtrlBitDgtrIdLsb     = 28u;              //!< Daughter Board Control register: 5V5 Rail Good
        static const uint8_t  kDgtrCtrlDgtrIdWidth      = 4u;               //!< Daughter Board Control register: 5V5 Rail Good

        static const uint8_t  kPaCtrlBitPwrMonEn        = 0u;               //!< PA Control register: Power Monitor Enable
        static const uint8_t  kPaCtrlBitShdn            = 1u;               //!< PA Control register: Shutdown
        static const uint8_t  kPaCtrlBitMute            = 2u;               //!< PA Control register: Mute
        static const uint8_t  kPaCtrlBitAlert           = 31u;              //!< PA Control register: Alert

        static const uint8_t  kRfCtrlBitSrcPort         = 0u;
        static const uint8_t  kRfCtrlBitDgtrPort        = 1u;
        static const uint8_t  kRfCtrlBitDgtrPathLsb     = 4u;
        static const uint8_t  kRfCtrlBitSrcBlank        = 9u;
        static const uint8_t  kRfCtrlBitSrcAttenLsb     = 12u;
        static const uint8_t  kRfCtrlBitDgtrAttenLsb    = 16u;

        static const uint8_t  kRfCtrlDgtrPathWidth      = 3u;
        static const uint8_t  kRfCtrlSrcAttenWidth      = 2u;
        static const uint8_t  kRfCtrlDgtrAttenWidth     = 8u;

        static const uint8_t  kBlankCtrlBitIntMaskExt   = 0u;               //!< Selects whether to route external blanking input to internal blanking signal or not
        static const uint8_t  kBlankCtrlBitExtMaskExt   = 4u;               //!< Selects whether to route external blanking input to external blanking output or not

        static const uint32_t kBlBreakInPreDelay        = 500ul;            //!< Bootloader break-in delay between power-up and sending ctrl-c, milliseconds

        static const uint32_t kBlUnlockMagic            = 0x75ED301F;       //!< Magic number to XOR with serial number to get unlock code

        comms::SourceBoardComms* m_pDutComms;
        hardware::PaControlMonitor* m_pPaCtrlMon[2];                        //!< Pointer to PA Control/Monitor objects
    };
}

#endif // SOURCEBOARD_HPP
