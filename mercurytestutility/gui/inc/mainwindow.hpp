#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QList>

#include "registercomms.hpp"
#include "serialcomms.hpp"
#include "assyrevision.hpp"

QT_BEGIN_NAMESPACE
class QPushButton;
class QGroupBox;
class QGridLayout;
class QWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QTextEdit;
QT_END_NAMESPACE

namespace testjig
{
    class TestJig;
}

namespace gui
{
    // Forward declaration of the custom widgets
    class EquipmentWidget;
    class TestWidget;
    class ProgressWidget;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    private slots:
        void setReportsDirectory();
        void setProgramScriptDirectory();
        void refreshAll();
        void refreshJig();
        void openSerialPortsDialog();
        void openTeDialog(int tab = 0);
        void calibrateCables();
        void sanityCheckCables();
        void helpAbout();
        void helpAboutQt();
        void actionProcess();

    private:
        static const QString kAppId;
        QWidget* createSplitter();
        QWidget* createEquipmentWidget();
        QWidget* createTestWidget();
        QWidget* createProgressWidget();
        void setupRevisionsMenus(QMenu* fileMenu);
        void createMenus();
        void createStatusBar();
        void readSettings();
        void closeEvent(QCloseEvent* event);
        QAction * setupMenuRevision(QMenu * assymenu, common::AssyKey assyKey, int revision, QString revString);

        // The following objects create the three window panes of the test uitility:
        // 'Equipment' (EqupmentWidget), 'Tests' (TestWiget) and 'Progress Messages' (ProgressWidget)
        // The TestWidget comprises of  a TestTreeWidget which in turn comprises
        // one or more TestTreeWidgetItems.
        // The EquipmentWidget comprises of an EquipmentTreeWidget which in turn
        // comprises of one or more TeTreeWidgetItems.
        // The EqupmentWidget also comprises of a TestJigTreeWidgetItem which inturn
        // comprises of one or more DutTreeWidgetItems.

        EquipmentWidget*  m_pEquipmentWidget;
        TestWidget*       m_pTestWidget;
        ProgressWidget*   m_pProgressWidget;
        testjig::TestJig* m_pJig;
    };
}
#endif // MAINWINDOW_HPP
