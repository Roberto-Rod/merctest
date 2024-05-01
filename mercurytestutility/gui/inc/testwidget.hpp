#ifndef TESTWIDGET_HPP
#define TESTWIDGET_HPP

#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLayout;
QT_END_NAMESPACE

namespace testsuite
{
    class TestCase;
}

namespace gui
{
    class TestTreeWidget;

    class TestWidget : public QGroupBox
    {
        Q_OBJECT

    public:
        TestWidget(QWidget* parent = 0);
        ~TestWidget();

    public slots:
        void setTestSuite(testsuite::TestCase* pTestSuite);
        void commsClosed();

    private slots:
        void runAuto();
        void runManual();
        void runSmartDownload();
        void setStopOnFail(bool sof);
        void contextMenu(const QPoint& pos);

    signals:
        void aborted();

    private:
        QLayout* createLayout();
        QLayout* createButtonLayout();
        QWidget* createTestTreeWidget();

        TestTreeWidget* m_pTestTreeWidget;

        bool m_running;
        bool m_aborted;
    };
}

#endif // TESTWIDGET_HPP
