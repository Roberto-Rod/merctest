/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "mainwidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{

    // Put files to be crunched in the selected directory e.g. below.
    // they must all be of the same assy number or PCB.
    // ** Note selected directory must have the sub directory "/stats"
    // for the output results log. eg. C:/SJ/QT/crunch/TestData/PM/MB/stats/ASY0021.txt

    // logfile  dblog("C:/SJ/QT/crunch/TestData/DB/HB");
    // logfile  dblog("C:/SJ/QT/crunch/TestData/DB/MB");
    // logfile  dblog("C:/SJ/QT/crunch/TestData/PM/HB");
    // logfile  dblog("C:/SJ/QT/crunch/TestData/PM/LB");
       logfile  dblog("C:/SJ/QT/crunch/TestData/SB/CIL");



    QTextStream *txtPtr = 0;

    // Get next log file for crunching.
    bool ok = dblog.getNextFile(&txtPtr);
    QString assy;
    dblog.getDUT( assy );

    // Create parse utility object for
    // parsing a line of the log file
    parseUtil * puPtr = new parseUtil();

    // The DUT object to be created is determined from the log
    // file ("ASY") . Note the directory must contain files of the
    // same DUT.

    // A list of all the tests to crunch in one file is generated.
    // The object generated is dependant on the assy type.
    // The parse utility is added to the object on creation.
    execObjList testObjList;
    QList <dataInterface *> * toList;
    toList =  testObjList.getTestObj( assy, puPtr);


    QString pth = dblog.getPath();
    QString logfile = pth+"/stats/"+assy+".txt";
    testObjList.setObjList(logfile, toList);

    // loop all log files, until no more
    while(  ok   )
    {
        // Set log file to play with
        puPtr->setFilePtr(txtPtr);
        // Get data from all tests from a single log file.
        // Data from subsequent files is stored in a vector
        // for processing by "getAlStats()"
        testObjList.getAllData();
        // TBD get attenution data (new class design)
        ok = dblog.getNextFile(&txtPtr);
    }

    // Crunch vector of results data to calculate
    // mean and SD data for each path and frequency
    testObjList.getAllStats();

}








