#include "testjig.hpp"
#include "directserialcomms.hpp"
#include "logging.hpp"

namespace testjig
{
    TestJig::TestJig(QObject* parent) :
        QObject(parent),
        m_pRegComms(0)
    {
    }

    QList<dut::DeviceUnderTest*>* TestJig::duts()
    {
        return &m_duts;
    }

    comms::RegisterComms* TestJig::registerComms()
    {
        return m_pRegComms;
    }
}
