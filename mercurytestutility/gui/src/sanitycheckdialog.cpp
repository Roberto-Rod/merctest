#include "sanitycheckdialog.hpp"
#include "cablecal.hpp"
#include "settings.hpp"
#include "testequipmentstore.hpp"

#include <QtWidgets>

namespace gui
{
     SanityCheckGroup::SanityCheckGroup(SanityCheckDetails* details, int index, QWidget* parent) :
        QGroupBox(details->m_name, parent),
        m_details(details),
        m_index(index)
    {
        setLayout(createLayout());
    }

    QLayout*  SanityCheckGroup::createLayout()
    {
        QGridLayout* layout = new QGridLayout;

        m_pLastCal = new QLabel;
        m_pCalStatus = new QLabel;
        updateStatus();

        layout->addWidget(new QLabel("Test Date:"), 0, 0);
        layout->addWidget(m_pLastCal, 0, 1);
        layout->addWidget(new QLabel("Max Error dB (Low attn., High attn.):"), 1, 0);
        layout->addWidget(m_pCalStatus, 1, 1);

        QPushButton* button = new QPushButton("Test Rf Power");
        connect(button, SIGNAL(pressed()), this, SLOT(calButtonPressed()));
        layout->addWidget(button, 2, 0, 1, 2, Qt::AlignCenter);

        return layout;
    }

    void  SanityCheckGroup::updateStatus()
    {
        testjig::sanityResult sr,sr2;
        QString res, neg, pos, neg2, pos2;

        QDate dt = QDate::currentDate();
        QString date(dt.toString("ddd, MMM dd, yyyy"));

        m_pLastCal->setText(date);

        neg2 = "";
        pos2 = "";

        common::Settings::instance()->getSanityResults(static_cast<testjig::SanityCheckType>(m_index), sr);
        // If Highband groupbox output second result.
        if(m_index == 2)
        {
            common::Settings::instance()->getSanityResults(static_cast<testjig::SanityCheckType>(m_index + 1), sr2);
            neg2 = QString::number(sr2.maxErrNeg);
            pos2 = QString::number(sr2.maxErrPos);
        }
        // If no settings key, the result defaults to 99.0
        if(sr.maxErrNeg != 99.0)
        {
            m_pLastCal->setText(date);
            neg = QString::number(sr.maxErrNeg);
            pos = QString::number(sr.maxErrPos);
            res = neg + "   " + pos + ",   " + neg2 + "   " + pos2;
            m_pCalStatus->setText(res);
        }
        else
        {
            m_pLastCal->setText("");
            m_pCalStatus->setText("");
        }
    }

    void  SanityCheckGroup::sanityOk()
    {
        updateStatus();
    }

    void  SanityCheckGroup::calButtonPressed()
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

     SanityCheckDialog::SanityCheckDialog(const QList< SanityCheckDetails*> details, QWidget* parent) :
        ModalDialog("Sanity Check Calibration", parent),
        m_details(details)
    {
        setLayout(createLayout());
        setFixedSize(sizeHint());
    }

    void  SanityCheckDialog::sanityOk(int index)
    {
        // Call the slot in the appropriate group box
        if (index < m_groupBoxes.length())
        {
            m_groupBoxes.at(index)->sanityOk();
        }

        setFixedSize(sizeHint());
    }

    QLayout*  SanityCheckDialog::createLayout()
    {
        QLayout* layout = new QVBoxLayout;

        // Create the group boxes
        for (int i = 0; i < m_details.length(); i++)
        {
             SanityCheckGroup* group = new  SanityCheckGroup(m_details.at(i), i, this);
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
