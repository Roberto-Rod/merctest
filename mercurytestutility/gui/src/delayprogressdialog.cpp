#include "delayprogressdialog.hpp"

#include <QThread>
#include <QApplication>

namespace gui
{
    DelayProgressDialog::DelayProgressDialog(const QString& labelText, int32_t milliseconds, QWidget* parent) :
        QProgressDialog(labelText, "Cancel", 1, milliseconds, parent, Qt::WindowTitleHint)
    {
        setWindowModality(Qt::ApplicationModal);
        setCancelButton(0);
        setFixedSize(sizeHint());
        show();

        for (int32_t i = 0; i < milliseconds; i++)
        {
            setValue(i);
            QApplication::processEvents();
            QThread::msleep(1);
        }

        hide();
    }
}
