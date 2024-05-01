#ifndef TESTTREEWIDGETITEM_HPP
#define TESTTREEWIDGETITEM_HPP

#include "testcase.hpp"

#include <QTreeWidgetItem>

namespace gui
{
    class TestTreeWidgetItem : public QTreeWidgetItem
    {
    public:
        TestTreeWidgetItem(QTreeWidgetItem *parent, testsuite::TestCase* pTest = 0, int32_t depth = -1);

        void addItem(QTreeWidgetItem* parentItem, testsuite::TestCase* parentTest, int32_t depth);

        testsuite::TestCase* testCase();

        void update();

    private:
        testsuite::TestCase* m_pTestCase;

        void updateText();
    };
}

#endif // TESTTREEWIDGETITEM_HPP
