#include "testcasepsumodule.hpp"
#include "testcasepsumodulebit.hpp"
#include "testcasepsumodulecontrolport.hpp"
#include "testcasepsumoduledcinput.hpp"
#include "testcasepsumoduledcoutput.hpp"
#include "testcasepsumodulefans.hpp"
#include "testcasepsumodulepacontrol.hpp"
#include "testcasepsumoduleprogmicro.hpp"
#include "testcasepsumodulethermalsensors.hpp"
#include "psumodule.hpp"

namespace testsuite
{
    TestCasePsuModule::TestCasePsuModule(QObject* parent) :
        TestCase(parent)
    {
        // Make test case objects with this as their parent
        new TestCasePsuModuleProgMicro(this);
        new TestCasePsuModuleDcInput(this);
        new TestCasePsuModuleControlPort(this);
        new TestCasePsuModuleDcOutput(this);
        new TestCasePsuModuleBit(this);
        new TestCasePsuModuleFans(this);
        new TestCasePsuModuleThermalSensors(this);
        new TestCasePsuModulePaControl(this);
    }

    TestCasePsuModule::TestCasePsuModule(QString name, QObject* parent) :
        TestCase(name, parent)
    {
    }

    QString TestCasePsuModule::setupMessage()
    {
        QString msg;

        // Configure the initial test setup message which is dependent on DUT type as some of the reference designators differ
        const dut::PsuModuleVariant* variant = dynamic_cast<const dut::PsuModuleVariant*>(dut()->selectedVariant());

        if (variant != 0)
        {
            // Power in/out ref des are type dependent
            QString powerIn, powerOut;

            if (variant->m_type == dut::PsuModuleType::MANPACK)
            {
                powerIn  = "J9";
                powerOut = "J10";
            }
            else if (variant->m_type == dut::PsuModuleType::VEHICLE)
            {
                powerIn  = "J2";
                powerOut = "J3";
            }

            // Other ref des are common
            msg = QString("<B>Initial Test Setup:</B>"
                           "<P>1. Connect Test Jig \"DUT POWER IN\" sockets to DUT %1 Using CAB0259 + CAB0260."
                           "<P>2. Connect Test Jig \"DUT POWER OUT\" sockets to DUT %2 Using CAB0259 + CAB0260."
                           "<P>3. Connect Test Jig to DUT J1 Using CAB0158."
                           "<P>4. Connect Test Jig to DUT J4 Using CAB0262."
                           "<P>5. Connect Test Jig to DUT J7 Using CAB0261."
                           "<P>6. Connect Test Jig to DUT J6 Using CAB0261.")
                           .arg(powerIn)
                           .arg(powerOut);
        }

        return msg;
    }
}
