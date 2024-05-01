#include "messagedialog.hpp"

#include <QtWidgets>

namespace gui
{
    MessageDialog::MessageDialog(QString msg, MessageDialogType type,
                                  bool okButton, bool cancelButton, QWidget* parent) :
        ModalDialog("", parent),
        m_okButton(okButton),
        m_cancelButton(cancelButton),
        m_msgLabel(0)
    {
        // Determine the title & icon based on the type that was passed in
        if (type == MessageDialogType::INFO)
        {
            setWindowTitle("Information");
            setWindowIcon(QIcon(":/icons/16x16/dialog-information-3.png"));
        }
        else if (type == MessageDialogType::WARN)
        {
            setWindowTitle("Warning");
            setWindowIcon(QIcon(":/icons/16x16/emblem-important-2.png"));
        }
        else if (type == MessageDialogType::ERR)
        {
            setWindowTitle("ERROR");
            setWindowIcon(QIcon(":/icons/16x16/dialog-error-4.png"));
        }

        // Setup the dialog
        setLayout(createLayout(msg));
        setFixedSize(sizeHint());
    }

    MessageDialog::~MessageDialog()
    {
    }

    QLayout* MessageDialog::createLayout(QString msg)
    {
        // Create the main layout
        QVBoxLayout* layout = new QVBoxLayout();

        // Add a label widget - message will be set later
        m_msgLabel = new QLabel(msg);
        layout->addWidget(m_msgLabel);

        // Add some space
        layout->addSpacing(kSpaceBeforeButtons);

        // Add the standard OK/Cancel buttons
        if (m_okButton || m_cancelButton)
        {
            QDialogButtonBox::StandardButtons stdButtons;
            if (m_okButton)     { stdButtons |= QDialogButtonBox::StandardButton::Ok; }
            if (m_cancelButton) { stdButtons |= QDialogButtonBox::StandardButton::Cancel; }
            QDialogButtonBox* buttons = new QDialogButtonBox(stdButtons);

            // Connect buttons to accept/reject slots
            connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
            connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

            layout->addWidget(buttons);
        }

        return layout;
    }

    void MessageDialog::setMessage(QString msg)
    {
        m_msgLabel->setText(msg);
        setFixedSize(sizeHint());
        QApplication::processEvents();
    }

    void MessageDialog::appendMessage(QString msg)
    {
        m_msgLabel->setText(m_msgLabel->text() + msg);
        setFixedSize(sizeHint());
        QApplication::processEvents();
    }
}
