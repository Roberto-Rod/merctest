/****************************************************************************
**
**
****************************************************************************/

#include "mainwindow.hpp"
#include "settings.hpp"
#include "serialportsdialog.hpp"
#include "testequipmentdialog.hpp"
#include "testequipmentstore.hpp"
#include "signalgenerator.hpp"
#include "equipmentwidget.hpp"
#include "testwidget.hpp"
#include "progresswidget.hpp"
#include "logging.hpp"
#include "testjigfactory.hpp"
#include "mercurytestjig1.hpp"
#include "sourceboard.hpp"
#include "assyrevision.hpp"

#include <QtWidgets>
#include <QSerialPort>

using namespace common;

namespace gui
{
    const QString MainWindow::kAppId = "SW0061, Mercury Test Utility";

    MainWindow::MainWindow() :
        m_pJig(0)
    {
        // Set the window title
        QString title = tr("Mercury Test Utility");
        if (common::Settings::instance()->teDebugMode()) { title.append(tr(" (TE Debug Mode)")); }
        setWindowTitle(title);

        // Set the window icon
        setWindowIcon(QIcon(":/icons/32x32/kirintec_logo.png"));

        // Create all of the widgets
        setCentralWidget(createSplitter());

        // Create the menus in the menu bar
        createMenus();

        // Set the window settings
        setMinimumSize(640, 480);
        resize(640, 480);
        readSettings();

        // Connect the logging object's status message signal to the status bar
        QLabel* label = new QLabel();
        statusBar()->addPermanentWidget(label);

        connect(common::Logging::instance(), SIGNAL(statusMessage(QString)),
                                       label,   SLOT(setText(QString)));

        // Ensure log file is closed (will also update the status bar)
        common::Logging::instance()->closeLogFile();

        // Call refresh at startup
        refreshAll();
    }

    MainWindow::~MainWindow()
    {
    }

    QWidget* MainWindow::createSplitter()
    {
        // Create a horizontal splitter and add an equipment widget
        // and a test widget to it
        QSplitter* hSplitter = new QSplitter;
        hSplitter->addWidget(createEquipmentWidget());
        hSplitter->addWidget(createTestWidget());
        hSplitter->setChildrenCollapsible(false);

        // Create a vertical splitter and add the horizontal splitter
        // created above and a progress widget to it
        QSplitter* vSplitter = new QSplitter;
        vSplitter->setOrientation(Qt::Vertical);
        vSplitter->setChildrenCollapsible(false);
        vSplitter->addWidget(hSplitter);
        vSplitter->addWidget(createProgressWidget());

        return vSplitter;
    }

    QWidget* MainWindow::createEquipmentWidget()
    {
        m_pEquipmentWidget = new EquipmentWidget;

        // When a TE item is double-clicked, open the TE dialog and select the
        // tab index corresponding to the double-clicked item.
        connect(m_pEquipmentWidget, SIGNAL(teDoubleClicked(int)),
                this,               SLOT(openTeDialog(int)));

        connect(m_pEquipmentWidget, SIGNAL(refreshClicked()),
                this,               SLOT(refreshAll()));

        return m_pEquipmentWidget;
    }

    QWidget* MainWindow::createTestWidget()
    {
        m_pTestWidget = new TestWidget;

        // Connect the equipment widget's setTestSuite signal to the test widget's slot
        connect(m_pEquipmentWidget, SIGNAL(setTestSuite(testsuite::TestCase*)),
                 m_pTestWidget,      SLOT(setTestSuite(testsuite::TestCase*)));

        // When a test run has been completed, refresh the jig (check that it's still there)
        connect(m_pTestWidget, SIGNAL(aborted()),
                 this,          SLOT(refreshJig()));

        return m_pTestWidget;
    }

    QWidget* MainWindow::createProgressWidget()
    {
        m_pProgressWidget = new ProgressWidget;

        // Connect the logging object's progress message signal to the progress widget
        connect(common::Logging::instance(), SIGNAL(progressMessage(QString)),
                           m_pProgressWidget, SLOT(append(QString)));

        return m_pProgressWidget;
    }

    void MainWindow::createMenus()
    {
        QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(QIcon(":icons/16x16/folder-3.png"), tr("Set Reports Directory"), this, SLOT(setReportsDirectory()));
        // fileMenu->addAction(tr("Exit"), this, SLOT(close()), QKeySequence::Close);
        fileMenu->addAction(QIcon(":icons/16x16/folder-3.png"), tr("Set Programming Scripts Directory"), this, SLOT(setProgramScriptDirectory()));
        fileMenu->addAction(tr("Exit"), this, SLOT(close()), QKeySequence::Close);

        QMenu* equipmentMenu = menuBar()->addMenu(tr("&Equipment"));
        equipmentMenu->addAction(tr("Configure Serial Ports"),   this, SLOT(openSerialPortsDialog()));
        equipmentMenu->addAction(tr("Configure Test Equipment"), this, SLOT(openTeDialog()));
        equipmentMenu->addAction(QIcon(":icons/16x16/ksysguard.png"), tr("Calibrate Cables"), this, SLOT(calibrateCables()));
        equipmentMenu->addAction(tr("Sanity Check Calibration"), this, SLOT(sanityCheckCables()));
        QMenu *revisionsMenu = equipmentMenu->addMenu(tr("&Set Assembly / PCB Revisions"));
        setupRevisionsMenus(revisionsMenu);
        equipmentMenu->addAction(tr("Refresh All"), this, SLOT(refreshAll()));

        QMenu* testMenu = menuBar()->addMenu(tr("&Test"));
        testMenu->addAction(tr("Run (Automated)"),    m_pTestWidget, SLOT(runAuto()));
        testMenu->addAction(tr("Run (Manual Steps)"), m_pTestWidget, SLOT(runManual()));
        testMenu->addAction(tr("Run (Auto Smart Download)"), m_pTestWidget, SLOT(runSmartDownload()));

        QMenu* progressMenu = menuBar()->addMenu(tr("&Progress Messages"));
        progressMenu->addAction(QIcon(":icons/16x16/edit-copy-4.png"), tr("Copy All"),   m_pProgressWidget, SLOT(copyAll()));
        progressMenu->addAction(QIcon(":icons/16x16/edit-clear-2.png"), tr("Clear All"), m_pProgressWidget, SLOT(clear()));

        QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(tr("About"), this, SLOT(helpAbout()));
        helpMenu->addAction(QIcon(":icons/16x16/qt-logo.png"), tr("About Qt"), this, SLOT(helpAboutQt()));
    }

    void MainWindow::setReportsDirectory()
    {
        // Open a file dialog showing existing directories only, preset to the current directory
        // from the Settings
        common::Settings* settings = common::Settings::instance();
        QString dir = QFileDialog::getExistingDirectory(this, tr("Select Test Reports Directory"),
                                                         settings->reportsDirectory());

        // Write the selected directory to the settings
        if (!dir.isEmpty())
        {
            settings->setReportsDirectory(dir);
        }
    }

    void MainWindow::setupRevisionsMenus(QMenu* menu)
    {
        /// @TODO: The way this menu and the AssyRevision object is written should be refactored.
        /// There shouldn't need to be so much code here. The code here has to know what all the assemblies are
        /// and how many revisions there are in each. The AssyRevision object should do more of the work and
        /// the abstraction should be improved.
        /// AssyRevision gets construted with a string parameter and then compares that to preset
        /// strings to decide on an enumerated assembly/revision, that could be improved.
        /// ASY0020 and ASY0021 would probably be better treated individually rather than grouped up.
        common::AssyRevision::setRevisionDefaults();

        QMenu *pPcb0044 = menu->addMenu("PCB0044 (DDS Source Board)");
        QMenu *pAssy0033 = menu->addMenu("ASY0033 (Mid-Band Source Module)");
        QMenu *pAssy129 = menu->addMenu("ASY0129 (High-band Source Module)");
        QMenu *pPcb0049 = menu->addMenu("PCB0049 (Manpack PSU)");
        QMenu *pPcb0058 = menu->addMenu("PCB0058 (Vehicle PSU)");
        QMenu *pAssy20Assy21 = menu->addMenu("ASY0020/0021 (Low/Mid-Band Power Monitor)");
        QMenu *pAssy130 = menu->addMenu("ASY0130 (High-Band Power Monitor)");
        QMenu *pPcb0114 = menu->addMenu("PCB0114 (CTS Board)");

        QActionGroup *pcb0044Group = new QActionGroup(this);
        QActionGroup *assy0033Group = new QActionGroup(this);
        QActionGroup *assy129Group = new QActionGroup(this);
        QActionGroup *psuManpackGroup = new QActionGroup(this);
        QActionGroup *psuVehicleGroup = new QActionGroup(this);
        QActionGroup *assy20Assy21Group = new QActionGroup(this);
        QActionGroup *assy130Group = new QActionGroup(this);
        QActionGroup *pcb0114Group = new QActionGroup(this);

        // Source board revision menu setup
        QString revStr1 = AssyRevision::instance()->PCB0044StrList.at(0);
        QString revStr2 = AssyRevision::instance()->PCB0044StrList.at(1);
        QString revStr3 = AssyRevision::instance()->PCB0044StrList.at(2);
        QAction* pAction1 = setupMenuRevision(pPcb0044, AssyKey::PCB0044, static_cast<int>(AssyRevision::PCB0044Rev::REVBtoGIss2), revStr1);
        QAction* pAction2 = setupMenuRevision(pPcb0044, AssyKey::PCB0044, static_cast<int>(AssyRevision::PCB0044Rev::GIss2toIIss2), revStr2);
        QAction* pAction3 = setupMenuRevision(pPcb0044, AssyKey::PCB0044, static_cast<int>(AssyRevision::PCB0044Rev::RevJ), revStr3);
        // Ensure selection acts as a radio button
        pcb0044Group->addAction(pAction1);
        pcb0044Group->addAction(pAction2);
        pcb0044Group->addAction(pAction3);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction3, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // MB Daughter board revision menu setup
        revStr1 = AssyRevision::instance()->ASY0033StrList.at(0);
        revStr2 = AssyRevision::instance()->ASY0033StrList.at(1);
        revStr3 = AssyRevision::instance()->ASY0033StrList.at(2);
        pAction1 = setupMenuRevision(pAssy0033, AssyKey::ASY0033, static_cast<int>(AssyRevision::ASY0033Rev::REVBtoC), revStr1);
        pAction2 = setupMenuRevision(pAssy0033, AssyKey::ASY0033, static_cast<int>(AssyRevision::ASY0033Rev::REVCIss4), revStr2);
        pAction3 = setupMenuRevision(pAssy0033, AssyKey::ASY0033, static_cast<int>(AssyRevision::ASY0033Rev::REVDIss2), revStr3);
        // Ensure selection acts as a radio button
        assy0033Group->addAction(pAction1);
        assy0033Group->addAction(pAction2);
        assy0033Group->addAction(pAction3);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction3, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // HB Daughter board revision menu setup
        revStr1 = AssyRevision::instance()->ASY129StrList.at(0);
        revStr2 = AssyRevision::instance()->ASY129StrList.at(1);
        pAction1 = setupMenuRevision(pAssy129, AssyKey::ASY129, static_cast<int>(AssyRevision::ASY129Rev::RevB), revStr1);
        pAction2 = setupMenuRevision(pAssy129, AssyKey::ASY129, static_cast<int>(AssyRevision::ASY129Rev::REVCtoCIss2), revStr2);
        // Ensure selection acts as a radio button
        assy129Group->addAction(pAction1);
        assy129Group->addAction(pAction2);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // PSU PCB0049  (Manpack)
        revStr1 = AssyRevision::instance()->PCB0049StrList.at(0);
        revStr2 = AssyRevision::instance()->PCB0049StrList.at(1);
        pAction1 = setupMenuRevision(pPcb0049, AssyKey::PSUMan, static_cast<int>(AssyRevision::PCB0049Rev::RevBtoC), revStr1);
        pAction2 = setupMenuRevision(pPcb0049, AssyKey::PSUMan, static_cast<int>(AssyRevision::PCB0049Rev::RevCIss2), revStr2);
        // Ensure selection acts as a radio button
        psuManpackGroup->addAction(pAction1);
        psuManpackGroup->addAction(pAction2);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // PSU PCB0058  (Vehicle)
        revStr1 = AssyRevision::instance()->PCB0058StrList.at(0);
        revStr2 = AssyRevision::instance()->PCB0058StrList.at(1);
        pAction1 = setupMenuRevision(pPcb0058, AssyKey::PSUVeh, static_cast<int>(AssyRevision::PCB0058Rev::RevBtoFIss3), revStr1);
        pAction2 = setupMenuRevision(pPcb0058, AssyKey::PSUVeh, static_cast<int>(AssyRevision::PCB0058Rev::RevFIss4), revStr2);
        // Ensure selection acts as a radio button
        psuVehicleGroup->addAction(pAction1);
        psuVehicleGroup->addAction(pAction2);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // LB or MB Power monitor
        revStr1 = AssyRevision::instance()->ASY20or21StrList.at(0);
        revStr2 = AssyRevision::instance()->ASY20or21StrList.at(1);
        pAction1 = setupMenuRevision(pAssy20Assy21, AssyKey::ASY20or21, static_cast<int>(AssyRevision::ASY20or21Rev::RevBtoBIss3), revStr1);
        pAction2 = setupMenuRevision(pAssy20Assy21, AssyKey::ASY20or21, static_cast<int>(AssyRevision::ASY20or21Rev::REVC), revStr2);
        // Ensure selection acts as a radio button
        assy20Assy21Group->addAction(pAction1);
        assy20Assy21Group->addAction(pAction2);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // High Band Power Monitor
        revStr1 = AssyRevision::instance()->ASY130StrList.at(0);
        revStr2 = AssyRevision::instance()->ASY130StrList.at(1);
        pAction1 = setupMenuRevision(pAssy130, AssyKey::ASY130, static_cast<int>(AssyRevision::ASY130Rev::REVBtoC), revStr1);
        pAction2 = setupMenuRevision(pAssy130, AssyKey::ASY130, static_cast<int>(AssyRevision::ASY130Rev::REVDtoDIss2), revStr2);
        // Ensure selection acts as a radio button
        assy130Group->addAction(pAction1);
        assy130Group->addAction(pAction2);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
        connect(pAction2, SIGNAL(triggered()), this, SLOT(actionProcess()));

        // CTS Board
        revStr1 = AssyRevision::instance()->PCB0114StrList.at(0);
        pAction1 = setupMenuRevision(pPcb0114, AssyKey::PCB0114, static_cast<int>(AssyRevision::PCB0114Rev::RevBOn), revStr1);
        // Ensure selection acts as a radio button
        pcb0114Group->addAction(pAction1);
        // Ensure selected revision is written to the registry
        connect(pAction1, SIGNAL(triggered()), this, SLOT(actionProcess()));
    }

    QAction * MainWindow::setupMenuRevision(QMenu * assymenu, common::AssyKey assyKey, int revision, QString revString)
    {
        QAction * OutActionPtr;
        common::AssyRev ar;
        QVariant v1;
        ar.asy = assyKey;

        OutActionPtr = assymenu->addAction(revString);
        OutActionPtr->setCheckable(true);
        ar.rev = revision;

        if(common::Settings::instance()->compareRevision(assyKey, revision))
        {
            OutActionPtr->setChecked(true);
        }
        else
        {
            OutActionPtr->setChecked(false);
        }
        v1 = QVariant::fromValue(ar);
        OutActionPtr->setData(v1);
        return OutActionPtr;
    }

    void MainWindow::actionProcess()
    {
        common::AssyRev assyRev;
        QAction* actionX = qobject_cast<QAction*>(sender());
        QVariant variant;

        if(actionX)
        {
            variant = actionX->data();
            assyRev = variant.value<common::AssyRev>();
            common::Settings::instance()->setRevision(assyRev.asy, assyRev.rev);
        }
    }

   void MainWindow::setProgramScriptDirectory()
   {
       // Open a file dialog showing existing directories only, preset to the current directory
       // from the Settings
       common::Settings* settings = common::Settings::instance();
       QString dir = QFileDialog::getExistingDirectory(this, tr("Select Programming Scripts Directory"), settings->programmingScriptsDirectory());

       // Write the selected directory to the settings
       if (!dir.isEmpty())
       {
           settings-> setProgrammingScriptsDirectory(dir);
       }
   }

    void MainWindow::openSerialPortsDialog()
    {
        SerialPortsDialog dialog;

        dialog.exec();
    }

    void MainWindow::openTeDialog(int tab)
    {
        TestEquipmentDialog dialog(tab);

        dialog.exec();

        m_pEquipmentWidget->updateDetails();
    }

    void MainWindow::refreshAll()
    {
        m_pEquipmentWidget->updateDetails();

        refreshJig();
    }

    void MainWindow::refreshJig()
    {
        // If there is a jig object then check if it is still connected
        if ((m_pJig != 0) && !m_pJig->isConnected())
        {
            // Delete the test jig object
            delete m_pJig;
            m_pJig = 0;
        }

        // If there isn't a jig, then attempt to create a new one based on detected jig type
        if (m_pJig == 0)
        {
            m_pJig = testjig::TestJigFactory::instance()->makeTestJig(this);

            if (m_pJig != 0)
            {
                connect(m_pJig, SIGNAL(commsClosed()), m_pTestWidget, SLOT(commsClosed()));
            }
        }

        m_pEquipmentWidget->setTestJig(m_pJig);
    }

    void MainWindow::helpAbout()
    {
        QString message = QString("This software provides automated routines to test<BR>"
                                   "various Mercury PCB assemblies."
                                   "<P>Application ID: %1<BR>"
                                   "Application Version: %2<BR>"
                                   "Compiled using Qt: %3"
                                   "<P>For support contact: <a href =\"https://www.kirintec.com/contact/\">Kirintec</a>")
                                   .arg(kAppId)
                                   .arg(GIT_VERSION)
                                   .arg(QT_VERSION_STR);

        QMessageBox::about(this, "About Mercury Test Utility", message);
    }

    void MainWindow::helpAboutQt()
    {
        QMessageBox::aboutQt(this, "About Qt");
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        common::Settings::instance()->setValue("geometry", saveGeometry());
        common::Settings::instance()->setValue("windowState", saveState());
        QMainWindow::closeEvent(event);
    }

    void MainWindow::readSettings()
    {
        restoreGeometry(common::Settings::instance()->value("geometry").toByteArray());
        restoreState(common::Settings::instance()->value("windowState").toByteArray());
    }

    void MainWindow::calibrateCables()
    {
        testjig::TestJigCableCal* jig = dynamic_cast<testjig::TestJigCableCal*>(m_pJig);

        if (jig != 0)
        {
            // Calibrate the cables if we have the right type of jig
            common::Logging::instance()->log("Calibrating Test Jig RF Cables...",
                                              common::Logging::LOG_DEST_PROGRESS);

            jig->calibrateCables();

            // Update the details in the equipment widget
            m_pEquipmentWidget->updateDetails();
        }
        else
        {
            // Otherwise log a message to tell the user that we don't have the right type of jig
            common::Logging::instance()->log("ERROR: no test jig connected with cable calibration facilities",
                                              common::Logging::LOG_DEST_PROGRESS);
        }
    }

    void MainWindow::sanityCheckCables()
    {
        testjig::TestJigCableCal* jig = dynamic_cast<testjig::TestJigCableCal*>(m_pJig);

        if (jig != 0)
        {
            // Calibrate the cables if we have the right type of jig
            common::Logging::instance()->log("Sanity Check Test Jig RF Cables...",
                                              common::Logging::LOG_DEST_PROGRESS);

            jig->sanityCheckCables();

            // Update the details in the equipment widget
            m_pEquipmentWidget->updateDetails();
        }
        else
        {
            // Otherwise log a message to tell the user that we don't have the right type of jig
            common::Logging::instance()->log("ERROR: no test jig connected with cable calibration facilities",
                                              common::Logging::LOG_DEST_PROGRESS);
        }
    }

}
