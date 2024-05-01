#ifndef MESSAGEDIALOG_HPP
#define MESSAGEDIALOG_HPP

#include "modaldialog.hpp"

QT_BEGIN_INCLUDE_NAMESPACE
class QLabel;
QT_END_INCLUDE_NAMESPACE

namespace gui
{
    enum class MessageDialogType
    {
        INFO,
        WARN,
        ERR
    };

    class MessageDialog : public ModalDialog
    {
    public:
        MessageDialog(QString msg, MessageDialogType type = MessageDialogType::INFO,
                       bool okButton = true, bool cancelButton = true, QWidget* parent = 0);

        ~MessageDialog();

        void setMessage(QString msg);

        void appendMessage(QString msg);

    private:
        bool m_okButton;
        bool m_cancelButton;
        QLabel* m_msgLabel;

        QLayout* createLayout(QString msg);
    };
}

#endif // MESSAGEDIALOG_HPP
