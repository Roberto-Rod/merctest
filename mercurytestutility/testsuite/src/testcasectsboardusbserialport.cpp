#include "testcasectsboardusbserialport.hpp"
#include "ctsboard.hpp"
#include "logging.hpp"
#include "directserialcomms.hpp"
#include "actiondialog.hpp"

#include <QSerialPortInfo>

namespace testsuite
{
    // CTS ping and expected response
    const QByteArray TestCaseCtsBoardUsbSerialPort::kSendBytes = QByteArrayLiteral("\xC0\x02\x04\x00\x00\x1C\x13");
    const QByteArray TestCaseCtsBoardUsbSerialPort::kExpectBytes = QByteArrayLiteral("\xC0\x04\x02\xFE\xFF\x00\x00\x8D\x37");

    TestCaseCtsBoardUsbSerialPort::TestCaseCtsBoardUsbSerialPort(QObject* parent) :
        TestCaseCtsBoard("USB Serial Port", parent)
    {
    }

    QString TestCaseCtsBoardUsbSerialPort::setupMessage()
    {
        return QString();
    }

    bool TestCaseCtsBoardUsbSerialPort::execute(bool manualSteps)
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

            // Kill DUT DC power (battery now installed so need to kill power to fully restart)
            TEST_STEP(m_pCtsBrd->killDcPower(), "kill DUT DC power");

            // Re-initialise CTS Board
            TEST_STEP(initialiseCtsBrd(), "re-initialise DUT");

            // Get a list of currently enumerated serial ports
            getPortNames(m_portNamesStart);

            // Instruct user to plug DUT USB port into PC and wait for a new serial port to appear,
            // this should be the DUT USB port serial port
            if (KEEP_GOING)
            {
                QString title("Connect USB Port to PC");
                QString msg("1. Disconnect CAB0362 from the DUT.\r\n"
                            "2. Connect the DUT to a USB port on the PC using a standard USB Type-A to Mini-B cable.");
                gui::ActionDialog dialog(title, msg);

                connect(&dialog, SIGNAL(getTestState()),     this, SLOT(findUsbPortName()));
                connect(   this, SIGNAL(testState(bool)), &dialog, SLOT(testState(bool)));

                // The dialog exec return value tells us whether to continue (true) or if test was cancelled (false)
                TEST_STEP(dialog.exec(), "wait for USB serial port to be detected by PC");

                // Get the test state
                TEST_BOOL("New Serial Device Connected to PC", dialog.status(), "");
            }

            // Open the serial port
            comms::DirectSerialComms serial(kUsbCommsTimeout_ms, this);

            bool ok(false);
            TEST_BOOL(QString("Open USB Serial Port (%1)").arg(m_usbPortName), (ok = serial.openPort(m_usbPortName)), "");

            if (ok)
            {
                QByteArray readBytes;

                // Send a test packet (ping) and expect a fixed response
                TEST_BOOL("Send Test Packet", serial.write(kSendBytes), "");
                TEST_BOOL("Receive Response", serial.read(readBytes, kExpectBytes.length()), "");
                TEST_BOOL("Response Matches Expected", readBytes == kExpectBytes, "");
            }

            // Clean up - attempt regardless of test state
            if (m_pCtsBrd)
            {
                // Disconnect DUT serial port
                m_pCtsBrd->disconnect();
            }
        }

        // Set and log the status
        setStatus(retVal, testsPassed);

        return retVal;
    }

    void TestCaseCtsBoardUsbSerialPort::getPortNames(QStringList& portNames) const
    {
        portNames.clear();

        // Get a list of currently enumerated serial ports
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

        QSerialPortInfo port;
        foreach (port, ports)
        {
            portNames << port.portName();
        }

        portNames.sort();
    }

    void TestCaseCtsBoardUsbSerialPort::findUsbPortName()
    {
        QStringList portNames;

        getPortNames(portNames);
        m_usbPortName.clear();

        // If there are now fewer ports than at the start of the test then update the start list
        // this means that if the USB port was already plugged in then unplugging it and plugging
        // it back in should allow it to be detected
        if (portNames.length() < m_portNamesStart.length())
        {
            m_portNamesStart = portNames;
        }
        else
        {
            // Look for the first port name in the sorted list which was not in the original list
            for (int i = 0; i < portNames.length(); i++)
            {
                if ((i == m_portNamesStart.length()) || (portNames.at(i) != m_portNamesStart.at(i)))
                {
                    m_usbPortName = portNames.at(i);
                    break;
                }
            }
        }

        emit testState(!m_usbPortName.isEmpty());
    }
}

