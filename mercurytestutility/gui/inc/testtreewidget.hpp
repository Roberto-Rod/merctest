#ifndef TESTTREEWIDGET_HPP
#define TESTTREEWIDGET_HPP

#include <QTreeWidget>
#include <QList>

#include "testequipment.hpp"
#include "testcase.hpp"
#include "deviceundertest.hpp"

namespace gui
{
    class TestTreeWidget : public QTreeWidget
    {
        Q_OBJECT

    public:
        TestTreeWidget(QWidget *parent = 0);
        ~TestTreeWidget();

    public slots:
        void setTestSuite(testsuite::TestCase* pTestSuite);
        void update();
        void itemDoubleClicked(QTreeWidgetItem* item, int column);
        void runTest(dut::RunMethod  runMethod);
        void commsClosed();

    signals:
        void aborted();

    private:
        bool m_running;
        bool m_aborted;
    };
}

#endif // TESTTREEWIDGET_HPP
