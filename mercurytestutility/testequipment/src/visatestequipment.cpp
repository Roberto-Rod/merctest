#include "visatestequipment.hpp"
#include "logging.hpp"

#include <QDebug>

namespace te
{
    /*VisaTestEquipment::VisaTestEquipment() :
        TestEquipment(),
        m_pVisaResource(new VisaResource)
    {
    }*/

    VisaTestEquipment::VisaTestEquipment(const QStringList& expectedModels, const QString& resourceName) :
        TestEquipment(),
        m_pVisaResource(new VisaResource),
        m_expectedModels(expectedModels)
    {
        // Set the resource name
        setResourceName(resourceName);

        // and attempt to connect - this will get equipment details if it succeeds,
        // if it fails then details won't get updated, just ignore the return value here
        (void)connect();
    }

    VisaTestEquipment::~VisaTestEquipment()
    {
        qDebug() << "Delete VisaTestEquipment";
        delete m_pVisaResource;
    }

    void VisaTestEquipment::setResourceName(const QString& resourceName)
    {
        // Call the parent method
        TestEquipment::setResourceName(resourceName);

        // And also pass the resource name to the visa resource
        m_pVisaResource->setResourceName(resourceName);
    }

    bool VisaTestEquipment::connect()
    {
        bool retVal = false;

        // Always re-open the VISA session when this method is called, this causes
        // the equipment to be re-polled each time
        if (m_pVisaResource->openSession())
        {
            QString details;
            if (m_pVisaResource->details(details))
            {
                // Apply the details string to this object
                setDetails(details);

                // Got some details - does the model match expected?
                if (expectedModelMatch(m_details.m_model))
                {
                    m_detected = true;
                    retVal = true;
                }
            }
        }

        if (!retVal)
        {
            // If connection failed then reset the details
            resetDetails();
            m_detected = false;
        }

        return retVal;
    }

    bool VisaTestEquipment::expectedModelMatch(QString model)
    {
        bool retVal = false;

        foreach (const QString &str, m_expectedModels)
        {
            if (model.indexOf(str) != -1)
            {
                retVal = true;
                break;
            }
        }

        return retVal;
    }
}
