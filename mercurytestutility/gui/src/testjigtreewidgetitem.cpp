#include "testjigtreewidgetitem.hpp"
#include "deviceundertest.hpp"
#include "testjig.hpp"
#include "calibrationstatus.hpp"

namespace gui
{
    TestJigTreeWidgetItem::TestJigTreeWidgetItem(testjig::TestJig* pJig) :
        QTreeWidgetItem(),
        m_pJig(pJig)
    {
        update();
    }

    testjig::TestJig* TestJigTreeWidgetItem::jig()
    {
        return m_pJig;
    }

    void TestJigTreeWidgetItem::update()
    {
        // Put the test jig name in first column
        setText(0, m_pJig->name());

        // Put the cable cal status in third column if this is a jig which supports cable cal
        common::CalibrationStatus* calStatus = dynamic_cast<common::CalibrationStatus*>(m_pJig);

        if (calStatus != 0)
        {
            setIcon(2, calStatus->calStatusIcon());
            setToolTip(2, calStatus->calStatusText());
        }
    }
}
