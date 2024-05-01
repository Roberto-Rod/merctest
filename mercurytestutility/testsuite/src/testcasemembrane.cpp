#include "testcasemembrane.hpp"
#include "testcasemembranebuttons.hpp"
#include "testcasemembraneleds.hpp"

namespace testsuite
{
    TestCaseMembrane::TestCaseMembrane(QObject* parent) :
        TestCase(parent)
    {
        new TestCaseMembraneLeds(this);
        new TestCaseMembraneButtons(this);
    }

    TestCaseMembrane::~TestCaseMembrane()
    {
    }

    QString TestCaseMembrane::setupMessage()
    {
        return QString("<B>Initial Test Setup:</B>"
                        "<P>1. Connect Membrane Keypad to Test Jig Membrane Keypad port using CAB0092.");
    }
}
