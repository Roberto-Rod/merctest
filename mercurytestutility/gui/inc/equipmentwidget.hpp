#ifndef EQUIPMENTWIDGET_HPP
#define EQUIPMENTWIDGET_HPP

#include "equipmenttreewidget.hpp"

#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLayout;
QT_END_NAMESPACE

namespace testjig
{
    class TestJig;
    class TestCase;
}

namespace gui
{
    class EquipmentWidget : public QGroupBox
    {
        Q_OBJECT

    public:
        EquipmentWidget(QWidget* parent = 0);
        ~EquipmentWidget();

        void setTestJig(testjig::TestJig* pTestJig);

    signals:
        void refreshClicked();
        void teDoubleClicked(int index);
        void setTestSuite(testsuite::TestCase* testSuite);

    public slots:
        void updateDetails();

    private:
        QLayout* createLayout();
        QLayout* createButtonLayout();
        QWidget* createEquipmentTreeWidget();
        void hookUpSignals();

        QPushButton*         m_pRefreshButton;
        EquipmentTreeWidget* m_pEquipmentTreeWidget;
    };
}

#endif // EQUIPMENTWIDGET_HPP
