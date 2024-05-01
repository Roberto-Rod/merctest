#include "tester.hpp"

#include <QApplication>

namespace testsuite
{
    Tester::Tester(QObject* parent) :
        QObject(parent),
        m_waiting (false)
    {
    }

    void Tester::waitStep(QString message)
    {
        emit step(message);

        m_waiting = true;

        // Keep processing application events whilst waiting for the stepOn()
        // slot to be called
        while (m_waiting)
        {
            QApplication::processEvents();
        }
    }

    void Tester::stepOn()
    {
        m_waiting = false;
    }
}
