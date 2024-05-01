#include "tetreewidgetitem.hpp"
#include "testequipment.hpp"
#include "testequipmentstore.hpp"

#include <QApplication>
#include <QClipboard>

namespace gui
{
    TeTreeWidgetItem::TeTreeWidgetItem(QTreeWidgetItem *parent, te::TestEquipment* pTe) :
        QTreeWidgetItem(parent),
        //QObject(0),
        m_pTe(pTe)
    {
        updateDetails();
        copyDetails();
    }

    void TeTreeWidgetItem::updateDetails()
    {
        // Re-attempt connection to test equipment to get details
        m_pTe->connect();

        bool teFound = false;
        if (m_pTe->details().m_model.indexOf("not found") == -1)
        {
            teFound = true;
        }

        // Set the text in the tree widget columns
        setText(0, m_pTe->resourceTypeString());
        setText(1, m_pTe->details().m_model);
        // Only display cal icon if TE has been found
        if (teFound) { setIcon(2, m_pTe->calStatusIcon()); }

        // Set the tool tip text
        QString tip = m_pTe->formattedDetails(true);
        setToolTip(0, tip);
        setToolTip(1, tip);
        // Only display cal tool tip if TE has been found
        if (teFound) { setToolTip(2, m_pTe->calStatusText()); }
    }

    te::ResourceType TeTreeWidgetItem::resourceType()
    {
        return m_pTe->resourceType();
    }

    void TeTreeWidgetItem::copyDetails()
    {
        QApplication::clipboard()->setText(m_pTe->formattedDetails());
    }
}
