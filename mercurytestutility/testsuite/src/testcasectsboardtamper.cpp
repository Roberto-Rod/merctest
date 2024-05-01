#include "testcasectsboardtamper.hpp"
#include "ctsboard.hpp"
#include "testharnessctsboard.hpp"
#include "logging.hpp"
#include "actiondialog.hpp"
#include "delayprogressdialog.hpp"

#include <QThread>

namespace testsuite
{
    TestCaseCtsBoardTamper::TestCaseCtsBoardTamper(QObject* parent) :
        TestCaseCtsBoard(QString("Anti-Tamper"), parent)
    {
    }

    QString TestCaseCtsBoardTamper::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardTamper::execute(bool manualSteps)
    {
        // testsPassed will get set to false if any test fails its limits
        // retVal will get set to false if any function calls fail - this doesn't mean a test has
        // failed its limits, this means the hardware (DUT or jig) is failing to respond.
        bool testsPassed(true);

        // Call the base class method to write the test name
        // to the log file and reset test status
        bool retVal(TestCase::execute());

        if (retVal)
        {
            // Indent log (automatically de-indented when object is destroyed)
            common::LogIndent indent;

            // Initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "initialise DUT");

            m_channelUnderTest = dut::CtsBoard::TamperChannel::Microswitch;

            // Use an action dialog to get the user to hold-down microswitch
            if (KEEP_GOING)
            {
                QString title("Microswitch");
                QString msg("Please hold-down the microswitch (SW1) lever.");
                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),    this,  SLOT(getTestStateUntampered()));
                connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for microswitch lever to be held down");

                // Get the test state
                TEST_BOOL("Microswitch Depressed", dialog.status(), "");
            }

            // Use an action dialog to get the user to release the microswitch
            if (KEEP_GOING)
            {
                QString title("Microswitch");
                QString msg("Please release the microswitch (SW1) lever.");

                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),    this,  SLOT(getTestStateTampered()));
                connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for microswitch lever to be released");

                // Get the test state
                TEST_BOOL("Microswitch Released", dialog.status(), "");
            }

            m_channelUnderTest = dut::CtsBoard::TamperChannel::LightSensor;

            // Use an action dialog to get the user to cover the ALS
            if (KEEP_GOING)
            {
                QString title("Ambient Light Sensor");
                QString msg("Please fully cover Ambient Light Sensor (TR6) e.g. using blu-tac.");

                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),    this,  SLOT(getTestStateUntampered()));
                connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for ambient light sensor to be covered");

                // Get the test state
                TEST_BOOL("Ambient Light Sensor Unilluminated", dialog.status(), "");
            }

            // Use an action dialog to get the user to uncover the ALS
            if (KEEP_GOING)
            {
                QString title = "Ambient Light Sensor";
                QString msg   = "Please uncover Ambient Light Sensor (TR6) and expose to normal room lighting.";

                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),    this,  SLOT(getTestStateTampered()));
                connect(this,    SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for ambient light sensor to be uncovered");

                // Get the test state
                TEST_BOOL("Ambient Light Sensor Illuminated", dialog.status(), "");
            }

            // Clean up - attempt these regardless of test state
            if (m_pCtsBrd != 0)
            {
                // Disconnect DUT serial port
                m_pCtsBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    void TestCaseCtsBoardTamper::getTestStateUntampered()
    {
        // Test ambient light sensor is unilluminated
        bool tampered;
        bool ok(m_pCtsBrd->resetTamperDevice());
        QThread::msleep(500);
        ok = ok && m_pCtsBrd->getTamperStatus(m_channelUnderTest, tampered);
        emit testState(ok && !tampered);
    }

    void TestCaseCtsBoardTamper::getTestStateTampered()
    {
        // Test ambient light sensor is illuminated
        bool tampered;
        bool ok(m_pCtsBrd->resetTamperDevice());
        QThread::msleep(500);
        ok = ok && m_pCtsBrd->getTamperStatus(m_channelUnderTest, tampered);
        emit testState(ok && tampered);
    }
}
