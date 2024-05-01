#ifndef SERIALPORTSDIALOG_HPP
#define SERIALPORTSDIALOG_HPP

#include <stdint.h>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QComboBox;
QT_END_NAMESPACE

namespace gui
{
    class SerialPortsDialog : public QDialog
    {
        Q_OBJECT

    public:
        SerialPortsDialog(QWidget* parent = 0);

    private slots:
        void accept();

    private:
        int32_t kWidth = 350;   //!< The width of the dialog

        QWidget* createPortSelector();
        QWidget* createButtons();
        QWidget* createStatusBar();

        QComboBox* m_pJigPortCombo;
        QComboBox* m_pSrcPortCombo;
        QComboBox* m_pCtsPortCombo;
        QPushButton* m_pApplyButton;
    };
}

#endif // SERIALPORTSDIALOG_HPP
