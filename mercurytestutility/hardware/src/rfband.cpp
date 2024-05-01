#include "rfband.hpp"

namespace hardware
{
    QString RfBand::description(Band band)
    {
        QString desc;

        if (band == LOW)      { desc = "Low (20MHz-520MHz)"; }
        else if (band == MID) { desc = "Mid (500MHz-2700MHz)"; }
        else if (band == HIGH) { desc = "High (2400MHz-6000MHz)"; }

        return desc;
    }
}
