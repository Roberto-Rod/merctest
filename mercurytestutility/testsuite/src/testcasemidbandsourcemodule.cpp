#include "testcasemidbandsourcemodule.hpp"
#include "testcasemidbandsourcemoduleassy.hpp"
#include "testcasemidbandsourcemoduledcpower.hpp"
#include "testcasemidbandsourcemoduleatt.hpp"
#include "testcasemidbandsourcemoduleid.hpp"
#include "testcasemidbandsourcemoduleportsel.hpp"
#include "testcasemidbandsourcemodulerfpath.hpp"
#include "sourceboard.hpp"
#include "testharnesssourceboard.hpp"
#include "messagedialog.hpp"
#include "daughtermodule.hpp"

namespace testsuite
{
    TestCaseMidBandSourceModule::TestCaseMidBandSourceModule(QObject* parent) :
        TestCase(parent)
    {
        // Create the child test cases
        new TestCaseMidBandSourceModuleDcPower(this);
        new TestCaseMidBandSourceModuleId(this);
        new TestCaseMidBandSourceModulePortSel(this);
        new TestCaseMidBandSourceModuleRfPath(this);
        new TestCaseMidBandSourceModuleAtt(this);
        new TestCaseMidBandSourceModuleAssy(this);
    }

    QString TestCaseMidBandSourceModule::setupMessage()
    {
       QString   asyType = this->variant();

       // High Band Module
       if(asyType  == "ASY0129")
       {
           return QString("<B>Initial Test Setup:</B>"
                           "<P>1. Connect Test Jig to DUT \"6\" using CAB0251."
                           "<P>2. Connect Test Jig to DUT \"9\" using CAB0249."
                           "<P>3. Spectrum Analyser  to  (RF O/P 2) using CAB0255 and the Spectrum Analyser Cable."
                           "<P>4. Power Meter to (RF O/P 1) using CAB0255 and the Power Meter Cable."
                           "<P>6. Connect Signal Generator to DUT J2 (RF I/P) using RF Test Cable + SMA-SMP Adapter.");
       }

       // Mid Band module
        return QString("<B>Initial Test Setup:</B>"
                        "<P>1. Connect Test Jig to DUT \"6\" using CAB0251."
                        "<P>2. Connect Test Jig to DUT \"9\" using CAB0249."
                        "<P>3. Connect Test Jig  DUT RF LHS to (RF O/P 2) using CAB0255."
                        "<P>4. Connect Test Jig  DUT RF RHS to  (RF O/P 1) using CAB0255."
                        "<P>6. Connect Signal Generator to DUT J2 (RF I/P) using RF Test Cable + SMA-SMP Adapter.");
    }

    bool TestCaseMidBandSourceModule::execute(bool manualSteps)
    {
        // Execute all of the test cases
        bool retVal = TestCase::execute(manualSteps);

        // Get a pointer to the DUT
        dut::SourceBoard* pSrcBrd = dynamic_cast<dut::SourceBoard*>(dut());

        // Get a pointer to the test harness
        testjig::TestHarnessSourceBoard* pHarness = static_cast<testjig::TestHarnessSourceBoard*>(pSrcBrd->testHarness());

        // Disable power to DUT
        pHarness->enablePower(false);

        return retVal;
    }
}
