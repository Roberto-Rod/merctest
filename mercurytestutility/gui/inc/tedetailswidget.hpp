/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TEDETAILSWIDGET_HPP
#define TEDETAILSWIDGET_HPP

#include "testequipment.hpp"

#include <Qt>
#include <QWidget>
#include <QDate>

QT_BEGIN_NAMESPACE
class QCalendarWidget;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLineEdit;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

namespace te
{
    class TestEquipment;
}

namespace gui
{
    class TeDetailsWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit TeDetailsWidget(te::TestEquipment* te, QWidget* parent = 0, Qt::WindowFlags f = 0);
        ~TeDetailsWidget();

    signals:
        void edited();

    public slots:
        void applyChanges();

    private slots:
        void update();
        void updateResourceName();
        void dateEditChanged();
        void calendarChanged();
        void updateCalStatus();
        void setExpiryToday();

    private:
        // Local constants
        static const int32_t kIdFieldMaxLength = 64;
        static const int32_t kVisaMaxLength    = 128;

        static const QDate kMinDate;
        static const QDate kMaxDate;

        // Methods to create group boxes
        QGroupBox* createVisaGroupBox();
        QGroupBox* createDetailsGroupBox();
        QGroupBox* createCalExpiryGroupBox();

        // Pointer to the test equipment this widget is editing
        te::TestEquipment* m_pTestEquipment;

        // Widgets in the VISA group box
        QLineEdit*         m_pResourceNameLineEdit;
        QPushButton*       m_pUpdateResourceButton;

        // Widgets in the details group box
        QLineEdit*         m_pMfrLineEdit;
        QLineEdit*         m_pModelLineEdit;
        QLineEdit*         m_pSerialLineEdit;
        QLineEdit*         m_pFirmwareLineEdit;

        // Widgets in the cal expiry group box
        QPushButton*       m_pTodayButton;
        QCalendarWidget*   m_pCalendar;
        QDateEdit*         m_pDateEdit;
        QLabel*            m_pCalLabel;

        // Changed states
        bool               m_calExpiryEdited;
    };
}

#endif
