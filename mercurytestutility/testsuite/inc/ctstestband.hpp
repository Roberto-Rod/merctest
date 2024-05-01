#ifndef CTSTESTBAND_HPP
#define CTSTESTBAND_HPP

#include "ctsboard.hpp"

#include <QString>
#include <QMap>

namespace testsuite
{
    enum class CtsTestBandType
    {
        Stop,
        Transition,
        Pass
    };

    /** @brief Defines a band which may configured as a pass-band, a stop-band or a
     * transition band by setting min/max power appropriately.
     * Power is tested as millivolts read from ADC as the paths are operated outside
     * normal operating limits which puts the conversion from mV to dBm out of calibration.
     * Also, as measurements are made outside normal operating bands, the LO offset is
     * defined per test band, this overrides the LO offset normally used for the path.
     */
    struct CtsTestBand
    {
        uint8_t m_rfPath;               //!< RF path, 0 to 5
        dut::CtsBoard::IfPath m_ifPath; //!< IF path
        uint32_t m_rfStart_kHz;         //!< First frequency to test
        uint32_t m_rfEnd_kHz;           //!< Last frequency to test
        uint32_t m_rfStep_kHz;          //!< Size of RF steps
        int32_t  m_loOffset_kHz;        //!< LO offset to use for this band
        bool m_invertRf;                //!< true for LO on low-side of IF, false for LO on high-side of IF
        bool m_fixedLo;                 //!< If true then RF will not be added/subtracted from LO offset and m_invertRf becomes "don't care"
        uint16_t m_minPowerReading_mV;  //!< Minimum power reading for this band
        uint16_t m_maxPowerReading_mV;  //!< Maximum power reading for this band
        float m_testPower_dBm;          //!< RF input power for test
        CtsTestBandType m_type;         //!< Enumerated test band type
        QString typeString();           //!< Test band type as a string

    private:
        static const QMap<CtsTestBandType, QString> kTestBandTypeMap;
    };
}

#endif // CTSTESTBAND_HPP

