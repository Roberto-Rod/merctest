#include "rfpath.hpp"

namespace testjig
{
    /*RfPath::RfPath()
    {
    }  */

    RfPathSourceAssy::RfPathSourceAssy(DutRfCable dutCable, DutRfPort dutPort, TePort tePort) :
        m_dutCable (dutCable),
        m_dutPort(dutPort),
        m_tePort(tePort)
    {
    }

    QString RfPathSourceAssy::calInstruction(RfPath *pPrevious) const
    {
        QString str;

        RfPathSourceAssy* pPreviousSrc = dynamic_cast<RfPathSourceAssy*>(pPrevious);

        // If there is no previous path then create initial message
        if (pPreviousSrc == 0)
        {
            str = "<B>Please connect the following:</B>"
                  "<P>1. Signal Generator to ";

            if (m_dutCable == RfPathSourceAssy::DutRfCable::UNKNOWN) { str += "Power Meter using RF test cable (Signal Generator cable)."; }
            else
            {
                str += dutPortString() +
                       " port on Test Jig via RF test cable (Signal Generator cable), SMA-SMP adapter and " + dutCableString() + "." +
                       "<P>2. Power Meter to " + tePortString() + " port on Test Jig via RF test cable (" + tePortString() + " cable).";
            }
        }
        // Otherwise, create a swapover message
        else
        {
            int step = 1;
            str = "<B>Please move the following:</B>";

            if (pPreviousSrc->dutPort() != m_dutPort)
            {
                str += QString("<P>%1. Signal Generator cable + SMA-SMP adapter from CAB0255 attached to ").arg(step++) +
                       pPreviousSrc->dutPortString() +
                       " port to CAB0255 attached to " + dutPortString() + " port on Test Jig.";
            }

            if (pPreviousSrc->tePort() != m_tePort)
            {
                str += QString("<P>%1. Detach ").arg(step++) + pPreviousSrc->tePortString() + " RF cable from Power Meter.";
                str += QString("<P>%1. Attach ").arg(step++) + tePortString()               + " RF cable to Power Meter.";
            }
        }

        return str;
    }

    QString RfPathSourceAssy::brief() const
    {
        QString retVal;

        if (m_dutCable == RfPathSourceAssy::DutRfCable::UNKNOWN)
        {
            retVal = "Calibration Null Path";
        }
        else
        {
            if (m_dutPort == RfPathSourceAssy::DutRfPort::LHS)     { retVal += "LHS DUT to "; }
            else                                                     { retVal += "RHS DUT to "; }

            if (m_tePort == RfPathSourceAssy::TePort::POWER_METER) { retVal += "Power Meter port"; }
            else                                                     { retVal += "Spectrum Analyser port"; }
        }

        return retVal;
    }

    QString RfPathSourceAssy::key() const
    {
        QString retVal;

        if ((m_dutPort  != RfPathSourceAssy::DutRfPort::UNKNOWN) &&
             (m_dutCable != RfPathSourceAssy::DutRfCable::UNKNOWN) &&
             (m_tePort   != RfPathSourceAssy::TePort::UNKNOWN))
        {
            retVal = "rfpathsourceassy/";

            if (m_dutPort == RfPathSourceAssy::DutRfPort::LHS)     { retVal += "lhs_"; }
            else                                                     { retVal += "rhs_"; }

            if (m_dutCable == RfPathSourceAssy::DutRfCable::SHORT) { retVal += "short_"; }
            else                                                     { retVal += "long_"; }

            if (m_tePort == RfPathSourceAssy::TePort::POWER_METER) { retVal += "pwr_mtr"; }
            else                                                     { retVal += "spec_an"; }
        }

        return retVal;
    }

    double RfPathSourceAssy::testPower() const
    {
        return kTestPowerDefault;
    }

    RfPathSourceAssy::DutRfCable RfPathSourceAssy::dutCable() const
    {
        return m_dutCable;
    }

    QString RfPathSourceAssy::dutCableString() const
    {
        QString str;

        if (m_dutCable == RfPathSourceAssy::DutRfCable::SHORT) { str = "CAB0254"; }
        else                                                     { str = "CAB0255"; }

        return str;
    }

    void RfPathSourceAssy::setDutCable(const DutRfCable &dutCable)
    {
        m_dutCable = dutCable;
    }

    RfPathSourceAssy::DutRfPort RfPathSourceAssy::dutPort() const
    {
        return m_dutPort;
    }

    QString RfPathSourceAssy::dutPortString() const
    {
        QString str;

        if (m_dutPort == RfPathSourceAssy::DutRfPort::LHS) { str = "\"DUT RF LHS\""; }
        else                                                 { str = "\"DUT RF RHS\""; }

        return str;
    }

    void RfPathSourceAssy::setDutPort(const DutRfPort &dutPort)
    {
        m_dutPort = dutPort;
    }

    RfPathSourceAssy::TePort RfPathSourceAssy::tePort() const
    {
        return m_tePort;
    }

    QString RfPathSourceAssy::tePortString() const
    {
        QString str;

        if (m_tePort == RfPathSourceAssy::TePort::POWER_METER) { str = "Power Meter"; }
        else                                                     { str = "Spectrum Analyser"; }

        return str;
    }

    void RfPathSourceAssy::setTePort(const TePort &tePort)
    {
        m_tePort = tePort;
    }

    //****************************************************************************
    // High Band Module
    //****************************************************************************

    RfPathHighBandModule::RfPathHighBandModule(RfPathHighBandModule::RfCableSet cableSet)
        : m_cableSet(cableSet)
    {
        m_testPowerMap[RfCableSet::UNKNOWN]                 = kTestPowerDefault;
        m_testPowerMap[RfCableSet::POWER_METER_AND_CAB0255] = kTestPowerDefault;
        m_testPowerMap[RfCableSet::SPEC_AN_AND_CAB0255]     = kTestPowerDefault;
    }

    QString  RfPathHighBandModule::calInstruction(RfPath* pPrevious) const
    {
        (void)pPrevious;

        QString retVal = "<B>Please connect the following:</B>"
                         "<P>1. Signal Generator to ";

        if (m_cableSet == RfCableSet::POWER_METER_AND_CAB0255) { retVal += "Power Meter via CAB0255 + SMA(Jack)-SMP(Plug) adapter + Power Meter cable "; }
        else if (m_cableSet == RfCableSet::SIG_GEN_AND_CAB0255) { retVal += "Power Meter via CAB0255 + SMA(Jack)-SMP(Plug) adapter + Sig Gen cable + N(Jack)-N(Jack) adapter"; }
        else if (m_cableSet == RfCableSet::SPEC_AN_AND_CAB0255) { retVal += "Power Meter via CAB0255 + SMA(Jack)-SMP(Plug) adapter + Spec An. cable + N(Jack)-N(Jack) adapter"; }
        return retVal;
    }

    QString  RfPathHighBandModule::brief() const
    {
        QString retVal;

        if (m_cableSet == RfCableSet::UNKNOWN)                      { retVal = "Calibration Null Path"; }
        else if (m_cableSet == RfCableSet::POWER_METER_AND_CAB0255) { retVal = "Power meter cable and CAB0255"; }
        else if (m_cableSet == RfCableSet::SPEC_AN_AND_CAB0255)     { retVal = "Spec An. cable and CAB0255"; }

        return retVal;
    }

    QString  RfPathHighBandModule::key() const
    {
        QString retVal;

        if (m_cableSet != RfCableSet::UNKNOWN)
        {
            retVal = "rfpathhbmodule/";

            if (m_cableSet == RfCableSet::POWER_METER_AND_CAB0255)   { retVal += "power_meter_and_CAB0255"; }
            else if (m_cableSet == RfCableSet::SPEC_AN_AND_CAB0255)   { retVal += "spec_an_and_CAB0255"; }
        }

        return retVal;
    }

    double  RfPathHighBandModule::testPower() const
    {
        return m_testPowerMap[m_cableSet];
    }

    RfPathHighBandModule::RfCableSet  RfPathHighBandModule::cableSet() const
    {
        return m_cableSet;
    }

     //****************************************************************************
     // Power Monitor
     //****************************************************************************

    RfPathPowerMonitor::RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet cableSet)
        : m_cableSet(cableSet)
    {
        m_testPowerMap[RfCableSet::UNKNOWN]                 = kTestPowerDefault;
        m_testPowerMap[RfCableSet::SIG_GEN_AND_POWER_METER] = kTestPowerDefault;
        m_testPowerMap[RfCableSet::SIG_GEN_AT_15_DBM]       = kTestPower15dbm;
    }

    QString RfPathPowerMonitor::calInstruction(RfPath* pPrevious) const
    {
        (void)pPrevious;

        QString retVal = "<B>Please connect the following:</B>"
                         "<P>1. Signal Generator to ";

        if (m_cableSet == RfCableSet::UNKNOWN)                      { retVal += "Power Meter (direct, no RF test cable)."; }
        else if (m_cableSet == RfCableSet::SIG_GEN_AT_15_DBM)       { retVal += "Power Meter via single RF test cable (Signal Generator cable)."; }
        else if (m_cableSet == RfCableSet::SIG_GEN_AND_POWER_METER) { retVal += "Power Meter via two RF test cables  (Signal Generator cable + Power Meter cable)."; }

        return retVal;
    }

    QString RfPathPowerMonitor::brief() const
    {
        QString retVal;

        if (m_cableSet == RfCableSet::UNKNOWN)                      { retVal = "Calibration Null Path"; }
        else if (m_cableSet == RfCableSet::SIG_GEN_AT_15_DBM)       { retVal = "+15dBm cal. (single RF test cable)"; }
        else if (m_cableSet == RfCableSet::SIG_GEN_AND_POWER_METER) { retVal = "Through cal. (pair of RF test cables)"; }

        return retVal;
    }

    QString RfPathPowerMonitor::key() const
    {
        QString retVal;

        if (m_cableSet != RfCableSet::UNKNOWN)
        {
            retVal = "rfpathpowermonitor/";

            if (m_cableSet == RfCableSet::SIG_GEN_AND_POWER_METER) { retVal += "sig_gen_and_pwr_mtr"; }
            else if (m_cableSet == RfCableSet::SIG_GEN_AT_15_DBM)  { retVal += "sig_gen_at_15_dbm"; }
        }
        return retVal;
    }

    double RfPathPowerMonitor::testPower() const
    {
        return m_testPowerMap[m_cableSet];
    }

    RfPathPowerMonitor::RfCableSet RfPathPowerMonitor::cableSet() const
    {
        return m_cableSet;
    }
}
