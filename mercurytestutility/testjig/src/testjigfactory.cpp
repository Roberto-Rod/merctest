#include "testjigfactory.hpp"
#include "registercomms.hpp"
#include "directserialcomms.hpp"
#include "testjigdetails.hpp"
#include "settings.hpp"
#include "mercurytestjig1.hpp"

namespace testjig
{
    TestJigFactory* TestJigFactory::m_pInstance = 0;

    TestJigFactory::TestJigFactory(QObject* parent) :
        QObject(parent)
    {
    }

    TestJigFactory::~TestJigFactory()
    {
        qDebug() << "Deleting TestJigFactory";
    }

    TestJigFactory* TestJigFactory::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new TestJigFactory;
        }

        return m_pInstance;
    }

    TestJig* TestJigFactory::makeTestJig(QObject* parent)
    {
        // If no jig is found then null pointer will be returned
        TestJig* retVal = 0;

        // Create a serial communications object
        comms::DirectSerialComms serialComms;

        // Open serial port
        if (serialComms.openPort(common::Settings::instance()->jigPortName()))
        {
            // Create a register comms object
            comms::RegisterComms regs(&serialComms);

            // Create a details instance to get the jig type
            TestJigDetails details(&regs);

            // Get the connected jig type
            testjig::JigType type = details.connectedJigType();

            // Now close the serial port so that the new jig object can open it on creation
            serialComms.closePort();

            if (type == JigType::MERCURY_JIG_1)
            {
                retVal = new MercuryTestJig1(parent);
            }
        }

        return retVal;
    }
}
