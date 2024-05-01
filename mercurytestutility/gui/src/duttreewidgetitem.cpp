#include "duttreewidgetitem.hpp"
#include "deviceundertest.hpp"

namespace gui
{
    DutTreeWidgetItem::DutTreeWidgetItem(QTreeWidgetItem *parent, dut::DeviceUnderTest* pDut) :
        QTreeWidgetItem(parent),
        m_pDut(pDut)
    {
        update();
    }

    dut::DeviceUnderTest* DutTreeWidgetItem::dut()
    {
        return m_pDut;
    }

    void DutTreeWidgetItem::update()
    {
        // Put the generic name/device number(s) in first and second columns
        setText(0, m_pDut->genericVariant()->m_name);
        setText(1, m_pDut->genericVariant()->m_number);
    }
}
