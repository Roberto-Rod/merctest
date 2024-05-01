#ifndef TESTER_HPP
#define TESTER_HPP

#include <QObject>
#include <QString>

namespace testsuite {
    class Tester : public QObject
    {
        Q_OBJECT
    public:
        Tester(QObject* parent = 0);
        ~Tester() {}

    signals:
        step(QString message);

    public slots:
        void stepOn();

    private:
        bool m_waiting;

        void waitStep(QString message);
    };
}

#endif // TESTER_HPP
