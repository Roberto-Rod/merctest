#include "mercurytestjig1.hpp"
#include "testjigdetails.hpp"
#include "rfswitchmatrix.hpp"
#include "logging.hpp"
#include "messagedialog.hpp"
#include "directserialcomms.hpp"
#include "registercomms.hpp"
#include "settings.hpp"
#include "sourceboard.hpp"
#include "daughtermodule.hpp"
#include "testharnesssourceboard.hpp"
#include "powermonitor.hpp"
#include "dockcontroller.hpp"
#include "psumodule.hpp"
#include "dutpowercontrol.hpp"
#include "membrane.hpp"
#include "ctsboard.hpp"
#include "griddialog.hpp"
#include "cablecaldialog.hpp"
#include "sanitycheckdialog.hpp"
#include "sanitycheckcal.hpp"

#include <QDebug>

namespace testjig
{
    const QString MercuryTestJig1::kJigName = QString("Mercury Test Jig 1");  //!< This test jig's name
    const QString MercuryTestJig1::kSourceAssyCableCalKey = QString("mercurytestjig1_sourceassy");
    const QString MercuryTestJig1::kPwrMonCableCalKey = QString("mercurytestjig1_pwrmon");
    const QString MercuryTestJig1::kHBCableCalKey = QString("mercurytestjig1_hb");

    MercuryTestJig1::MercuryTestJig1(QObject* parent) :
        TestJig(parent),
        m_pComms(new comms::DirectSerialComms(this))
    {
        // Open the serial port - it will get closed automatically when the object is deleted
        m_pComms->openPort(common::Settings::instance()->jigPortName());

        // Create the register comms object
        m_pRegComms = new comms::RegisterComms(m_pComms, this);

        // Enable the test jig 5V5 rail
        testjig::DutPowerControl pwr(m_pRegComms);
        pwr.enablePower(testjig::PowerSupply::TESTJIG_5V5, true);

        // Create the DUT objects
        m_duts.append(new dut::SourceBoard(this));
        m_duts.append(new dut::DaughterModule(this));
        m_duts.append(new dut::DockController(this));
        m_duts.append(new dut::PsuModule(this));
        m_duts.append(new dut::PowerMonitor(this));
        m_duts.append(new dut::Membrane(this));
        m_duts.append(new dut::CtsBoard(this));

        // Connect the comms closed signal
        QObject::connect(m_pComms, SIGNAL(commsClosed()), this, SIGNAL(commsClosed()));

        // Connect the status updated signals
        for (int i = 0; i < m_duts.length(); i++)
        {
            QObject::connect(m_duts.at(i)->topLevelTestCase(), SIGNAL(statusUpdated()), this, SIGNAL(statusUpdated()));
        }
    }

    MercuryTestJig1::~MercuryTestJig1()
    {
        // Disable the test jig 5V5 rail
        testjig::DutPowerControl pwr(m_pRegComms);
        pwr.enablePower(testjig::PowerSupply::TESTJIG_5V5, false);
    }

    QString MercuryTestJig1::name() const
    {
        return QString(kJigName);
    }

    bool MercuryTestJig1::connect()
    {
        bool retVal = false;

        // Disconnect current session
        disconnect();

        // Can we communicate with the jig?
        if (m_pComms->openPort(common::Settings::instance()->jigPortName()))
        {
            // Check the expected test jig is connected
            retVal = isConnected();
        }

        // If something went wrong then close port
        if (!retVal)
        {
           m_pComms->closePort();
        }

        return retVal;
    }

    void MercuryTestJig1::disconnect()
    {
        m_pComms->closePort();
    }

    bool MercuryTestJig1::isConnected() const
    {
        bool retVal = false;

        // Is jig port open?
        if (m_pComms->isPortOpen())
        {
            // Create a details instance to get the jig type
            TestJigDetails details(m_pRegComms);

            // Check that the type is a Mercury Test Jig 1
            if (details.connectedJigType() == JigType::MERCURY_JIG_1)
            {
                retVal = true;
            }
        }

        return retVal;
    }

    void MercuryTestJig1::calibrateCables()
    {
        QList<gui::CableCalDetails*> details;

        details.append(new gui::CableCalDetails("Source Module Cables", kSourceAssyCableCalKey));
        details.append(new gui::CableCalDetails("Power Monitor Cables", kPwrMonCableCalKey));
        details.append(new gui::CableCalDetails("High Band Module Cables", kHBCableCalKey));

        gui::CableCalDialog dialog(details);

        QObject::connect(&dialog, SIGNAL(runCal(int)), this, SLOT(runCableCal(int)));
        QObject::connect(this, SIGNAL(cableCalOk(int)), &dialog, SLOT(calOk(int)));

        dialog.exec();

        delete details.at(0);
        delete details.at(1);
        delete details.at(2);
    }

    void MercuryTestJig1::sanityCheckCables()
    {
        QList<gui::SanityCheckDetails*> details;

        details.append(new gui::SanityCheckDetails("[Source Board] Sig Gen Cable + CAB0255 + TestRig + Spectrum Analyser Cable", kPwrMonCableCalKey));
        details.append(new gui::SanityCheckDetails("[Power Monitor] Sig Gen Cable + Power Meter Cable", kSourceAssyCableCalKey));
        details.append(new gui::SanityCheckDetails("[High Band]  Cab255 + Power Meter Cable,  Cab255 + Sig Gen Cable ",  kHBCableCalKey));

        gui::SanityCheckDialog dialog(details);

        QObject::connect(&dialog, SIGNAL(runCal(int)), this, SLOT(runSanityCheckl(int)));
        QObject::connect(this, SIGNAL(sanityCheckOk(int)), &dialog, SLOT(sanityOk(int)));

        dialog.exec();

        delete details.at(0);
        delete details.at(1);
        delete details.at(2);
    }

    void MercuryTestJig1::runCableCal(int index)
    {
        if (index == 0)
        {
            calibrateSourceModuleCables();
        }
        else if (index == 1)
        {
            calibratePowerMonitorCables();
        }
        else if(index == 2)
        {
            calibrateHighBandCables();
        }
    }

    void MercuryTestJig1::runSanityCheckl(int index)
    {
        if (index == 0)
        {
            sanityCheckTestJigSpecAnCable();
        }
        else if (index == 1)
        {
            sanityCheckPowerMeterCable();
        }
        else if (index == 2)
        {
            sanityCheckHighBandCable();
        }
    }

    void MercuryTestJig1::calibrateSourceModuleCables()
    {
        bool ok = false;
        bool cancelled = false;

        // Is jig port open?
        if (m_pComms->isPortOpen())
        {
            ok = true;

            QVector<CableCal*> cableCals;
            QVector<RfPathSourceAssy*> calPaths;

            // Create the cal paths - note that the order is such that the user only changes
            // one cable between each cal. Also note that "short DUT RF cables" are no longer used
            calPaths.append(new RfPathSourceAssy());
            calPaths.append(new RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::RHS, RfPathSourceAssy::TePort::POWER_METER));
            calPaths.append(new RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::RHS, RfPathSourceAssy::TePort::SPECTRUM_ANALYSER));
            calPaths.append(new RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::LHS, RfPathSourceAssy::TePort::SPECTRUM_ANALYSER));
            calPaths.append(new RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::LHS, RfPathSourceAssy::TePort::POWER_METER));

            // Create an RF switch matrix object to switch the paths
            RfSwitchMatrix rfSwitch(m_pRegComms);

            // Throw up a modal dialog to inform user of overall status
            gui::GridDialog gridDialog("RF Cable Calibration : Source Assemblies", calPaths.length(), 2);

            for (int row = 0; row < calPaths.length(); row++)
            {
                gridDialog.setItem(calPaths.at(row)->brief() + "...", row, 0);
            }

            // Show the dialog
            gridDialog.show();

            // Run all of the cable cals
            for (int i = 0; (i < calPaths.length()) && !cancelled; i++)
            {
                // get pointer to this path
                RfPathSourceAssy* path = calPaths.at(i);

                // Get pointer to previous path, leave it as null if this is first or second path
                RfPathSourceAssy* prevPath = 0;
                if (i > 1) { prevPath = calPaths.at(i - 1); }

                // Create new cable cal object and set test power according to path
                CableCal* pCableCal = new CableCal(path->key());
                pCableCal->setTestPowerLevel(path->testPower());
                cableCals.append(pCableCal);

                // Set the ports now so that the user can see which should be connected via the jig LEDs
                if (i > 0)
                {
                    ok &= rfSwitch.setDutRfPort(path->dutPort());
                    ok &= rfSwitch.setTestEquipment(path->tePort());
                }

                // Create a Message Dialog
                gui::MessageDialog msgDialog(path->calInstruction(prevPath), gui::MessageDialogType::INFO, this);

                // Did user press OK?
                if (msgDialog.exec())
                {
                    // Is this the "null" correction path?
                    if (i == 0)
                    {
                        cableCals.at(0)->calibrate(cancelled);
                    }
                    else
                    {
                        // Correct each path with the "null path" cal points
                        cableCals.at(i)->calibrate(cancelled, cableCals.at(0)->calPoints());
                    }
                }
                else
                {
                    cancelled = true;
                }

                if (!cancelled)
                {
                    gridDialog.setItem("Done", i, 1);
                }
            }

            // If it all worked without being cancelled then store all of the cable cals
            // (index 0 is the null path - we don't need to store that)
            if (ok && !cancelled)
            {
                for (int i = 1; i < cableCals.length(); i++)
                {
                    cableCals.at(i)->storeCal();
                }

                // Set new cal expiry date
                common::Calibration calSourceAssy(kSourceAssyCableCalKey);
                calSourceAssy.setCalExpiry(QDate::currentDate().addDays(CableCal::kCalValidDays));
            }

            // Delete all of the path and cable cal objects
            for (int i = 0; i < calPaths.length(); i++)
            {
                delete calPaths.at(i);
            }

            for (int i = 0; i < cableCals.length(); i++)
            {
                delete cableCals.at(i);
            }
        }

        if (ok && !cancelled)
        {
            emit cableCalOk(0u);
        }
    }

    void MercuryTestJig1::calibratePowerMonitorCables()
    {
        bool ok = false;
        bool cancelled = false;

        // Is jig port open?
        if (m_pComms->isPortOpen())
        {
            ok = true;

            QVector<CableCal*> cableCals;
            QVector<RfPathPowerMonitor*> calPaths;

            // Create the cal paths - note that the order is such that the user only changes
            // one cable between each cal
            //calPaths.append(new RfPathPowerMonitor());
            calPaths.append(new RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM));
            calPaths.append(new RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet::SIG_GEN_AND_POWER_METER));

            // Throw up a modal dialog to inform user of overall status
            gui::GridDialog gridDialog("RF Cable Calibration : Power Monitor", calPaths.length(), 2);

            for (int row = 0; row < calPaths.length(); row++)
            {
                gridDialog.setItem(calPaths.at(row)->brief() + "...", row, 0);
            }

            // Show the dialog
            gridDialog.show();

            // Run all of the cable cals
            for (int i = 0; (i < calPaths.length()) && !cancelled; i++)
            {
                RfPathPowerMonitor* path = calPaths.at(i);

                // Create new cable cal object and set test power according to path
                CableCal* pCableCal = new CableCal(path->key());
                pCableCal->setTestPowerLevel(path->testPower());
                cableCals.append(pCableCal);

                // Create a Message Dialog
                gui::MessageDialog msgDialog(path->calInstruction(), gui::MessageDialogType::INFO, this);

                // Did user press OK?
                if (msgDialog.exec())
                {
                    cableCals.at(i)->calibrate(cancelled);
                }
                else
                {
                    cancelled = true;
                }

                if (!cancelled)
                {
                    gridDialog.setItem("Done", i, 1);
                }
            }

            // If it all worked without being cancelled then store all of the cable cals
            if (ok && !cancelled)
            {
                for (int i = 0; i < cableCals.length(); i++)
                {
                    cableCals.at(i)->storeCal();
                }

                // Set new cal expiry date
                common::Calibration calPwrMon(kPwrMonCableCalKey);
                calPwrMon.setCalExpiry(QDate::currentDate().addDays(CableCal::kCalValidDays));
            }

            // Delete all of the path and cable cal objects
            for (int i = 0; i < calPaths.length(); i++)
            {
                delete calPaths.at(i);
            }

            for (int i = 0; i < cableCals.length(); i++)
            {
                delete cableCals.at(i);
            }
        }

        if (ok && !cancelled)
        {
            emit cableCalOk(1u);
        }
    }

    void MercuryTestJig1::calibrateHighBandCables()
    {
        bool ok = false;
        bool cancelled = false;

        // Is jig port open?
        if (m_pComms->isPortOpen())
        {
            ok = true;

            QVector<CableCal*> cableCals;
            QVector<RfPathHighBandModule*> calPaths;

            // Create the cal paths - note that the order is such that the user only changes
            // one cable between each cal
            //calPaths.append(new RfPathPowerMonitor());
            calPaths.append(new RfPathHighBandModule(RfPathHighBandModule::RfCableSet::SIG_GEN_AND_CAB0255));
            calPaths.append(new RfPathHighBandModule(RfPathHighBandModule::RfCableSet::POWER_METER_AND_CAB0255));
            calPaths.append(new RfPathHighBandModule(RfPathHighBandModule::RfCableSet::SPEC_AN_AND_CAB0255));

            // Throw up a modal dialog to inform user of overall status
            gui::GridDialog gridDialog("RF Cable Calibration : Power Monitor", calPaths.length(), 2);

            for (int row = 0; row < calPaths.length(); row++)
            {
                gridDialog.setItem(calPaths.at(row)->brief() + "...", row, 0);
            }

            // Show the dialog
            gridDialog.show();

            // Run all of the cable cals
            for (int i = 0; (i < calPaths.length()) && !cancelled; i++)
            {
                RfPathHighBandModule* path = calPaths.at(i);

                // Create new cable cal object and set test power according to path
                CableCal* pCableCal = new CableCal(path->key());
                pCableCal->setTestPowerLevel(path->testPower());
                cableCals.append(pCableCal);

                // Create a Message Dialog
                gui::MessageDialog msgDialog(path->calInstruction(), gui::MessageDialogType::INFO, this);

                // Did user press OK?
                if (msgDialog.exec())
                {
                    cableCals.at(i)->calibrate(cancelled);
                }
                else
                {
                    cancelled = true;
                }

                if (!cancelled)
                {
                    gridDialog.setItem("Done", i, 1);
                }
            }

            // If it all worked without being cancelled then store all of the cable cals
            if (ok && !cancelled)
            {
                for (int i = 0; i < cableCals.length(); i++)
                {
                    cableCals.at(i)->storeCal();
                }

                // Set new cal expiry date
                common::Calibration calHB(kHBCableCalKey);
                calHB.setCalExpiry(QDate::currentDate().addDays(CableCal::kCalValidDays));
            }

            // Delete all of the path and cable cal objects
            for (int i = 0; i < calPaths.length(); i++)
            {
                delete calPaths.at(i);
            }

            for (int i = 0; i < cableCals.length(); i++)
            {
                delete cableCals.at(i);
            }
        }

        if (ok && !cancelled)
        {
            emit cableCalOk(2u);
        }
    }

    void MercuryTestJig1::sanityCheckPowerMeterCable()
    {
        bool ok = false;
        bool cancelled = false;

        // Is jig port open?
        if (m_pComms->isPortOpen())
        {
            ok = true;

            QVector<SanityCheckCal*> sanityCheckCals;
            QVector<RfPathPowerMonitor*> calPaths;

            // Create the cal paths - note that the order is such that the user only changes
            // one cable between each cal
            //calPaths.append(new RfPathPowerMonitor());
           // calPaths.append(new RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM));
            calPaths.append(new RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet::SIG_GEN_AND_POWER_METER));

            /*
            // Throw up a modal dialog to inform user of overall status
            gui::GridDialog gridDialog("RF Cable Calibration : Power Monitor", calPaths.length(), 2);

            for (int row = 0; row < calPaths.length(); row++)
            {
                gridDialog.setItem(calPaths.at(row)->brief() + "...", row, 0);
            }

            // Show the dialog
            gridDialog.show();

            */

            // Run all of the cable cals
            for (int i = 0; (i < calPaths.length()) && !cancelled; i++)
            {
                RfPathPowerMonitor* path = calPaths.at(i);

                // Create new cable cal object and set test power according to path
                SanityCheckCal* pCableCal = new  SanityCheckCal(path->key());
                pCableCal->setTestPowerLevel(path->testPower());
                sanityCheckCals.append(pCableCal);

                // Create a Message Dialog
                gui::MessageDialog msgDialog(pCableCal->measurePowerMonitorInstruction(), gui::MessageDialogType::INFO, this);

                // Did user press OK?
                if (msgDialog.exec())
                {
                    sanityCheckCals.at(i)->testRfPower(testjig::SanityCheckType::POWERMONITER, cancelled);
                }
                else
                {
                    cancelled = true;
                }

                /*  if (!cancelled)
                {
                    gridDialog.setItem("Done", i, 1);
                }
                */
            }

            // If it all worked without being cancelled then store all of the cable cals
            if (ok && !cancelled)
            {
            }

            // Delete all of the path and cable cal objects
            for (int i = 0; i < calPaths.length(); i++)
            {
                delete calPaths.at(i);
            }

            for (int i = 0; i < sanityCheckCals.length(); i++)
            {
                delete sanityCheckCals.at(i);
            }
        }

        if (ok && !cancelled)
        {
            emit sanityCheckOk(1u);
        }
    }

    void MercuryTestJig1::sanityCheckHighBandCable()
    {
        bool ok = false;
        bool cancelled = false;

        // Is jig port open?
        if (m_pComms->isPortOpen())
        {
            ok = true;

            QVector<SanityCheckCal*> sanityCheckCals;
            QVector< RfPathHighBandModule*> calPaths;

            // Create the cal paths - note that the order is such that the user only changes
            // one cable between each cal
            //calPaths.append(new RfPathPowerMonitor());
           // calPaths.append(new RfPathPowerMonitor(RfPathPowerMonitor::RfCableSet::SIG_GEN_AT_15_DBM));
            calPaths.append(new  RfPathHighBandModule(RfPathHighBandModule::RfCableSet::POWER_METER_AND_CAB0255));
            calPaths.append(new  RfPathHighBandModule(RfPathHighBandModule::RfCableSet::SIG_GEN_AND_CAB0255));
            /*
            // Throw up a modal dialog to inform user of overall status
            gui::GridDialog gridDialog("RF Cable Calibration : Power Monitor", calPaths.length(), 2);

            for (int row = 0; row < calPaths.length(); row++)
            {
                gridDialog.setItem(calPaths.at(row)->brief() + "...", row, 0);
            }

            // Show the dialog
            gridDialog.show();

            */

            // Run all of the cable cals
            for (int i = 0; (i < calPaths.length()) && !cancelled; i++)
            {
                RfPathHighBandModule* path = calPaths.at(i);

                // Create new cable cal object and set test power according to path
                SanityCheckCal* pCableCal = new  SanityCheckCal(path->key());
                pCableCal->setTestPowerLevel(path->testPower());
                sanityCheckCals.append(pCableCal);

                // Create a Message Dialog
                gui::MessageDialog msgDialog(pCableCal->measureInstruction(path->cableSet()), gui::MessageDialogType::INFO, this);

                // Did user press OK?
                if (msgDialog.exec())
                {
                    int sanityCheckType = (path->cableSet() == RfPathHighBandModule::RfCableSet::POWER_METER_AND_CAB0255) ? testjig::SanityCheckType::HIGHBAND_POWERMETER : testjig::SanityCheckType::HIGHBAND_SIGGEN;
                    sanityCheckCals.at(i)->testRfPower(static_cast<testjig::SanityCheckType>(sanityCheckType), cancelled, path->cableSet());
                }
                else
                {
                    cancelled = true;
                }

                /*  if (!cancelled)
                {
                    gridDialog.setItem("Done", i, 1);
                }
                */
            }

            // If it all worked without being cancelled then store all of the cable cals
            if (ok && !cancelled)
            {
            }

            // Delete all of the path and cable cal objects
            for (int i = 0; i < calPaths.length(); i++)
            {
                delete calPaths.at(i);
            }

            for (int i = 0; i < sanityCheckCals.length(); i++)
            {
                delete sanityCheckCals.at(i);
            }
        }

        if (ok && !cancelled)
        {
             emit sanityCheckOk(2u);
        }
    }

    void MercuryTestJig1::sanityCheckTestJigSpecAnCable()
    {
            bool ok = false;
            bool cancelled = false;

            // Is jig port open?
            if (m_pComms->isPortOpen())
            {
                ok = true;

                //QVector<CableCal*> cableCals;
                QVector<SanityCheckCal*> sanityCheckCals;
                QVector<RfPathSourceAssy*> calPaths;

                // Create the cal paths - note that the order is such that the user only changes
                // one cable between each cal. Also note that "short DUT RF cables" are no longer used

                calPaths.append(new RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::LHS, RfPathSourceAssy::TePort::SPECTRUM_ANALYSER));
                // calPaths.append(new RfPathSourceAssy(RfPathSourceAssy::DutRfCable::LONG,  RfPathSourceAssy::DutRfPort::LHS, RfPathSourceAssy::TePort::POWER_METER));

                // Create an RF switch matrix object to switch the paths
                RfSwitchMatrix rfSwitch(m_pRegComms);

                // Throw up a modal dialog to inform user of overall status
               /* gui::GridDialog gridDialog("RF Cable Calibration : Source Assemblies", calPaths.length(), 2);

                for (int row = 0; row < calPaths.length(); row++)
                {
                    gridDialog.setItem(calPaths.at(row)->brief() + "...", row, 0);
                }

                // Show the dialog
                gridDialog.show();  */

                // Run all of the cable cals
                for (int i = 0; (i < calPaths.length()) && !cancelled; i++)
                {
                    // get pointer to this path
                    RfPathSourceAssy* path = calPaths.at(i);

                    // Get pointer to previous path, leave it as null if this is first or second path
                    RfPathSourceAssy* prevPath = 0;
                    if (i > 1) { prevPath = calPaths.at(i - 1); }

                    // Create new cable cal object and set test power according to path
                    SanityCheckCal* pCableCal = new  SanityCheckCal(path->key());
                    pCableCal->setTestPowerLevel(path->testPower());
                    sanityCheckCals.append(pCableCal);

                     ok &= rfSwitch.setDutRfPort(path->dutPort());
                     ok &= rfSwitch.setTestEquipment(path->tePort());

                    // Create a Message Dialog
                    gui::MessageDialog msgDialog(pCableCal->measureTestJigInstruction(), gui::MessageDialogType::INFO, this);

                    // Did user press OK?
                    if (msgDialog.exec())
                    {
                        sanityCheckCals.at(0)->testRfPower(testjig::SanityCheckType::SOUCEBOARD, cancelled, RfPathHighBandModule::RfCableSet::UNKNOWN, SanityCheckCal::KStopFreqSourceBoard);
                    }
                    else
                    {
                        cancelled = true;
                    }

                   /* if (!cancelled)
                    {
                        gridDialog.setItem("Done", i, 1);
                    }
                    */
                }

                // If it all worked without being cancelled then store all of the cable cals
                // (index 0 is the null path - we don't need to store that)
                if (ok && !cancelled)
                {
                }

                // Delete all of the path and cable cal objects
                for (int i = 0; i < calPaths.length(); i++)
                {
                    delete calPaths.at(i);
                }

                for (int i = 0; i <  sanityCheckCals.length(); i++)
                {
                    delete  sanityCheckCals.at(i);
                }
            }

            if (ok && !cancelled)
            {
                emit sanityCheckOk(0u);
            }
     }

    QDate MercuryTestJig1::calExpiry() const
    {
        QDate expiry;

        common::Calibration calSrcAssy(kSourceAssyCableCalKey);
        common::Calibration calPwrMon(kPwrMonCableCalKey);
        common::Calibration calHighBand(kHBCableCalKey);

        if (calSrcAssy.calExpiry() > calPwrMon.calExpiry())
        {
            expiry = calPwrMon.calExpiry();
        }
        else
        {
            expiry = calSrcAssy.calExpiry();
        }

        if(expiry  > calHighBand.calExpiry())
        {
            expiry = calHighBand.calExpiry();
        }

        return expiry;
    }

    bool MercuryTestJig1::isCalExpired() const
    {
        common::Calibration calSrcAssy(kSourceAssyCableCalKey);
        common::Calibration calPwrMon(kPwrMonCableCalKey);
        common::Calibration calHighBand(kHBCableCalKey);

        return (calSrcAssy.isCalExpired() || calPwrMon.isCalExpired()  ||   calHighBand.isCalExpired());
    }

    bool MercuryTestJig1::isCalNearlyExpired() const
    {
        common::Calibration calSrcAssy(kSourceAssyCableCalKey);
        common::Calibration calPwrMon(kPwrMonCableCalKey);
        common::Calibration calHighBand(kHBCableCalKey);

        calSrcAssy.setWarnDays(CableCal::kCalWarnDays);
        calPwrMon.setWarnDays(CableCal::kCalWarnDays);
        calHighBand.setWarnDays(CableCal::kCalWarnDays);

        return (calSrcAssy.isCalNearlyExpired() || calPwrMon.isCalNearlyExpired()  || calHighBand.isCalNearlyExpired());
    }
}
