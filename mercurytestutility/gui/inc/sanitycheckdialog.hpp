#ifndef SANITYCHECKDIALOG_HPP
#define SANITYCHECKDIALOG_HPP

#include "modaldialog.hpp"

#include <QGroupBox>

QT_BEGIN_INCLUDE_NAMESPACE
class QLabel;
QT_END_INCLUDE_NAMESPACE

namespace gui
{
    struct SanityCheckDetails
    {
        SanityCheckDetails(QString name, QString calKey) :
            m_name(name), m_calKey(calKey)
        {
        }

        QString m_name;
        QString m_calKey;
    };

    class  SanityCheckGroup : public QGroupBox
    {
        Q_OBJECT

    public:
         SanityCheckGroup(SanityCheckDetails* details, int index, QWidget* parent = 0);

    signals:
        void runCal(int index);

    public slots:
        void sanityOk();

    private slots:
        void calButtonPressed();

    private:
         SanityCheckDetails* m_details;
        QLabel* m_pLastCal;
        QLabel* m_pCalStatus;
        int m_index;

        QLayout* createLayout();

        void updateStatus();
    };

    class  SanityCheckDialog : public ModalDialog
    {
        Q_OBJECT

    public:
         SanityCheckDialog(const QList< SanityCheckDetails*> details, QWidget* parent = 0);
        ~ SanityCheckDialog() {}

        void init();

    signals:
        void runCal(int index);

    public slots:
        void sanityOk(int index);

    private:
        QList< SanityCheckDetails*> m_details;
        QList< SanityCheckGroup*> m_groupBoxes;

        QLayout* createLayout();
    };
}

#endif // SANITYCHECKDIALOG_HPP
