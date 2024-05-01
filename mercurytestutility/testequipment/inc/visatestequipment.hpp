#ifndef VISATESTEQUIPMENT_HPP
#define VISATESTEQUIPMENT_HPP

#include "testequipment.hpp"
#include "visaresource.hpp"

namespace te
{
    class VisaTestEquipment : public TestEquipment
    {
    public:
        //VisaTestEquipment();
        VisaTestEquipment(const QStringList& expectedModels, const QString& resourceName = "");

        ~VisaTestEquipment();

        void setResourceName(const QString& resourceName);

        bool connect();

        bool expectedModelMatch(QString model);

        /** Overrides te::TestEquipment::resourceType */
        virtual ResourceType resourceType()= 0;

        /** Overrides te::TestEquipment::resourceTypeString */
        virtual QString resourceTypeString()= 0;

        /** Overrides te::TestEquipment::reset */
        virtual bool reset()= 0;

    protected:
        VisaResource* m_pVisaResource;

    private:
        QStringList m_expectedModels;
    };
}

#endif // VISATESTEQUIPMENT_HPP
