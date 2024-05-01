
/****************************************************************************
**
**
****************************************************************************/

#include <QtWidgets>
#include <QSerialPortInfo>

#include "serialportsdialog.hpp"
#include "settings.hpp"

namespace gui
{
    SerialPortsDialog::SerialPortsDialog(QWidget* parent) :
        QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
    {
        // Create a vertical layout
        QVBoxLayout *pLayout = new QVBoxLayout;

        // Create the widgets and add them to the layout
        pLayout->addWidget(createPortSelector());
        pLayout->addWidget(createButtons());

        // Assign the layout to the parent (this)
        setLayout(pLayout);

        // Set the QDialog properties
        setWindowTitle(tr("Serial Port Configuration"));
        setFixedSize(kWidth, sizeHint().height());
        setSizeGripEnabled(false);
    }

    QWidget* SerialPortsDialog::createPortSelector()
    {
        m_pJigPortCombo = new QComboBox();
        m_pSrcPortCombo = new QComboBox();
        m_pCtsPortCombo = new QComboBox();

        m_pSrcPortCombo->addItem("<not assigned>");
        m_pCtsPortCombo->addItem("<not assigned>");

        QGridLayout* layout = new QGridLayout();

        layout->addWidget(new QLabel(tr("Test Jig Port:")), 0, 0);
        layout->addWidget(new QLabel(tr("Source Board Port:")), 1, 0);
        layout->addWidget(new QLabel(tr("CTS Board Port:")), 2, 0);
        layout->addWidget(m_pJigPortCombo, 0, 1);
        layout->addWidget(m_pSrcPortCombo, 1, 1);
        layout->addWidget(m_pCtsPortCombo, 2, 1);

        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        QList<QSerialPortInfo>::iterator port;

        for (port = ports.begin(); port != ports.end(); port++)
        {
            QString portName = port->portName();
            m_pJigPortCombo->addItem(portName);
            m_pSrcPortCombo->addItem(portName);
            m_pCtsPortCombo->addItem(portName);
        }

        int32_t index;

        index = m_pJigPortCombo->findText(common::Settings::instance()->jigPortName());
        if (index != -1) { m_pJigPortCombo->setCurrentIndex(index); }
        else             { m_pJigPortCombo->setCurrentIndex(0); }

        index = m_pSrcPortCombo->findText(common::Settings::instance()->srcPortName());
        if (index != -1) { m_pSrcPortCombo->setCurrentIndex(index); }
        else             { m_pSrcPortCombo->setCurrentIndex(0); }

        index = m_pCtsPortCombo->findText(common::Settings::instance()->ctsPortName());
        if (index != -1) { m_pCtsPortCombo->setCurrentIndex(index); }
        else             { m_pCtsPortCombo->setCurrentIndex(0); }

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }

    QWidget* SerialPortsDialog::createButtons()
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

    void SerialPortsDialog::accept()
    {
        common::Settings::instance()->setJigPortName(m_pJigPortCombo->currentText());
        common::Settings::instance()->setSrcPortName(m_pSrcPortCombo->currentText());
        common::Settings::instance()->setCtsPortName(m_pCtsPortCombo->currentText());

        // Call the base class method
        QDialog::accept();
    }
}
