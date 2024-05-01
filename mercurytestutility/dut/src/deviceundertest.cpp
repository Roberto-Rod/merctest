#include "deviceundertest.hpp"
#include "testjig.hpp"

namespace dut
{
    DeviceUnderTest::DeviceUnderTest(testjig::TestJig* parentJig) :
        QObject(parentJig),
        m_serialNumber(QString("000000")),
        m_pTopLevelTestCase(0),
        m_pTestHarness(0),
        m_selectedVariant(0u)
    {
    }

    DeviceUnderTest::~DeviceUnderTest()
    {
        // Delete the Variant objects in the variant list
        for (int i = 0; i < m_variants.length(); i++)
        {
            delete m_variants.at(i);
        }
    }

    QString DeviceUnderTest::serialNumber() const
    {
        return m_serialNumber;
    }

    bool DeviceUnderTest::setSerialNumber(QString ser)
    {
        m_serialNumber = ser;

        return true;
    }

    common::AssyRevision* DeviceUnderTest::assyRevision() const
    {
        return m_pRevision;
    }

    testsuite::TestCase* DeviceUnderTest::topLevelTestCase() const
    {
        return m_pTopLevelTestCase;
    }

    testjig::TestHarness* DeviceUnderTest::testHarness() const
    {
        return m_pTestHarness;
    }

    QList<Variant*> DeviceUnderTest::variants() const
    {
        return m_variants;
    }

    bool DeviceUnderTest::isSingleVariant()
    {
       bool result = false;
       QList<Variant*> variants = this->variants();

       if (variants.size() == 1)
       {
           result = true;
       }
       return result;
    }

    const Variant* DeviceUnderTest::selectedVariant() const
    {
        return m_variants.at(m_selectedVariant);
    }

    const Variant* DeviceUnderTest::genericVariant() const
    {
        return m_variants.at(0);
    }

    bool DeviceUnderTest::setSelectedVariant(int intSelectedVariant)
    {
        bool retVal = false;

        if (intSelectedVariant < m_variants.length())
        {
            m_selectedVariant = intSelectedVariant;
            retVal = true;
        }

        return retVal;
    }

}
