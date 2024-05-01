#include "modaldialog.hpp"

#include <QKeyEvent>

namespace gui
{
    ModalDialog::ModalDialog(const QString& title, QWidget* parent) :
        QDialog(parent, Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint)
    {
        setWindowTitle(title);
        setWindowModality(Qt::ApplicationModal);
    }

    void ModalDialog::keyPressEvent(QKeyEvent *e)
    {
        // Pass the event on unless it is the ESC key, in which case ignore it
        if(e->key() != Qt::Key_Escape)
        {
            QDialog::keyPressEvent(e);
        }
    }
}
