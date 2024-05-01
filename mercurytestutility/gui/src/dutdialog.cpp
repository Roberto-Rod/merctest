#include "dutdialog.hpp"
#include "deviceundertest.hpp"

#include <QtWidgets>

namespace gui
{
    DutDialog::DutDialog(dut::DeviceUnderTest* pDut, QWidget* parent) :
        ModalDialog("DUT Details", parent),
        m_pDut(pDut),
        m_pSerialEdit(0),
        m_pVariantCombo(0)
    {
    }

    DutDialog::~DutDialog()
    {
    }

    int DutDialog::exec()
    {
        // Initialise this DUT dialog
        init();

        // Call base class exec method
        return ModalDialog::exec();
    }

    void DutDialog::init()
    {
        setLayout(createLayout());
        setFixedSize(sizeHint());
    }

    QLayout* DutDialog::createLayout()
    {
        // Create the main layout and add the sub-layouts
        QVBoxLayout* layout = new QVBoxLayout();

        // Add the serial number layout
        layout->addLayout(createSerialLayout());

        // Add the variant layout
        layout->addLayout(createVariantLayout());

        // Add the standard OK/Cancel buttons
        QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok |
                                                          QDialogButtonBox::StandardButton::Cancel);

        connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

        layout->addWidget(buttons);

        return layout;
    }

    QLayout* DutDialog::createSerialLayout()
    {
        // Create the edit box
        m_pSerialEdit = new QLineEdit;
        m_pSerialEdit->setText(m_pDut->serialNumber());
        m_pSerialEdit->setValidator(new QIntValidator(0, kSerialNrMax));
        m_pSerialEdit->setMaxLength(kSerialNrLen);
        m_pSerialEdit->selectAll();

        // Add the widgets to a new layout
        QVBoxLayout* vLayout = new QVBoxLayout;
        vLayout->addWidget(new QLabel("Serial Number:"));

        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->addWidget(m_pSerialEdit);
        hLayout->addStretch();

        vLayout->addLayout(hLayout);

        return vLayout;
    }

    QLayout* DutDialog::createVariantLayout()
    {
        // Create a horizontal layout and add label/combo
        QVBoxLayout* layout = new QVBoxLayout;
        QList<dut::Variant*> variants = m_pDut->variants();

        // If the length of the variants list is 1 then there is only a single
        // variant and we don't need the combo box
        if (variants.length() > 1)
        {
            // Create the combo box with the bands in it
            m_pVariantCombo = new QComboBox;

            // Start at index 1 (don't display generic variant)
            for (int i = 1; i < variants.length(); i++)
            {
                m_pVariantCombo->addItem(variants.at(i)->nameAndNumber());
            }

            // Set the currently selected item to the band set in the DUT item
            m_pVariantCombo->setCurrentText(m_pDut->selectedVariant()->nameAndNumber());

            layout->addWidget(new QLabel("Variant:"));
            layout->addWidget(m_pVariantCombo);
        }

        return layout;
    }

    void DutDialog::accept()
    {
        // Convert the serial number string to int and then back to string padded with leading zeros
        bool ok;
        int32_t serial = m_pSerialEdit->text().toInt(&ok);

        if (ok)
        {
            // Pad the serial number with leading zeros
            m_pDut->setSerialNumber(QString("%1").arg(serial, kSerialNrLen, 10, QChar('0')));
        }

        // If we have created the variant combo box...
        if (m_pVariantCombo != 0)
        {
            // Set variant in the DUT object
            int idx = m_pVariantCombo->currentIndex();

            if (idx >= 0)
            {
                // Add 1 to index as generic variant is not included in combo box
                m_pDut->setSelectedVariant(idx + 1);
            }
        }

        QDialog::accept();
    }
}
