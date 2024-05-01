#ifndef TESTJIGFACTORY_HPP
#define TESTJIGFACTORY_HPP

#include <QObject>

namespace testjig
{
    class TestJig;

    class TestJigFactory : public QObject
    {
    public:
        ~TestJigFactory();

        static TestJigFactory* instance();

        TestJig* makeTestJig(QObject* parent = 0);

    protected:
        TestJigFactory(QObject* parent = 0);

    private:
        static TestJigFactory* m_pInstance;
    };
}
#endif // TESTJIGFACTORY_HPP
