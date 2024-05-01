#ifndef CABLECALDIALOG_HPP
#define CABLECALDIALOG_HPP

#include "modaldialog.hpp"

#include <QGroupBox>

QT_BEGIN_INCLUDE_NAMESPACE
class QLabel;
QT_END_INCLUDE_NAMESPACE

namespace gui
{
    struct CableCalDetails
    {
        CableCalDetails(QString name, QString calKey) :
            m_name(name), m_calKey(calKey)
        {
        }

        QString m_name;
        QString m_calKey;
    };

    class CableCalGroup : public QGroupBox
    {
        Q_OBJECT

    public:
        CableCalGroup(CableCalDetails* details, int index, QWidget* parent = 0);

    signals:
        void runCal(int index);

    public slots:
        void calOk();

    private slots:
        void calButtonPressed();

    private:
        CableCalDetails* m_details;
        QLabel* m_pLastCal;
        QLabel* m_pCalStatus;
        int m_index;

        QLayout* createLayout();

        void updateStatus();
    };

    class CableCalDialog : public ModalDialog
    {
        Q_OBJECT

    public:
        CableCalDialog(const QList<CableCalDetails*> details, QWidget* parent = 0);
        ~CableCalDialog() {}

        void init();

    signals:
        void runCal(int index);

    public slots:
        void calOk(int index);

    private:
        QList<CableCalDetails*> m_details;
        QList<CableCalGroup*> m_groupBoxes;

        QLayout* createLayout();
    };
}

#endif // CABLECALDIALOG_HPP
