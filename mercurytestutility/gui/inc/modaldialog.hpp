#ifndef MODALDIALOG_HPP
#define MODALDIALOG_HPP

#include <QDialog>

namespace gui
{
    class ModalDialog : public QDialog
    {
        Q_OBJECT

    public:
        ModalDialog(const QString &title, QWidget *parent = 0);

        virtual ~ModalDialog() {}

    protected:
        static const int kSpaceBeforeButtons = 15;      //!< Spacing to use above the dialog buttons

    private slots:
        void keyPressEvent(QKeyEvent *e);
    };
}
#endif // MODALDIALOG_HPP
