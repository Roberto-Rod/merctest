#ifndef DELAYPROGRESSDIALOG_HPP
#define DELAYPROGRESSDIALOG_HPP

#include <QProgressDialog>

namespace gui
{
    class DelayProgressDialog : public QProgressDialog
    {
        Q_OBJECT

    public:
        DelayProgressDialog(const QString& labelText, int32_t milliseconds, QWidget* parent = 0);

        ~DelayProgressDialog() {}
    };
}

#endif // DELAYPROGRESSDIALOG_HPP
