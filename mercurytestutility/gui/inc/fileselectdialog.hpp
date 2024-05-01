#ifndef FILESELECTDIALOG_HPP
#define FILESELECTDIALOG_HPP

#include "modaldialog.hpp"

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

namespace gui
{
    class FileSelectDialog : public ModalDialog
    {
        Q_OBJECT

    public:
        FileSelectDialog(QString description, QString fileName, QWidget* parent = 0);

        ~FileSelectDialog() {}

        QString fileName();

    public slots:
        void fileSelect();
        void accept();

    private:
        QString m_description;
        QString m_fileName;

        QLineEdit* m_pLineEdit;

        QLayout* createLayout();
        QLayout* createLabelLayout();
        QLayout* createFileEditLayout();
        QLayout* createButtonLayout();
    };
}

#endif // FILESELECTDIALOG_HPP
