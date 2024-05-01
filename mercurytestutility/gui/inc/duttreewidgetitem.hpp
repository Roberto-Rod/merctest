#ifndef DUTTREEWIDGETITEM_HPP
#define DUTTREEWIDGETITEM_HPP

#include <QTreeWidgetItem>

namespace dut
{
    class DeviceUnderTest;
}

namespace gui
{
    class DutTreeWidgetItem : public QTreeWidgetItem
    {
    public:
        DutTreeWidgetItem(QTreeWidgetItem *parent, dut::DeviceUnderTest* pDut = 0);

        dut::DeviceUnderTest* dut();

        void update();

    private:
        dut::DeviceUnderTest* m_pDut;
    };
}

#endif // DUTTREEWIDGETITEM_HPP
