#include "serialcomms.hpp"

namespace comms
{
    SerialComms::SerialComms(QObject* parent) :
        QObject(parent),
        m_readTimeout(kDefaultReadTimeout)
    {
    }

    SerialComms::SerialComms(int32_t readTimeout, QObject* parent) :
        QObject(parent),
        m_readTimeout(readTimeout)
    {
    }

    bool SerialComms::setReadTimeout(int32_t milliseconds)
    {
        bool retVal(false);

        if (milliseconds > 0)
        {
            m_readTimeout = milliseconds;
        }

        return retVal;
    }
}
