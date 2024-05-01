#include "testtreewidget.hpp"
#include "testtreewidgetitem.hpp"
#include "deviceundertest.hpp"
#include "duttreewidgetitem.hpp"

#include <QTreeWidgetItem>
#include <QApplication>

namespace gui
{
    TestTreeWidget::TestTreeWidget(QWidget *parent) :
        QTreeWidget(parent),
        m_running(false),
        m_aborted(false)
    {
        setColumnCount(2);
        setColumnWidth(0, 420);
        setHeaderHidden(true);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setExpandsOnDoubleClick(false);

        connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
                this,   SLOT(itemDoubleClicked(QTreeWidgetItem*, int)));
    }

    TestTreeWidget::~TestTreeWidget()
    {
    }

    void TestTreeWidget::setTestSuite(testsuite::TestCase* pTestSuite)
    {
        // Delete current top level item, if it exists
        if (topLevelItem(0))
        {
            delete topLevelItem(0);
        }

        // Was a non-null pointer passed in?
        if (pTestSuite)
        {
            // Create the new top level item
            TestTreeWidgetItem* item = new TestTreeWidgetItem(nullptr, pTestSuite);
            addTopLevelItem(item);

            // Expand it
            item->setExpanded(true);

            // And set it to selected
            item->setSelected(true);
        }

        m_aborted = false;
    }

    void TestTreeWidget::update()
    {
        TestTreeWidgetItem* item = dynamic_cast<TestTreeWidgetItem*>(topLevelItem(0));

        if (item != 0)
        {
            item->update();
        }
    }

    void TestTreeWidget::itemDoubleClicked(QTreeWidgetItem* item, int column)
    {
        // Not interested in column or item, but we've received them
        // so just suppress a compiler warning
        (void)column;
        (void)item;

        runTest(dut::RunMethod::AUTOMATIC_RUN);
    }

    //void TestTreeWidget::runTest(bool manualSteps)
    void TestTreeWidget::runTest(dut::RunMethod  runMethod)
    {
        m_running = true;

        QList<QTreeWidgetItem*> items = selectedItems();

        // Check that the list is populated - the tree is set to only allow one selection
        // at a time so we don't need to worry about multiple selections
        if (items.length() > 0)
        {
            TestTreeWidgetItem* item = dynamic_cast<TestTreeWidgetItem*>(items.at(0));

            if (item != 0)
            {
                m_running = true;

                bool cont = true;

                // If the tree widget item has no parent then it is a top-level DUT.
                // if it's not a top-level DUT then warn the user that they are executing a sub-test.
                if (item->parent() != 0)
                {
                    QString msg = QString("Warning: the selected item is not a top-level DUT (\"Device Under Test\").<br><br>"
                                           "A valid test results file can only be created by running the full suite of "
                                           "tests for each DUT.<br><br>"
                                           "You may continue for hardware debugging purposes but the test results will "
                                           "<b>not be logged</b> to file.");
                    MessageDialog dialog(msg, MessageDialogType::INFO, this);

                    // Execute the dialog and get its return code - true means carry on with test (OK was pressed)
                    // false means don't carry on with test (Cancel was pressed)
                    cont = dialog.exec();
                }

                // Run test(s) if cont is still true
                if (cont)
                {
                    // Show a modal message box whilst tests are running
                    MessageDialog msg(QString("Running %1 Tests").arg(item->testCase()->name()),
                                       MessageDialogType::INFO, false, false, this);

                    msg.setWindowModality(Qt::WindowModal);
                    msg.show();
                    QApplication::processEvents();
                    bool manualSteps = false;
                    if  (runMethod == dut::RunMethod::MANUAL_RUN)
                    {
                        manualSteps = true;
                    }

                   item->testCase()->setRunMode(runMethod);
                    (void)item->testCase()->execute(manualSteps);
                }

                m_running = false;

                if (m_aborted)
                {
                    emit aborted();
                }
            }
        }
    }

    void TestTreeWidget::commsClosed()
    {
        m_aborted = true;

        if (!m_running)
        {
            emit aborted();
        }
    }
}
