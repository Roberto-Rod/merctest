#ifndef POWERMONITORADC_HPP
#define POWERMONITORADC_HPP

#include "stdint.h"

namespace hardware
{
    class powerMonitorAdc
    {
    public:
       powerMonitorAdc();
       enum PowerMonitorStanbyCurrent { RAIL5VCURRENT = 5,  RAIL3V3CURRENT = 25  };
       enum  VoltageRail{  RAIL5V, RAIL3V3 };
       double getKMillivoltsPerLsb();
       bool setVoltageRail(double current, bool isHighband);
       uint16_t getMinStandbyCurrent(bool isHiighBand);
       uint16_t getMaxStandbyCurrent(bool isHighBand);
       bool is5Vref();

    private:
        VoltageRail m_rail;
    };

}

#endif // POWERMONITORADC_HPP
