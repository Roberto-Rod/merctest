#include "equipmentwidget.hpp"
#include "settings.hpp"

#include <QtWidgets>
#include <QSerialPort>

namespace gui
{
    EquipmentWidget::EquipmentWidget(QWidget* parent) :
        QGroupBox(tr("Equipment"), parent)
    {
        setLayout(createLayout());
        hookUpSignals();
    }

    EquipmentWidget::~EquipmentWidget()
    {
    }

    QLayout* EquipmentWidget::createLayout()
    {
        QVBoxLayout* pLayout = new QVBoxLayout;
        pLayout->addWidget(createEquipmentTreeWidget());
        pLayout->addLayout(createButtonLayout());

        return pLayout;
    }

    QLayout* EquipmentWidget::createButtonLayout()
    {
        m_pRefreshButton = new QPushButton(tr("Refresh All"));

        QHBoxLayout* pLayout = new QHBoxLayout;
        pLayout->addWidget(m_pRefreshButton);
        pLayout->addStretch();

        return pLayout;
    }

    QWidget* EquipmentWidget::createEquipmentTreeWidget()
    {
        QWidget *parent = 0;
        m_pEquipmentTreeWidget = new EquipmentTreeWidget(parent);

        return m_pEquipmentTreeWidget;
    }

    void EquipmentWidget::hookUpSignals()
    {
        // Connect the refresh button to the refreshClicked signal
        connect(m_pRefreshButton, SIGNAL(clicked()),
                this,             SIGNAL(refreshClicked()));

        // Emit a signal when a TE item is double clicked
        connect(m_pEquipmentTreeWidget, SIGNAL(teDoubleClicked(int)),
                this,                   SIGNAL(teDoubleClicked(int)));

        // Emit a signal when a test item is single clicked
        connect(m_pEquipmentTreeWidget, SIGNAL(setTestSuite(testsuite::TestCase*)),
                this,                   SIGNAL(setTestSuite(testsuite::TestCase*)));
    }

    void EquipmentWidget::updateDetails()
    {
        m_pEquipmentTreeWidget->updateDetails();
    }

    void EquipmentWidget::setTestJig(testjig::TestJig* pTestJig)
    {
        m_pEquipmentTreeWidget->setTestJig(pTestJig);
    }
}
