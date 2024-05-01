#include "equipmenttreewidget.hpp"
#include "tetreewidgetitem.hpp"
#include "testequipmentstore.hpp"
#include "dutdialog.hpp"
#include "testjig.hpp"
#include "deviceundertest.hpp"
#include "duttreewidgetitem.hpp"
#include "testjigtreewidgetitem.hpp"

namespace gui
{
    EquipmentTreeWidget::EquipmentTreeWidget(QWidget *parent) :
        QTreeWidget(parent)
    {
        setColumnCount(3);
        setColumnWidth(0, 230);
        setColumnWidth(1, 185);
        setHeaderHidden(true);
        setSelectionMode(QAbstractItemView::SingleSelection);

        // Create the top level test equipment item to the tree
        QTreeWidgetItem* teTop = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << tr("Test Equipment"));

        // Add all the test equipment items to the tree
        QListIterator<te::TestEquipment*> i(te::TestEquipmentStore::instance()->te());
        while (i.hasNext()) { new TeTreeWidgetItem(teTop, i.next()); }

        QTreeWidgetItem* dutTop = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << tr("Test Jig (none detected)"));

        addTopLevelItem(teTop);
        addTopLevelItem(dutTop);

        teTop->setExpanded(true);

        connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));

        connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
                this, SLOT(itemClicked(QTreeWidgetItem*,int)));
    }

    EquipmentTreeWidget::~EquipmentTreeWidget()
    {
    }

    void EquipmentTreeWidget::itemDoubleClicked(QTreeWidgetItem* item, int column)
    {
        // Not interested in column, but we've received it so just suppress a compiler warning
        (void)column;

        // Is this one of the test equipment items?
        if (item->parent() == topLevelItem(0))
        {
            // Perform a dynamic cast to a TeTreeWidgetItem, it should be one of these...
            TeTreeWidgetItem* teItem = dynamic_cast<TeTreeWidgetItem*>(item);

            // ... but test just in case it isn't
            if (teItem != NULL)
            {
                emit teDoubleClicked(item->parent()->indexOfChild(item));
            }
        }
        // Or is it one of the DUTs?
        else if (item->parent() == topLevelItem(1))
        {
            // Perform a dynamic cast to a DutTreeWidgetItem, it should be one of these...
            DutTreeWidgetItem* dutItem = dynamic_cast<DutTreeWidgetItem*>(item);

            if (dutItem != 0)
            {
                // Now make the DUT dialog box and fire it up
                DutDialog dialog(dutItem->dut());
                dialog.exec();

                // Emit the setTestSuite signal to refresh the test view
                emit setTestSuite(dutItem->dut()->topLevelTestCase());
            }
        }
    }

    void EquipmentTreeWidget::itemClicked(QTreeWidgetItem* item, int column)
    {
        // Not interested in column, but we've received it so just suppress a compiler warning
        (void)column;

        // Is this one of the test equipment items?
        if (item->parent() == topLevelItem(0))
        {
            // Do nothing with these for now
        }
        // Is this one of the DUTs?
        if (item->parent() == topLevelItem(1))
        {
            // Perform a dynamic cast to a DutTreeWidgetItem, it should be one of these...
            DutTreeWidgetItem* dutItem = dynamic_cast<DutTreeWidgetItem*>(item);

            // ... but test just in case it isn't
            if (dutItem != NULL)
            {
                emit setTestSuite(dutItem->dut()->topLevelTestCase());
            }
        }
    }

    void EquipmentTreeWidget::updateDetails()
    {
        // Go through all of the test equipment items
        for (int i = 0; i < topLevelItem(0)->childCount(); i++)
        {
            TeTreeWidgetItem* item = dynamic_cast<TeTreeWidgetItem*>(topLevelItem(0)->child(i));

            if (item != 0)
            {
                item->updateDetails();
            }
        }

        // Now update the test jig details (if present)
        TestJigTreeWidgetItem* item = dynamic_cast<TestJigTreeWidgetItem*>(topLevelItem(1));

        if (item != 0)
        {
            item->update();
        }
    }

    void EquipmentTreeWidget::setTestJig(testjig::TestJig* pTestJig)
    {
        // Delete current DUT top level item, if it exists
        if (topLevelItem(1) != 0)
        {
            delete topLevelItem(1);
        }

        if (pTestJig != 0)
        {
            // Create the new top level item
            TestJigTreeWidgetItem* jigItem = new TestJigTreeWidgetItem(pTestJig);

            // Add all the DUTs as children
            for (int i = 0; i < pTestJig->duts()->length(); i++)
            {
                DutTreeWidgetItem* dutItem = new DutTreeWidgetItem(jigItem, pTestJig->duts()->at(i));
                jigItem->addChild(dutItem);
            }

            // Add top-level item to this tree widget
            addTopLevelItem(jigItem);

            // Expand top-level item
            jigItem->setExpanded(true);
        }
        else
        {
            emit setTestSuite(0);
        }
    }
}
