#include "testcasepowermonitor.hpp"
#include "testcasepowermonitordcpower.hpp"
#include "testcasepowermonitordirectivity.hpp"
#include "testcasepowermonitorinsertionloss.hpp"
#include "testcasepowermonitornorf.hpp"
#include "messagedialog.hpp"
#include "powermonitor.hpp"

namespace testsuite
{
    TestCasePowerMonitor::TestCasePowerMonitor(QObject* parent) :
        TestCase(parent)
    {
        // Make test case objects with this as their parent
        new TestCasePowerMonitorInsertionLoss(this);
        new TestCasePowerMonitorDcPower(this);
        new TestCasePowerMonitorNoRf(this);
        new TestCasePowerMonitorDirectivity(this);
    }

    TestCasePowerMonitor::TestCasePowerMonitor(QString name, QObject* parent) :
        TestCase(name, parent)
    {
    }

    QString TestCasePowerMonitor::setupMessage()
    {
        return QString("<B>Initial Test Setup:</B>"
                        "<P>1. Connect Test Jig PSU/Power Monitor Port to DUT J3 Using CAB0158."
                        "<P>2. Connect Signal Generator to DUT \"IN\" port using RF Test Cable."
                        "<P>3. Connect Power Meter to DUT \"OUT\" port using RF Test Cable.");
    }
}
