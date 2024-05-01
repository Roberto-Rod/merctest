#include "fileselectdialog.hpp"

#include <QtWidgets>

namespace gui
{
    FileSelectDialog::FileSelectDialog(QString description, QString fileName, QWidget* parent) :
        ModalDialog("Select File", parent),
        m_description(description),
        m_fileName(fileName)
    {
        // Setup the dialog
        setLayout(createLayout());
        setFixedSize(sizeHint());
    }

    QLayout* FileSelectDialog::createLayout()
    {
        QVBoxLayout* layout = new QVBoxLayout;

        layout->addLayout(createLabelLayout());
        layout->addLayout(createFileEditLayout());
        layout->addLayout(createButtonLayout());

        return layout;
    }

    QLayout* FileSelectDialog::createLabelLayout()
    {
        QHBoxLayout* layout = new QHBoxLayout;

        layout->addWidget(new QLabel("Please select " + m_description + " file:"));
        layout->addStretch();

        return layout;
    }

    QLayout* FileSelectDialog::createFileEditLayout()
    {
        // Create filename line edit
        m_pLineEdit = new QLineEdit(m_fileName);

        // Create the elipsis push button to bring up full file dialog
        QPushButton* fileSelectButton = new QPushButton("...");
        fileSelectButton->setStyleSheet("padding-left: 0px; padding-right: 0px");
        fileSelectButton->setFixedSize(fileSelectButton->minimumSizeHint());
        connect(fileSelectButton, SIGNAL(pressed()), this, SLOT(fileSelect()));

        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(m_pLineEdit);
        layout->addWidget(fileSelectButton);

        return layout;
    }

    QLayout* FileSelectDialog::createButtonLayout()
    {
        QHBoxLayout* layout = new QHBoxLayout;

        QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok |
                                                          QDialogButtonBox::StandardButton::Cancel);

        // Connect buttons to accept/reject slots
        connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

        layout->addWidget(buttons);

        return layout;
    }

    QString FileSelectDialog::fileName()
    {
        return m_fileName;
    }

    void FileSelectDialog::fileSelect()
    {
        QFileDialog dialog(this);

        dialog.setFileMode(QFileDialog::ExistingFile);

        if (QFile::exists(m_fileName))
        {
            QFileInfo info(m_fileName);
            dialog.selectFile(m_fileName);
            dialog.setDirectory(info.dir());
        }
        else
        {
            dialog.setDirectory(QDir::homePath());
        }

        if (dialog.exec())
        {
            if (dialog.selectedFiles().length() > 0)
            {
                m_fileName = dialog.selectedFiles().at(0);
                m_pLineEdit->setText(m_fileName);
            }
        }
    }

    void FileSelectDialog::accept()
    {
        m_fileName = m_pLineEdit->text();

        ModalDialog::accept();
    }
}
