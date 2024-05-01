#include "actiondialog.hpp"
#include "settings.hpp"

#include <QtWidgets>

namespace gui
{
    ActionDialog::ActionDialog(QString title, QString msg, QWidget* parent) :
        ModalDialog(title, parent),
        m_passed(false),
        m_complete(false),
        m_cancelled(false)
    {
        setLayout(createLayout(msg));
        setFixedSize(sizeHint());
    }

    ActionDialog::~ActionDialog()
    {
    }

    QLayout* ActionDialog::createLayout(const QString msg)
    {
        QVBoxLayout* layout = new QVBoxLayout();

        // Add the message
        m_pMessage = new QLabel(msg);
        layout->addWidget(m_pMessage);

        // Add some space
        layout->addSpacing(kSpaceBeforeButtons);

        // Add the buttons
        layout->addLayout(createButtonLayout());

        return layout;
    }

    QLayout* ActionDialog::createButtonLayout()
    {
        // Create the fail and cancel buttons
        QPushButton* failButton = new QPushButton("&FAIL");
        QPushButton* cancelButton = new QPushButton("&Cancel Test");
        failButton->setAutoDefault(false);
        cancelButton->setAutoDefault(false);

        // Add to the dialog box
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(failButton);
        buttonLayout->addWidget(cancelButton);

        // Connect the fail button signal
        connect(failButton, SIGNAL(clicked()), this, SLOT(failClicked()));

        // Connect the cancel button's rejected signal
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
        connect(this, SIGNAL(rejected()), this, SLOT(cancelTest()));

        return buttonLayout;
    }

    int ActionDialog::exec()
    {
        int retVal = QDialog::Accepted;

        setModal(true);
        show();

        // Continue until the test is complete (pass or fail) or has been cancelled
        while (!m_complete && !m_cancelled)
        {
            QApplication::processEvents();
            emit getTestState();
        }

        // If dialog was cancelled then set return value to rejected
        if (m_cancelled)
        {
            retVal = QDialog::Rejected;
        }

        return retVal;
    }

    void ActionDialog::cancelTest()
    {
        m_cancelled = true;
    }

    void ActionDialog::failClicked()
    {
        // Set complete to true, leave m_passed = false
        m_complete = true;
    }

    void ActionDialog::testState(bool state)
    {
        // We only take action on a pass state here because this method will
        // continually be called with a fail state whilst the manual test is performed
        // and until it passed
        if (state)
        {
            m_passed = true;
            m_complete = true;
        }
    }

    void ActionDialog::updateMessage(QString msg)
    {
        m_pMessage->setText(msg);
        setFixedSize(sizeHint());
    }

    bool ActionDialog::status()
    {
        // If passed flag was set then return true;
        return m_passed;
    }
}
