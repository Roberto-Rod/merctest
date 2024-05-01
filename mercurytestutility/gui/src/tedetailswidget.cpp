/****************************************************************************
**
**
****************************************************************************/

#include <QtWidgets>
#include "tedetailswidget.hpp"
#include "visatestequipment.hpp"
#include "testequipment.hpp"
#include "logging.hpp"

namespace gui
{
    const QDate TeDetailsWidget::kMinDate = QDate(2000, 1, 1);
    const QDate TeDetailsWidget::kMaxDate = QDate(3000, 12, 31);

    TeDetailsWidget::TeDetailsWidget(te::TestEquipment* te, QWidget* parent, Qt::WindowFlags f) :
        QWidget(parent, f),
        m_pTestEquipment(te),
        m_calExpiryEdited(false)
    {
        QVBoxLayout *pLayout = new QVBoxLayout(this);

        pLayout->addWidget(createVisaGroupBox());
        pLayout->addWidget(createDetailsGroupBox());
        pLayout->addWidget(createCalExpiryGroupBox());

        update();

        setLayout(pLayout);
    }

    TeDetailsWidget::~TeDetailsWidget()
    {
    }

    QGroupBox* TeDetailsWidget::createVisaGroupBox()
    {
        QGroupBox* pVisaGroupBox = new QGroupBox(tr("Visa Resource"));

        QVBoxLayout* pLayout = new QVBoxLayout();

        pLayout->addWidget(new QLabel(tr("VISA Resource Name")));

        m_pResourceNameLineEdit = new QLineEdit;
        m_pResourceNameLineEdit->setMaxLength(kVisaMaxLength);
        pLayout->addWidget(m_pResourceNameLineEdit);

        m_pUpdateResourceButton = new QPushButton(tr("Update"));
        m_pUpdateResourceButton->setToolTip(tr("Re-attempt to connect to the equipment and get the details."));

        QHBoxLayout* pHLayout = new QHBoxLayout;
        pHLayout->addWidget(m_pUpdateResourceButton);
        pHLayout->addStretch();

        pLayout->addLayout(pHLayout);

        connect(m_pUpdateResourceButton, SIGNAL(clicked()),
                this,                    SLOT(updateResourceName()));

        pVisaGroupBox->setLayout(pLayout);

        return pVisaGroupBox;
    }

    QGroupBox* TeDetailsWidget::createDetailsGroupBox()
    {
        QGroupBox* pDetailsGroupBox  = new QGroupBox(tr("Test Equipment Details"));

        QFormLayout* pFormLayout = new QFormLayout();

        m_pMfrLineEdit = new QLineEdit;
        m_pMfrLineEdit->setMaxLength(kIdFieldMaxLength);
        m_pMfrLineEdit->setEnabled(false);
        pFormLayout->addRow(new QLabel(tr("Manufacturer")), m_pMfrLineEdit);

        m_pModelLineEdit = new QLineEdit;
        m_pModelLineEdit->setMaxLength(kIdFieldMaxLength);
        m_pModelLineEdit->setEnabled(false);
        pFormLayout->addRow(new QLabel(tr("Model")), m_pModelLineEdit);

        m_pSerialLineEdit = new QLineEdit;
        m_pSerialLineEdit->setMaxLength(kIdFieldMaxLength);
        m_pSerialLineEdit->setEnabled(false);
        pFormLayout->addRow(new QLabel(tr("Serial Number")), m_pSerialLineEdit);

        m_pFirmwareLineEdit = new QLineEdit;
        m_pFirmwareLineEdit->setMaxLength(kIdFieldMaxLength);
        m_pFirmwareLineEdit->setEnabled(false);
        pFormLayout->addRow(new QLabel(tr("Firmware Version(s)")), m_pFirmwareLineEdit);

        pDetailsGroupBox->setLayout(pFormLayout);

        return pDetailsGroupBox;
    }

    QGroupBox* TeDetailsWidget::createCalExpiryGroupBox()
    {
        QGroupBox* pCalExpiryGroupBox = new QGroupBox(tr("Calibration Expiry"));

        m_pCalendar = new QCalendarWidget();
        m_pCalendar->setMinimumDate(kMinDate);
        m_pCalendar->setMaximumDate(kMaxDate);
        m_pCalendar->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        m_pCalendar->setGridVisible(true);
        m_pCalendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

        m_pDateEdit = new QDateEdit();
        m_pDateEdit->setMinimumDate(kMinDate);
        m_pDateEdit->setMaximumDate(kMaxDate);

        m_pCalLabel = new QLabel();
        updateCalStatus();

        m_pTodayButton = new QPushButton(tr("Today"));

        QHBoxLayout* pHLayoutToday = new QHBoxLayout();
        pHLayoutToday->addWidget(m_pDateEdit);
        pHLayoutToday->addStretch();
        pHLayoutToday->addWidget(m_pTodayButton);

        QHBoxLayout* pHLayoutCalendar = new QHBoxLayout();
        pHLayoutCalendar->addStretch();
        pHLayoutCalendar->addWidget(m_pCalendar);
        pHLayoutCalendar->addStretch();

        QHBoxLayout* pHLayoutLabel = new QHBoxLayout();
        pHLayoutLabel->addStretch();
        pHLayoutLabel->addWidget(m_pCalLabel);
        pHLayoutLabel->addStretch();

        QVBoxLayout* pVLayout = new QVBoxLayout();
        pVLayout->addLayout(pHLayoutToday);
        pVLayout->addLayout(pHLayoutCalendar);
        pVLayout->addLayout(pHLayoutLabel);

        pCalExpiryGroupBox->setLayout(pVLayout);

        connect(m_pDateEdit,  SIGNAL(dateChanged(QDate)),
                this,         SLOT(dateEditChanged()));
        connect(m_pCalendar,  SIGNAL(selectionChanged()),
                this,         SLOT(calendarChanged()));
        connect(m_pTodayButton, SIGNAL(clicked()),
                this,         SLOT(setExpiryToday()));

        return pCalExpiryGroupBox;
    }

    void TeDetailsWidget::update()
    {
        // Write the details into the LineEdit boxes
        m_pResourceNameLineEdit->setText(m_pTestEquipment->resourceName());
        m_pMfrLineEdit->setText(m_pTestEquipment->details().m_manufacturer);
        m_pModelLineEdit->setText(m_pTestEquipment->details().m_model);
        m_pSerialLineEdit->setText(m_pTestEquipment->details().m_serialNumber);
        m_pFirmwareLineEdit->setText(m_pTestEquipment->details().m_firmwareVersion);

        // Set the cal expiry date in the DateEdit box and calendar
        m_pDateEdit->setDate(m_pTestEquipment->calExpiry());
        m_pCalendar->setSelectedDate(m_pTestEquipment->calExpiry());

        // If the test equipment has a valid calibration key then allow the calibration date to be edited
        bool en = m_pTestEquipment->isCalKeyValid();
        m_pDateEdit->setEnabled(en);
        m_pCalendar->setEnabled(en);
        m_pTodayButton->setEnabled(en);
        if(!en) { m_pCalLabel->setText(""); }
    }

    void TeDetailsWidget::updateResourceName()
    {
        QString resourceName = m_pResourceNameLineEdit->text();

        common::Logging* pLog = common::Logging::instance();

        pLog->log(QString("Polling VISA Resource: %1").arg(resourceName),
                       common::Logging::LOG_DEST_PROGRESS);

        // Set the resource name in the TE object
        m_pTestEquipment->setResourceName(resourceName);

        // Store the resource name in the settings
        te::ResourceType type = m_pTestEquipment->resourceType();
        common::Settings::instance()->setResourceName(type, resourceName);

        // Poll the resource
        if (m_pTestEquipment->connect())
        {
           pLog->log("OK: VISA Resource Detected",
                          common::Logging::LOG_DEST_PROGRESS);
        }
        else
        {
            pLog->log("ERROR: VISA Resource Not Detected or Connected Model Not Supported",
                           common::Logging::LOG_DEST_PROGRESS);
        }

        update();
    }

    void TeDetailsWidget::dateEditChanged()
    {
        m_pCalendar->setSelectedDate(m_pDateEdit->date());
        updateCalStatus();
    }

    void TeDetailsWidget::calendarChanged()
    {
        m_pDateEdit->setDate(m_pCalendar->selectedDate());
        updateCalStatus();
    }

    void TeDetailsWidget::updateCalStatus()
    {
        // Update calibration status for the selected date - note that this expiry date
        // has not necessarily been set in the object. We use static methods in the
        // test equipment class to test the expiry status for the selected date.
        QDate expiry(m_pCalendar->selectedDate());

        QString status = "Calibration Valid";

        if (m_pTestEquipment->isCalExpired(expiry))
        {
            status = "** CALIBRATION EXPIRED **";
        }
        else if (m_pTestEquipment->isCalNearlyExpired(expiry))
        {
            int32_t daysToExpiry = QDate::currentDate().daysTo(expiry);

            if (daysToExpiry == 0)
            {
                status = QString("Warning: Calibration Expires Today!");
            }
            else if (daysToExpiry == 1)
            {
                status = QString("Warning: Calibration Expires in 1 Day");
            }
            else
            {
                status = QString("Warning: Calibration Expires in %1 Days").arg(daysToExpiry);
            }
        }

        m_pCalLabel->setText(status);

        m_calExpiryEdited = true;
        emit edited();
    }

    void TeDetailsWidget::setExpiryToday()
    {
        m_pDateEdit->setDate(QDate::currentDate());
        m_pCalendar->setSelectedDate(QDate::currentDate());
    }

    void TeDetailsWidget::applyChanges()
    {
        if (m_calExpiryEdited)
        {
            // Update the calibration expiry date in the test equipment object
            m_pTestEquipment->setCalExpiry(m_pCalendar->selectedDate());
            m_calExpiryEdited = false;
        }
    }
}
