#ifndef GRIDDIALOG_HPP
#define GRIDDIALOG_HPP

#include "modaldialog.hpp"

QT_BEGIN_INCLUDE_NAMESPACE
class QGridLayout;
class QLayoutItem;
QT_END_INCLUDE_NAMESPACE

namespace gui
{
    class GridDialog : public ModalDialog
    {
        Q_OBJECT
    public:
        GridDialog(QString title, int rows, int cols, QWidget* parent = 0);

        bool setItem(QString text, int row, int col);

        bool setItem(QWidget* pWidget, int row, int col);

    private:
        int m_rows;
        int m_cols;
        QGridLayout* m_grid;

        QLayout* createLayout();
    };
}

#endif // GRIDDIALOG_HPP
