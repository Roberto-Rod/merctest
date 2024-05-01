#ifndef TESTJIGTREEWIDGETITEM_HPP
#define TESTJIGTREEWIDGETITEM_HPP

#include <QTreeWidgetItem>

namespace testjig
{
    class TestJig;
}

namespace gui
{
    class TestJigTreeWidgetItem : public QTreeWidgetItem
    {
    public:
        TestJigTreeWidgetItem(testjig::TestJig* pJig = 0);

        testjig::TestJig* jig();

        void update();

    private:
        testjig::TestJig* m_pJig;
    };
}

#endif // TESTJIGTREEWIDGETITEM_HPP
