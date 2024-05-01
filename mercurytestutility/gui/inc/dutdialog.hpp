#ifndef DUTDIALOG_HPP
#define DUTDIALOG_HPP

#include "modaldialog.hpp"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

namespace dut
{
    class DeviceUnderTest;
}

namespace gui
{
    class DutDialog : public ModalDialog
    {
        Q_OBJECT
    public:
        DutDialog(dut::DeviceUnderTest* pDut, QWidget* parent = 0);
        virtual ~DutDialog();

    public slots:
        int exec();

    protected slots:
        virtual void accept();

    protected:
        dut::DeviceUnderTest* m_pDut;

    private:
        static const int32_t kSerialNrLen = 6;      //!< Serial number length (6-digits)
        static const int32_t kSerialNrMax = 999999; //!< Maximum serial number (6-digits)

        QLayout* createLayout();
        QLayout* createSerialLayout();
        QLayout* createVariantLayout();

        QLineEdit* m_pSerialEdit;
        QComboBox* m_pVariantCombo;

        void init();
    };
}

#endif // DUTDIALOG_HPP
