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
#include <QtCharts/QChart>
#include <QValueAxis>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCore/QtMath>
#include <QDir>
#include <QFile.h>
#include <QList>
#include <QTextStream>
#include <QIODevice>
#include <QtMath>


QT_CHARTS_USE_NAMESPACE

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    QDir myDir("C:/SJ/dds_cal");
    //QDir myDir("C:/SJ/dds_testcal");
    QDir myBadDir("C:/SJ/dds_badcal");
    QDir myGoodDir("C:/SJ/dds_goodcal");


    // Create chart view with the chart
    m_chart = new QChart();  
    m_chartView = new QChartView(m_chart, this);

    m_chart2 = new QChart();
    m_chartView2 = new QChartView(m_chart2, this );


    // Create layout for grid and detached legend
    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);   
    m_mainLayout->addWidget(m_chartView2,4, 1, 3, 1);
    setLayout(m_mainLayout);


    QStringList filters, filtersTest;
    filters << "*.csv";
    filtersTest << "lower.txt" << "mean.txt" << "upper.txt";
    myDir.setNameFilters(filters);
    //myDir.setNameFilters(filtersTest);
    myGoodDir.setNameFilters(filters);

     QFileInfoList list = myDir.entryInfoList();
     QFileInfoList glist = myGoodDir.entryInfoList();
    /*QFileInfo fileInfo = list.at(0);
    QString fl = fileInfo.fileName();
    QString abpath = myDir.absolutePath();
    QString pthfile = abpath+"/"+fl;  */

    unsigned int i;
    for(i=0; i < myDir.count(); i++)
    {
        m_seriesCnt++;
        QLineSeries s1;
        QFileInfo fileInfo = list.at(i);
        QString fl = fileInfo.fileName();
        QString abpath = myDir.absolutePath();
        QString badabpath =  myBadDir.absolutePath();
        QString goodabpath =  myGoodDir.absolutePath();
        QString pthfile = abpath+"/"+fl;        
        getSeriesfromFile(pthfile);
        QString ff = QString::number(m_failfreq);
        QString as = QString::number(m_failasf);
        QString flm = QString::number(m_flimit);
        QString badpthfile = badabpath+"/"+fl+ff+"-"+as+"-"+flm+".csv";
        QString goodpthfile = goodabpath+"/"+fl;

        if( m_badSeriesFl== true)
        {

            QFile ddsfile( pthfile );
            ddsfile.copy(pthfile, badpthfile);
        }
        else
        {
              QFile ddsfile( pthfile );
              ddsfile.copy(pthfile, goodpthfile);
        }
    }


    for(i=0; i < myGoodDir.count(); i++)
    {
        QFileInfo gfileInfo = glist.at(i);
        QString fl = gfileInfo.fileName();
        QString abpath = myGoodDir.absolutePath();
        QString pthfile = abpath+"/"+fl;
        storeStats(pthfile);
    }

    crunchStats();
    outputStats();
    extractBadData();

    // get file fromdirectory
    // get points from file store in lineseries.
    // pass to addSeries

    // Add few series
    //addSeries();
    //addSeries();
    //addSeries();
    //addSeries();

    connectMarkers();

    // Set the title and show legend
    m_chart->setTitle("Good DDS Cals (click on legend)");
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chart2->setTitle("Bad DDS Cals");
    m_chart2->legend()->setVisible(true);
    m_chart2->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView2->setRenderHint(QPainter::Antialiasing);

}

void MainWidget:: storeStats(QString file)
{
    QFile ddsfile( file );
    ddsfile.open(  QIODevice::ReadOnly);
    QRegExp serialNum("^([^,]+),\\s?([^$]+)");
    QRegExp freqLine("^([^,]+),\\s?([^$]+)");
    //QRegExp freqLine("^([^,]+),([^\\W]+)$");
    QRegExp  serialNumRx("pcb0044_[^_]+_([^_]+)_");

    QTextStream myStream(&ddsfile);
    QString s1,s2,s3,s4;
    s1 = myStream.readLine();
    s2 = myStream.readLine();
    s3 = myStream.readLine();
    s4 = myStream.readLine();
    QString myString;

    // Capture serial number from file name
    serialNumRx.indexIn(file);
    serialNum.indexIn(s2);
    QString sn;
    sn = serialNum.cap(2);

    while( myStream.atEnd() == 0)
    {
        QString fs, asfs;
        uint16_t fsi=0;

        myString = myStream.readLine();
        // Capture test frequency and ASF from line
        if(freqLine.indexIn(myString) !=-1)
        {
            fs = freqLine.cap(1);
            fsi = (fs.toInt() /1e6);
            fs =QString::number( fsi);
            asfs = freqLine.cap(2);
            if(m_storeTfreqs== true)
            {
                m_vtFreqs.append(fs);
            }
            m_freqtoAsf.insertMulti(fs,asfs.toInt());
           //fx =   fs.toInt() / 1e6;
        }
    }
    m_storeTfreqs = false;
}



 void MainWidget::crunchStats()
 {
     QString key;
     uint16_t num;
     uint32_t sumMean=0;
     uint32_t sumVar=0;
     for(int i=0; i <  m_vtFreqs.size(); i++)
     {
         key = m_vtFreqs.at(i);
         QList  <uint16_t>  asfv  = m_freqtoAsf.values(key);
         num = asfv.size();
          sumMean=0;
         for( int j=0; j < asfv.size(); j++ )
         {
             sumMean += asfv.at(j);
         }
         uint16_t mean = sumMean/ num;
         sumVar=0;
         for( int j=0; j< asfv.size(); j++)
         {
             sumVar += (asfv.at(j) -mean) *(asfv.at(j) -mean);
         }
         double_t  variance = sumVar / num;
         double_t  sd = qSqrt(variance);

         stat vstat;
         vstat.mean = mean;
         vstat.sd = sd;
          m_freqtoStat.insert(key,vstat);
     }


 }


  //  { 20,   370, 490+z+12, },



  void MainWidget::outputStats()
  {
      QFile limitf ( "C:/SJ/dds_goodcal/limits.txt");
      QFile lowerf("C:/SJ/dds_goodcal/lower.csv");
      QFile meanf("C:/SJ/dds_goodcal/mean.csv");
      QFile upperf("C:/SJ/dds_goodcal/upper.csv");
      limitf.open(QIODevice::WriteOnly  |  QIODevice::Text);
      meanf.open(QIODevice::WriteOnly  |  QIODevice::Text);
      lowerf.open(QIODevice::WriteOnly  |  QIODevice::Text);
      upperf.open(QIODevice::WriteOnly  |  QIODevice::Text);
      QTextStream slimit(&limitf);
      QTextStream smean(&meanf);
      QTextStream slower(&lowerf);
      QTextStream supper(&upperf);
      slimit << "\nDDS Limits per frequency\n";
      double  sdx = 4.0;
      QString ssdx = QString::number(sdx);
      slimit << "\nSD factor"  <<  ssdx;

      slower <<   "file_version,0.1"  <<  "\nserial,000000"  <<  "\n" <<  "\nfreq_Hz,asf";
      smean <<   "file_version,0.1"  <<  "\nserial,000001" <<  "\n" <<  "\nfreq_Hz,asf";
      supper <<  "file_version,0.1"  <<  "\nserial,000002"  <<  "\n" <<  "\nfreq_Hz,asf";

      for(int i =0; i < m_vtFreqs.size(); i++)
      {
          QString key = m_vtFreqs.at(i);
          stat vstat = m_freqtoStat.value(key);
          uint16_t lo = vstat.mean - (sdx * vstat.sd);  // was 6
          uint16_t hi = vstat.mean +  (sdx  * vstat.sd);  // was 6
          uint32_t f=key.toInt();
          f *= 1e6;
          QString fs = QString::number(f);
          QString los = QString::number(lo);
          QString his = QString::number(hi);
          slimit    << "\n { "<< key  << ",  " << los << ",  " <<  his << ",  }," ;
          smean  << "\n" <<  fs << "," <<  vstat.mean;
          slower  << "\n" <<  fs << "," <<  lo;
          supper << "\n" <<  fs  << ","<<  hi;
      }

      smean << "\n";
      slower << "\n";
      supper << "\n";

  /* file_version,0.1
      serial,570077

      freq_Hz,asf
      20000000,434
      40000000,434 */

  }



  void MainWidget::extractBadData()
  {
      QFile badf ("C:/SJ/dds_badcal/badcal.txt");
      badf.open(QIODevice::WriteOnly | QIODevice::Text );
      QTextStream badfs(&badf);
      badfs << "Bad DDS Cals ";
      QMap <QString, bp > sntoBand;
      QList <QString>  sn;
      bp bpv;

      for(int i =0; i < dds_size ; i++  )
      {
          QString f =  QString::number(m_ddslimits[i].fr);
          badfs <<  "\n\nFrequency " <<  f << "MHz";
          QList   <sb> values = m_map.values(f) ;
          for (int i =0; i < values.size(); i++)
          {
              QString s =  values.at(i).serial;
              char p = bandPower( values.at(i).actual.toInt(),  values.at(i).limit.toInt() );
              bpv.bfr = f;
              bpv.powerChar = p;

              if( sntoBand.contains(s))
              {
                  //sn.append(s);
                  sntoBand.insertMulti(s,bpv);
              }
              else
              {
                  sn.append(s);
                  sntoBand.insert(s,bpv);
              }
              badfs << "\nSerial: " <<  values.at(i).serial;
              badfs << " Frequency:  " << values.at(i).testFreq;
              badfs <<  " Limit:  " << values.at(i).limit;
              badfs <<  " Actual: " <<  values.at(i).actual;

          }
      }

      badfs << "\n\n\n Serial Number / Band Failure Analysis\n";

      for(int i=0; i < sn.size(); i++)
      {
          QString s = sn.at(i);
          badfs << "\n S/N: "  <<  s  << " Bands :  ";
          QList <bp> bandlist = sntoBand.values(s);
          for(int j=0; j < bandlist.size(); j++)
          {
              badfs << " " << bandlist.at(j).bfr << bandlist.at(j).powerChar;
          }

      }








     /* badfs <<  "\n\nFrequency 1380 MHz";
      values = m_map.values("1380") ;
      for (int i =0; i < values.size(); i++)
      {
          badfs << "  \nSerial: " <<  values.at(i).serial;
          badfs <<  " Limit:  " << values.at(i).limit;
          badfs <<  " Actual: " <<  values.at(i).actual;
      } */

  }




   char MainWidget::bandPower(uint16_t val, uint16_t limit)
   {
       uint16_t diff;
       char retChar= ' ';
       if( val < limit )
       {
           diff = limit -val;
           if (diff  < 30 )
           {
               retChar= '!';
           }
       }
       else
       {
           diff = val-limit;
           if (diff  < 30 )
           {
               retChar= '*';
           }

       }
       return retChar;
   }


   bool  MainWidget::limitcheck(uint16_t freq, uint16_t val)
   {
       bool result = false;
       for( uint16_t i = 0 ;  i <=   dds_size; i ++ )
       {
           if( freq >  m_ddslimits[i].fr  )
               continue;
           else
           {
               if( val >=  m_ddslimits[i].lo && val  <=  m_ddslimits[i].hi)
               {
                   result = true;                    
                }
               else
               {
                   if( val <=  m_ddslimits[i].lo )
                   {
                        m_flimit =  m_ddslimits[i].lo;
                   }
                   else
                   {
                        m_flimit =  m_ddslimits[i].hi;
                   }
                   m_failBand =  m_ddslimits[i].fr;
                   result = false;
               }
               break;
           }
       }
       return result;
   }


void MainWidget::getSeriesfromFile( QString ddsfileStr )
{
    m_badSeriesFl = false;
    QFile ddsfile( ddsfileStr );
    ddsfile.open(  QIODevice::ReadOnly);
    QRegExp h1("file_version, 0.1");
    QRegExp serialNum("^([^,]+),\\s?([^$]+)");
    QRegExp freqLine("^([^,]+),\\s?([^$]+)");
    //QRegExp freqLine("^([^,]+),([^\\W]+)$");
    QRegExp asfLine(",\n");
    QRegExp  serialNumRx("pcb0044_[^_]+_([^_]+)_");

    QTextStream myStream(&ddsfile);
    QString s1,s2,s3,s4;
    s1 = myStream.readLine();
    s2 = myStream.readLine();
    s3 = myStream.readLine();
    s4 = myStream.readLine();
    QString myString;
    QList<QPointF> data;

    serialNumRx.indexIn(ddsfileStr);
    serialNum.indexIn(s2);
    QString sn;
    sn = serialNum.cap(2);
    bool goodData = true;
    uint16_t fx;
    uint16_t as;



    while( myStream.atEnd() == 0)
    {
        QString fs, asfs;

        myString = myStream.readLine();
        if(freqLine.indexIn(myString) !=-1)
        {
            fs= freqLine.cap(1);
            asfs = freqLine.cap(2);
           //fx =   fs.toInt() / 1e6;
            if (! this->limitcheck(fs.toInt()/1e6,asfs.toInt() ))
            {                  
                 fx =   fs.toInt() / 1e6;
                 QString sfx = QString::number(fx);
                 QString slimit = QString::number(m_flimit);
                 QString sfailBand = QString::number(m_failBand);
                 as = asfs.toInt();
                 sb sbv;
                 sbv.serial = sn;
                 sbv.testFreq = sfx;
                 sbv.actual = asfs;
                 sbv.limit = slimit;
                 m_map.insertMulti( sfailBand,sbv);
                 goodData = false;
            }
            data.append(QPointF((fs.toFloat()/1e6),asfs.toFloat()));
        }       
    }

    QLineSeries *series = new QLineSeries();
    if(goodData)
    {
        m_series.append(series);
         series->setName(QString(sn));
         series->append(data);
        m_chart->addSeries(series);
        m_goodCnt++;
    }
    else
    {
        m_badSeries.append(series);
        series->setName(QString(sn));
        series->append(data);
        m_chart2->addSeries(series);
        m_badCnt++;
        m_badSeriesFl =  true;
        m_failfreq = fx;
        m_failasf = as;
    }



      //m_chart->addSeries(series);

    if (m_initAxis1 == true) {
    //if (true) {
        //m_chart->createDefaultAxes();
        QValueAxis * xaxis = new QValueAxis;
        xaxis->setTickCount(15);
        xaxis->setMax(1500);
        xaxis->setMin(20);
        xaxis->setLabelFormat("%1d");
        QValueAxis * yaxis = new QValueAxis;
        yaxis->setTickCount(10);
        //yaxis->setMax(944);
        //yaxis->setMin(434);
        yaxis->setRange(434,944);
        yaxis->setLabelFormat("%1d");
        m_chartView->chart()->setAxisX(xaxis,series);
        m_chartView->chart()->setAxisY(yaxis,series);
        m_initAxis1 = false;
    }

    if (m_initAxis2 == true) {
        //m_chart->createDefaultAxes();
        QValueAxis * xaxis = new QValueAxis;
        xaxis->setTickCount(15);
        xaxis->setMax(1500);
        xaxis->setMin(20);
        xaxis->setLabelFormat("%1d");
        QValueAxis * yaxis = new QValueAxis;
        yaxis->setTickCount(10);
        yaxis->setMax(944);
        yaxis->setMin(434);
        yaxis->setLabelFormat("%1d");
        m_chartView2->chart()->setAxisX(xaxis,series);
        m_chartView2->chart()->setAxisY(yaxis,series);
        m_initAxis2 = false;
    }






}






/*void MainWidget::addSeries()
{
    QLineSeries *series = new QLineSeries();
    m_series.append(series);

    series->setName(QString("line " + QString::number(m_series.count())));

    // Make some sine wave for data
    QList<QPointF> data;
    int offset = m_chart->series().count();
    for (int i = 0; i < 360; i++) {
        qreal x = offset * 20 + i;
        data.append(QPointF(i, qSin(2.0 * 3.141592 * x / 360.0)));
    }

    series->append(data);
    m_chart->addSeries(series);

    if (m_series.count() == 1) {
        m_chart->createDefaultAxes();
    }
}  */

void MainWidget::removeSeries()
{
    // Remove last series from chart
    if (m_series.count() > 0) {
        QLineSeries *series = m_series.last();
        m_chart->removeSeries(series);
        m_series.removeLast();
        delete series;
    }
}

void MainWidget::connectMarkers()
{
//![1]
    // Connect all markers to handler
    foreach (QLegendMarker* marker, m_chart->legend()->markers()) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
        QObject::connect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));       
        //QObject::connect( series, SIGNAL(clicked()), this, SLOT(handleMarkerSeriesClicked()));
    }
//![1]
}

void MainWidget::disconnectMarkers()
{
//![2]
    foreach (QLegendMarker* marker, m_chart->legend()->markers()) {
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
    }
//![2]
}

void MainWidget::handleMarkerClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible()) {
            alpha = 0.5;
        }

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}


void MainWidget::handleMarkerSeriesClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible()) {
            alpha = 0.5;
        }

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}
