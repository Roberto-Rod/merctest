#ifndef EQUIPMENTTREEWIDGET_HPP
#define EQUIPMENTTREEWIDGET_HPP

#include <QTreeWidget>

namespace testsuite
{
    class TestCase;
}

namespace testjig
{
    class TestJig;
}

namespace gui
{
    class EquipmentTreeWidget : public QTreeWidget
    {
        Q_OBJECT

    public:
        EquipmentTreeWidget(QWidget *parent = 0);
        ~EquipmentTreeWidget();

        void setTestJig(testjig::TestJig* pTestJig);

    signals:
        void teDoubleClicked(int index);
        void setTestSuite(testsuite::TestCase* testSuite);

    public slots:
        void updateDetails();

    private slots:
        void itemDoubleClicked(QTreeWidgetItem* item, int column);
        void itemClicked(QTreeWidgetItem* item, int column);
    };
}

#endif // EQUIPMENTTREEWIDGET_HPP
