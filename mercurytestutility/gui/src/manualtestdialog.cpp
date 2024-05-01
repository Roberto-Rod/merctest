#include "manualtestdialog.hpp"
#include "settings.hpp"

#include <QtWidgets>

namespace gui
{
    ManualTestDialog::ManualTestDialog(QString title, QString msg, QWidget* parent) :
        ModalDialog(title, parent),
        m_status(false)
    {
        createLayout(msg);
        setFixedSize(sizeHint());
    }

    ManualTestDialog::~ManualTestDialog()
    {
    }

    void ManualTestDialog::createLayout(const QString msg)
    {
        QVBoxLayout* layout = new QVBoxLayout();

        // Add the label
        layout->addWidget(new QLabel(msg));

        // Add some space
        layout->addSpacing(kSpaceBeforeButtons);

        // Add the buttons
        layout->addLayout(createButtonLayout());

        setLayout(layout);
    }

    QLayout* ManualTestDialog::createButtonLayout()
    {
        // Create the buttons
        QPushButton* passButton = new QPushButton("&PASS");
        QPushButton* failButton = new QPushButton("&FAIL");
        QPushButton* cancelButton = new QPushButton("&Cancel Test");

        passButton->setDefault(true);
        failButton->setAutoDefault(false);
        cancelButton->setAutoDefault(false);

        // Add to the dialog box
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(passButton);
        buttonLayout->addWidget(failButton);
        buttonLayout->addStretch();
        buttonLayout->addWidget(cancelButton);

        // Connect the pass/fail buttons signals
        connect(passButton, SIGNAL(clicked()), this, SLOT(passClicked()));
        connect(failButton, SIGNAL(clicked()), this, SLOT(failClicked()));

        // Connect the cancel button's rejected signal
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

        return buttonLayout;
    }

    bool ManualTestDialog::status()
    {
        return m_status;
    }

    void ManualTestDialog::passClicked()
    {
        m_status = true;
        accept();
    }

    void ManualTestDialog::failClicked()
    {
        m_status = false;
        accept();
    }
}
