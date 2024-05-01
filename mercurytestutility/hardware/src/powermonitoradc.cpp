#include "powermonitoradc.hpp"

namespace hardware {
       powerMonitorAdc::powerMonitorAdc()
           :  m_rail (hardware::powerMonitorAdc::RAIL3V3)
       {
       }

       // Establish variant of board using standby current and band of DUT.

       bool powerMonitorAdc::setVoltageRail(double current, bool isHighBand)
       {
           bool result = false;
           if (isHighBand)
           {
                m_rail = hardware::powerMonitorAdc::RAIL3V3;
                result = true;
           }
           else if (current > hardware::powerMonitorAdc::RAIL3V3CURRENT - 5 && current < hardware::powerMonitorAdc::RAIL3V3CURRENT + 5)
           {
               m_rail = hardware::powerMonitorAdc::RAIL3V3;
               result = true;
           }
           else  if (current > hardware::powerMonitorAdc::RAIL5VCURRENT - 5 && current < hardware::powerMonitorAdc::RAIL5VCURRENT + 5)
           {
               m_rail = hardware::powerMonitorAdc::RAIL5V;
               result = true;
           }

           return result;
       }

       double powerMonitorAdc::getKMillivoltsPerLsb()
        {
           double millivoltsPerLsb;
           if (m_rail == hardware::powerMonitorAdc::RAIL3V3)
           {
              millivoltsPerLsb = (3300.0 / 4096.0);
           }
           else
           {
             millivoltsPerLsb = (5000.0 / 4096.0);
           }
           return millivoltsPerLsb;
        }

        bool powerMonitorAdc::is5Vref()
        {
            bool result = false;
            if   (m_rail == hardware::powerMonitorAdc::RAIL5V)
            {
                result = true;
            }
            return result;
        }

        uint16_t powerMonitorAdc::getMinStandbyCurrent(bool isHighBand)
        {
            uint16_t result = hardware::powerMonitorAdc::RAIL5VCURRENT - 5;
            if(isHighBand)
            {
                return result;
            }
            else if (m_rail == hardware::powerMonitorAdc::RAIL3V3)
            {
                result = hardware::powerMonitorAdc::RAIL3V3CURRENT - 5;
            }

            return result;
       }

       uint16_t powerMonitorAdc::getMaxStandbyCurrent(bool isHighBand)
       {
           uint16_t result = hardware::powerMonitorAdc::RAIL5VCURRENT + 5;
           if(isHighBand)
           {
                return  result;
           }
           else if (m_rail == hardware::powerMonitorAdc::RAIL3V3)
           {
               result = hardware::powerMonitorAdc::RAIL3V3CURRENT + 5;
           }
           return result;
       }
};
