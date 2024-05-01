#ifndef TETREEWIDGETITEM_HPP
#define TETREEWIDGETITEM_HPP

#include "testequipment.hpp"

#include <QTreeWidgetItem>
#include <QStringList>
//#include <QObject>

namespace gui
{
    class TeTreeWidgetItem : public QTreeWidgetItem
    {
    public:
        TeTreeWidgetItem(QTreeWidgetItem* parent, te::TestEquipment* pTe);

        te::ResourceType resourceType();

        void updateDetails();

        void copyDetails();

    private:
        te::TestEquipment* m_pTe;
    };
}

#endif // TETREEWIDGETITEM_HPP
