#include "resultstat.h"
#include <QtMath>

resultStat::resultStat(QString namep) :
    m_name(namep)
{
    m_vals.clear();
}


resultStat::resultStat()
{
    m_vals.clear();
}


void resultStat::addVal(double val)
{
    m_n++;
    m_partialMean += val;
    m_vals.append(val);
}


void resultStat::setVal(double val)
{
    m_n++;
    m_partialMean += val;
    m_vals.append(val);
}



void resultStat::crunchsd()
{
    double mean = calcMean();
    double temp = 0;
    
    for(int i=0; i < m_vals.size(); i++)
    {
        double val = m_vals.at(i);
        temp = (mean - val)*(mean - val);
        m_partialSD += temp;
    }

}


double resultStat::calcMean()
{
    double x = m_partialMean / m_n;
    return x;
}

double resultStat::calcSD()
{
    this->crunchsd();
    double x = m_partialSD / m_n;
    x = qSqrt(x)    ;
    return x;
}









