#ifndef MANUALTESTDIALOG_HPP
#define MANUALTESTDIALOG_HPP

#include "modaldialog.hpp"

namespace gui
{
    class ManualTestDialog : public ModalDialog
    {
        Q_OBJECT

    public:
        ManualTestDialog(QString title, QString msg, QWidget* parent = 0);
        ~ManualTestDialog();

        bool status();

    private slots:
        void passClicked();
        void failClicked();

    private:
        void createLayout(const QString msg);
        QLayout* createButtonLayout();

        bool m_status;
    };
}

#endif // MANUALTESTDIALOG_HPP
