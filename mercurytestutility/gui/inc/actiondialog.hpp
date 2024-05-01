#ifndef ACTIONDIALOG_HPP
#define ACTIONDIALOG_HPP

#include "modaldialog.hpp"

class QLabel;

namespace gui
{
    class ActionDialog : public ModalDialog
    {
        Q_OBJECT

    public:
        ActionDialog(QString title, QString msg, QWidget* parent = 0);
        ~ActionDialog();

        bool status();

    signals:
        void getTestState();

    public slots:
        int exec();
        void testState(bool state);
        void updateMessage(QString msg);

    private slots:
        void cancelTest();
        void failClicked();

    private:
        QLayout* createLayout(const QString msg);
        QLayout* createButtonLayout();
        QLabel* m_pMessage;

        bool m_passed;
        bool m_complete;
        bool m_cancelled;
    };
}

#endif // ACTIONDIALOG_HPP
