/****************************************************************************
**
**
****************************************************************************/

#include <QtWidgets>
#include <QList>
#include <QListIterator>
#include "testequipmentdialog.hpp"
#include "testequipmentstore.hpp"
#include "visaspectrumanalyser.hpp"

namespace gui
{
    TestEquipmentDialog::TestEquipmentDialog(int32_t tabIndex, QWidget* parent) :
        QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
    {
        // Create a vertical layout
        QVBoxLayout *pLayout = new QVBoxLayout;

        // Create the widgets and add them to the layout
        pLayout->addWidget(createTabWidget(tabIndex));
        pLayout->addWidget(createButtons());

        // Assign the layout to the parent (this)
        setLayout(pLayout);

        // Set the QDialog properties
        setWindowTitle(tr("Test Equipment Configuration"));
        setFixedSize(kWidth, sizeHint().height());
        setSizeGripEnabled(false);
    }

    QWidget* TestEquipmentDialog::createTabWidget(int32_t tabIndex)
    {
        // Create a new tab widget
        QTabWidget* pTabWidget = new QTabWidget;

        // Get the test equipment list from the "Test Equipment Store" singleton class
        const QList<te::TestEquipment*>& te = te::TestEquipmentStore::instance()->te();

        // Create TE details widgets for each test equipment item
        QListIterator<te::TestEquipment*> i(te);
        while (i.hasNext())
        {
            te::TestEquipment* pTeItem = i.next();
            TeDetailsWidget* widge = new TeDetailsWidget(pTeItem);
            m_teDetailsWidgets.append(widge);
            pTabWidget->addTab(widge, pTeItem->resourceTypeString());

            connect(widge, SIGNAL(edited()),
                    this,    SLOT(edited()));
        }

        // Set the current tab index to that passed to this method - check that the index
        // doesn't exceed the number of TE items
        if (tabIndex < te.length())
        {
            pTabWidget->setCurrentIndex(tabIndex);
        }

        return pTabWidget;
    }

    QWidget* TestEquipmentDialog::createButtons()
    {
        QDialogButtonBox* pButtons = new QDialogButtonBox((QDialogButtonBox::Apply |
                                                           QDialogButtonBox::Ok    |
                                                           QDialogButtonBox::Cancel));

        // Maintain the pointer to the apply button as we need to enable/disable it when anything
        // gets changed
        m_pApplyButton = pButtons->button(QDialogButtonBox::Apply);

        // Disable the apply button - this will get enabled when the user has changed something
        m_pApplyButton->setEnabled(false);

        connect(pButtons->button(QDialogButtonBox::Apply), SIGNAL(clicked()), SLOT(applyChanges()));
        connect(pButtons, SIGNAL(accepted()), SLOT(accept()));
        connect(pButtons, SIGNAL(rejected()), SLOT(reject()));

        return pButtons;
    }

    void TestEquipmentDialog::applyChanges()
    {
        // Iterate over all of the TE details widgets and apply changes in each
        QListIterator<TeDetailsWidget*> i(m_teDetailsWidgets);
        while (i.hasNext()) { i.next()->applyChanges(); }

        // Disable the Apply button
        m_pApplyButton->setEnabled(false);
    }

    void TestEquipmentDialog::edited()
    {
        // Something was edited, so enable the "Apply" button
        m_pApplyButton->setEnabled(true);
    }

    void TestEquipmentDialog::accept()
    {
        // If the apply button is enabled then there are changes to apply...
        if (m_pApplyButton->isEnabled())
        {
            // ... so apply them
            applyChanges();
        }

        // Call the base class method
        QDialog::accept();
    }
}
