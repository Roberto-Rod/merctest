#include "membrane.hpp"
#include "testharnessmembrane.hpp"
#include "testcasemembrane.hpp"
#include "settings.hpp"
#include "testjig.hpp"
#include "registercomms.hpp"
#include "assyrevision.hpp"

namespace dut
{
    Membrane::Membrane(testjig::TestJig* parentJig) :
        DeviceUnderTest(parentJig)
    {
        m_variants.append(new Variant("Membrane Keypad", "KEY000[2-6]"));  // The generic variant
        m_variants.append(new MembraneVariant("Blade Membrane Keypad",   "KEY0002", true));
        m_variants.append(new MembraneVariant("RCU Membrane Keypad",     "KEY0003", true));
        m_variants.append(new MembraneVariant("Ventura Membrane Keypad", "KEY0004", true));
        m_variants.append(new MembraneVariant("Sky Net Recurve High-Band Membrane Keypad", "KEY0005", true));
        m_variants.append(new MembraneVariant("CTS Membrane Keypad", "KEY0006", true));
        m_pRevision = new common::AssyRevision("Null Assembly","");

        m_pTopLevelTestCase = new testsuite::TestCaseMembrane(this);
        m_pTestHarness = new testjig::TestHarnessMembrane(parentJig->registerComms(), this);
    }

    Membrane::~Membrane()
    {
    }

    bool Membrane::connect()
    {
        return true;
    }

    void Membrane::disconnect()
    {
    }

    bool Membrane::isConnected() const
    {
        return true;
    }

    DutState Membrane::state() const
    {
        // Only two possible states for this DUT - either No Comms (with jig)
        // or Ready
        DutState retVal = DutState::NO_COMMS;

        if (isConnected())
        {
            retVal = DutState::READY;
        }

        return retVal;
    }

    bool Membrane::waitReady() const
    {
        // No waiting to do - it's ready or not (here I come)
        return (state() == DutState::READY);
    }

    bool Membrane::syncLedPresent() const
    {
        bool retVal = false;

        const MembraneVariant* variant = dynamic_cast<const MembraneVariant*>(selectedVariant());

        if (variant != 0)
        {
            retVal = variant->m_syncLedPresent;
        }

        return retVal;
    }
}
