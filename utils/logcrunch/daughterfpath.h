#ifndef DAUGHTERFPATH_H
#define DAUGHTERFPATH_H

#include <QMap>


class daughterRfPath
{
public:
    daughterRfPath();
    struct stat{
        double mean;
        double sd;

    };

    struct rfdata{
        double power;
        double rej1;
        double rej2;
        double rej3;
    };

    QMap<uint16_t,  rfdata>  m_pathFreqtoRfdata;

};

#endif // DAUGHTERFPATH_H
