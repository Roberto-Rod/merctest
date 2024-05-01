#include "cablecaldialog.hpp"
#include "cablecal.hpp"
#include "settings.hpp"
#include "testequipmentstore.hpp"

#include <QtWidgets>

namespace gui
{
    CableCalGroup::CableCalGroup(CableCalDetails* details, int index, QWidget* parent) :
        QGroupBox(details->m_name, parent),
        m_details(details),
        m_index(index)
    {
        setLayout(createLayout());
    }

    QLayout* CableCalGroup::createLayout()
    {
        QGridLayout* layout = new QGridLayout;

        m_pLastCal = new QLabel;
        m_pCalStatus = new QLabel;
        updateStatus();

        layout->addWidget(new QLabel("Calibration Expires:"), 0, 0);
        layout->addWidget(m_pLastCal, 0, 1);
        layout->addWidget(new QLabel("Calibration Status:"), 1, 0);
        layout->addWidget(m_pCalStatus, 1, 1);

        QPushButton* button = new QPushButton("Calibrate");
        connect(button, SIGNAL(pressed()), this, SLOT(calButtonPressed()));
        layout->addWidget(button, 2, 0, 1, 2, Qt::AlignCenter);

        return layout;
    }

    void CableCalGroup::updateStatus()
    {
        common::Calibration cal(m_details->m_calKey);
        cal.setWarnDays(testjig::CableCal::kCalWarnDays);

        m_pLastCal->setText(cal.calExpiry().toString("dd/MM/yyyy"));
        m_pCalStatus->setText(cal.status());
    }

    void CableCalGroup::calOk()
    {
        common::Calibration cal(m_details->m_calKey);
        cal.setCalExpiry(QDate::currentDate().addDays(testjig::CableCal::kCalValidDays));
        updateStatus();
    }

    void CableCalGroup::calButtonPressed()
    {
        // Check that necessary TE is available, if so then run cal
        QList<te::TestEquipment*> teList;
        teList.append(te::TestEquipmentStore::te(te::ResourceType::SignalGenerator));
        teList.append(te::TestEquipmentStore::te(te::ResourceType::PowerMeter));

        if (te::TestEquipment::checkTeAvailable(teList))
        {
            emit runCal(m_index);
        }
    }

    CableCalDialog::CableCalDialog(const QList<CableCalDetails*> details, QWidget* parent) :
        ModalDialog("Cable Calibration", parent),
        m_details(details)
    {
        setLayout(createLayout());
        setFixedSize(sizeHint());
    }

    void CableCalDialog::calOk(int index)
    {
        // Call the slot in the appropriate group box
        if (index < m_groupBoxes.length())
        {
            m_groupBoxes.at(index)->calOk();
        }

        setFixedSize(sizeHint());
    }

    QLayout* CableCalDialog::createLayout()
    {
        QLayout* layout = new QVBoxLayout;

        // Create the group boxes
        for (int i = 0; i < m_details.length(); i++)
        {
            CableCalGroup* group = new CableCalGroup(m_details.at(i), i, this);
            connect(group, SIGNAL(runCal(int)), this, SIGNAL(runCal(int)));
            m_groupBoxes.append(group);
            layout->addWidget(group);
        }

        // Add the standard OK/Cancel buttons
        QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok);

        connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));

        layout->addWidget(buttons);

        return layout;
    }
}
