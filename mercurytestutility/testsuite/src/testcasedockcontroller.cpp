#include "testcasedockcontroller.hpp"
#include "testcasedockcontrollerbit.hpp"
#include "testcasedockcontrollercontrolport.hpp"
#include "testcasedockcontrollerdcpower.hpp"
#include "testcasedockcontrollereeprom.hpp"
#include "testcasedockcontrollergps.hpp"
#include "testcasedockcontrollerinterfaceid.hpp"
#include "testcasedockcontrollerpactrlmonitor.hpp"
#include "testcasedockcontrollerprogfpga.hpp"

namespace testsuite
{
    TestCaseDockController::TestCaseDockController(QObject* parent) :
        TestCase(parent)
    {
        // Make test case objects with this as their parent
        new TestCaseDockControllerProgFpga(this);
        new TestCaseDockControllerDcPower(this);
        new TestCaseDockControllerControlPort(hardware::Port::A, this);
        new TestCaseDockControllerControlPort(hardware::Port::B, this);
        new TestCaseDockControllerPaCtrlMonitor(hardware::Port::A, this);
        new TestCaseDockControllerPaCtrlMonitor(hardware::Port::B, this);
        new TestCaseDockControllerInterfaceId(this);
        new TestCaseDockControllerBit(this);
        new TestCaseDockControllerEeprom(this);
        new TestCaseDockControllerGps(this);
    }

    TestCaseDockController::TestCaseDockController(QString name, QObject* parent) :
        TestCase(name, parent)
    {
    }

    QString TestCaseDockController::setupMessage()
    {
        // Configure the initial test setup message
        return QString("<B>Initial Test Setup:</B>"
                        "<P>1. Connect Test Jig to DUT J10 (DC IN) using CAB0249."
                        "<P>2. Connect Test Jig to DUT J2 (CHANNEL A) using CAB0258."
                        "<P>4. Connect Test Jig to DUT J3 (CHANNEL A) using CAB0252."
                        "<P>3. Connect Test Jig to DUT J6 (CHANNEL B) using CAB0258."
                        "<P>5. Connect Test Jig to DUT J7 (CHANNEL B) using CAB0252."
                        "<P>6. Connect Active GPS Antenna to DUT J9 (GPS ANT.)."
                        "<P><I>Note that DUT J1, J4, J8 are not connected and not tested.</I>");
    }
}
