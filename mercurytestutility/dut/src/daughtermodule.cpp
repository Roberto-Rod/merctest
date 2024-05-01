#include "daughtermodule.hpp"
#include "testcasemidbandsourcemodule.hpp"
#include "assyrevision.hpp"

namespace dut
{
    DaughterModule::DaughterModule(testjig::TestJig* parentJig) :
        SourceBoard(parentJig)
    {
        m_variants.clear();
        m_variants.append(new Variant("Daughter Board", "ASY0033/ASY0129"));
        m_variants.append(new DaughterBoardVariant("Mid-Band Son/Daughter Board", "ASY0033", hardware::RfBand::MID));
        m_variants.append(new DaughterBoardVariant("High-Band Son/Daughter Board", "ASY0129", hardware::RfBand::HIGH));
        m_pTopLevelTestCase = new testsuite::TestCaseMidBandSourceModule(this);
        m_pRevision = new common::AssyRevision("Null Assembly","");
    }

    uint16_t  DaughterModule::getRfPathDivisor(uint8_t path)
    {
         uint16_t path_divisor = 1;

          const  DaughterBoardVariant* variant = dynamic_cast<const  DaughterBoardVariant*>(selectedVariant());
          hardware::RfBand::Band band = hardware::RfBand::MID;

          if (variant != 0)
          {
              band = variant->m_band;
          }

          if(band == hardware::RfBand::MID)
          {
              if(path == 0u)
              {
                  path_divisor = 1;
              }
              else
              {
                  path_divisor = 2;
              }
          }
          else if (band == hardware::RfBand::HIGH)
          {
              if(path == 0u)
              {
                  path_divisor = 2;
              }
              else if (path == 1 || path == 2)
              {
                  path_divisor = 4;
              }
              else if(path == 3)
              {
                  path_divisor = 6;
              }
          }

          return path_divisor;
    }

    hardware::RfBand::Band  DaughterModule::band()
    {
        // Default to low band
        hardware::RfBand::Band band = hardware::RfBand::MID;

        const  DaughterBoardVariant* variant = dynamic_cast<const  DaughterBoardVariant*>(selectedVariant());

        if (variant != 0)
        {
            band = variant->m_band;
        }

        return band;
    }

     bool DaughterModule::isMidBand()
     {
         bool result = false;
         if (this->band() == hardware::RfBand::MID)
         {
             result = true;
         }
       return result;
     }

     bool DaughterModule::isHighBand()
     {
         bool result = false;
         if (this->band() == hardware::RfBand::HIGH)
         {
             result = true;
         }
       return result;
     }

}
