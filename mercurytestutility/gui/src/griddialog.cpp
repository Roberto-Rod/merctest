#include "griddialog.hpp"

#include <QtWidgets>

namespace gui
{
    GridDialog::GridDialog(QString title, int rows, int cols, QWidget* parent) :
        ModalDialog(title, parent),
        m_rows(rows),
        m_cols(cols),
        m_grid(0)
    {
        setLayout(createLayout());
    }

    bool GridDialog::setItem(QString text, int row, int col)
    {
        bool retVal = false;

        if ((row < m_rows) && (col < m_cols))
        {
            retVal = setItem(new QLabel(text), row, col);
        }

        return retVal;
    }

    bool GridDialog::setItem(QWidget* pWidget, int row, int col)
    {
        bool retVal = false;

        if ((row < m_rows) && (col < m_cols))
        {
            // Remove the old item and delete the object
            QLayoutItem* item = m_grid->itemAtPosition(row, col);
            m_grid->removeItem(item);
            delete item;

            // Add the new item
            m_grid->addWidget(pWidget, row, col);

            // Reset the size
            adjustSize();

            // Repaint the widget
            qApp->processEvents();
            repaint();
            qApp->processEvents();

            retVal = true;
        }

        return retVal;
    }

    QLayout* GridDialog::createLayout()
    {
        // Create the grid layout
        m_grid = new QGridLayout;

        for (int row = 0; row < m_rows; row++)
        {
            for (int col = 0; col < m_cols; col++)
            {
                m_grid->addWidget(new QLabel(QString("    ").arg(row).arg(col)), row, col);
            }
        }

        // Create a top level vertical layout
        QVBoxLayout* layout = new QVBoxLayout;

        // Add the grid layout
        layout->addLayout(m_grid);

        return layout;
    }
}
