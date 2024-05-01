#include "testwidget.hpp"
#include "testtreewidget.hpp"
#include "testtreewidgetitem.hpp"
#include  "deviceundertest.hpp"
#include "logging.hpp"
#include "settings.hpp"
#include "messagedialog.hpp"

#include <QtWidgets>
#include <QApplication>

namespace gui
{
    TestWidget::TestWidget(QWidget* parent) :
        QGroupBox(tr("Tests"), parent),
        m_pTestTreeWidget(0),
        m_running(false),
        m_aborted(false)
    {
        setLayout(createLayout());
    }

    TestWidget::~TestWidget()
    {
    }

    void TestWidget::setTestSuite(testsuite::TestCase* pTestSuite)
    {
        m_pTestTreeWidget->setTestSuite(pTestSuite);

        if (pTestSuite != 0)
        {
            connect(pTestSuite, SIGNAL(statusUpdated()), m_pTestTreeWidget, SLOT(update()));
        }

        m_aborted = false;
    }

    QLayout* TestWidget::createLayout()
    {
        QVBoxLayout* pLayout = new QVBoxLayout;
        QWidget * pTestTreeWidget;
        pTestTreeWidget = createTestTreeWidget();

        // added to remove scroll bars
        pTestTreeWidget->setMinimumHeight(290);
        pLayout->addWidget(pTestTreeWidget);
        pLayout->addLayout(createButtonLayout());

        return pLayout;
    }

    QLayout* TestWidget::createButtonLayout()
    {
        // Create the Stop on Fail checkbox and get the current state from the settings
        QCheckBox* pCheckBox = new QCheckBox();
        pCheckBox->setChecked(common::Settings::instance()->stopOnFail());
        connect(pCheckBox,         SIGNAL(toggled(bool)), SLOT(setStopOnFail(bool)));

        QHBoxLayout* pLayout = new QHBoxLayout;
        pLayout->addStretch();
        pLayout->addWidget(new QLabel("Stop on Fail"));
        pLayout->addWidget(pCheckBox);

        return pLayout;
    }

    QWidget* TestWidget::createTestTreeWidget()
    {
        m_pTestTreeWidget = new TestTreeWidget;

        m_pTestTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(m_pTestTreeWidget, SIGNAL(customContextMenuRequested(QPoint)),
                this,              SLOT(contextMenu(QPoint)));

        connect(m_pTestTreeWidget, SIGNAL(aborted()),
                this,              SIGNAL(aborted()));

        return m_pTestTreeWidget;
    }

    void TestWidget::runAuto()
    {
        m_pTestTreeWidget->runTest(dut::RunMethod::AUTOMATIC_RUN);
    }

    void TestWidget::runManual()
    {
        m_pTestTreeWidget->runTest(dut::RunMethod::MANUAL_RUN);
    }

    void TestWidget::runSmartDownload()
    {
         m_pTestTreeWidget->runTest(dut::RunMethod::AUTOMATIC_SMART);
    }

    void TestWidget::setStopOnFail(bool sof)
    {
        common::Settings::instance()->setStopOnFail(sof);
    }

    void TestWidget::contextMenu(const QPoint& pos)
    {
        // If the context menu is requested over a tree widget item then display one
        TestTreeWidgetItem* item = dynamic_cast<TestTreeWidgetItem*>(m_pTestTreeWidget->itemAt(pos));

        if (item)
        {
            QPoint globalPos = m_pTestTreeWidget->mapToGlobal(pos);

            QMenu context;

            context.addAction("Run (Automated)",    this, SLOT(runAuto()));
            context.addAction("Run (Manual Steps)", this, SLOT(runManual()));
            context.addSeparator();
            context.addAction("Reset Test Status",  item->testCase(), SLOT(resetStatus()));

            context.exec(globalPos);
        }
    }

    void TestWidget::commsClosed()
    {
        m_pTestTreeWidget->commsClosed();
    }
}
