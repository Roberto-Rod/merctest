#include "testtreewidgetitem.hpp"

#include <QApplication>

namespace gui
{
    TestTreeWidgetItem::TestTreeWidgetItem(QTreeWidgetItem *parent, testsuite::TestCase* pTest, int32_t depth) :
        QTreeWidgetItem(parent),
        m_pTestCase(pTest)
    {
        // Set the text
        updateText();

        // Call the recursive add item method
        addItem(parent, pTest, depth);
    }

    void TestTreeWidgetItem::addItem(QTreeWidgetItem* parentItem, testsuite::TestCase* parentTest, int32_t depth)
    {
        // Stop at depth = 0
        // Continue for negative depth so that a negative depth can be passed into to recurse through the whole tree
        if (depth != 0)
        {
            TestTreeWidgetItem* item;

            // If there is no parent then this is the top level item
            if (parentItem == 0)
            {
                item = this;
            }
            else
            {
                item = new TestTreeWidgetItem(parentItem, parentTest, 0);
            }

            QObjectList tests = parentTest->children();

            for (int i = 0; i < tests.length(); i++)
            {
                testsuite::TestCase* testCase = dynamic_cast<testsuite::TestCase*>(tests.at(i));

                if (testCase != 0)
                {
                    // Call the recursive add item method and decrement the depth
                    addItem(item, testCase, (depth - 1));
                }
            }
        }
    }

    testsuite::TestCase* TestTreeWidgetItem::testCase()
    {
        return m_pTestCase;
    }

    void TestTreeWidgetItem::updateText()
    {
        // Set the test name
        setText(0, m_pTestCase->name() + "   " + m_pTestCase->variant() + "  " + m_pTestCase->assyRevision());

        // Set the status icon
        QIcon icon;
        QString toolTip;
        if (m_pTestCase->status() == testsuite::TestStatus::NOT_TESTED)
        {
            icon.addFile(":/icons/16x16/dialog-question-2.png");
            toolTip = "Not Tested or Test Cancelled";
        }
        else if (m_pTestCase->status() == testsuite::TestStatus::FAILED)
        {
            icon.addFile("://icons/16x16/dialog-close.png");
            toolTip = "Test Failed";
        }
        else if (m_pTestCase->status() == testsuite::TestStatus::PASSED)
        {
            icon.addFile(":/icons/16x16/dialog-ok-3.png");
            toolTip = "Test Passed";
        }

        setIcon(1, icon);

        // Set the same tool tip in both columns
        setToolTip(0, toolTip);
        setToolTip(1, toolTip);
    }

    void TestTreeWidgetItem::update()
    {
        // Update text in this item
        updateText();

        // And then call this method on all children
        for (int i = 0; i < childCount(); i++)
        {
            TestTreeWidgetItem* childItem = dynamic_cast<TestTreeWidgetItem*>(child(i));

            if (childItem != 0)
            {
                childItem->update();
            }
        }
    }
}
