#ifndef RESULTSTAT_H
#define RESULTSTAT_H

#include <QVector>

class resultStat
{
public:
    resultStat();
    resultStat(QString name);
    void addVal(double val);
    void setVal(double val);
    double calcMean();
    double calcSD();   

private:    
    void crunchsd();
    QString m_name ="";
    QVector<double> m_vals;
    double m_partialMean=0;
    double m_partialSD=0;
    double m_mean=0;
    uint16_t m_n=0;
};

#endif // RESULTSTAT_H
