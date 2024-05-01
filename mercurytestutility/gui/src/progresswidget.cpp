#include "progresswidget.hpp"

#include <QtWidgets>
#include <QApplication>

namespace gui
{
    ProgressWidget::ProgressWidget(QWidget* parent) :
        QGroupBox(tr("Progress Messages"), parent),
        m_noTextAdded(true)
    {
        setLayout(createLayout());
        hookUpSignals();
    }

    ProgressWidget::~ProgressWidget()
    {
    }

    QLayout* ProgressWidget::createLayout()
    {
        QVBoxLayout* pLayout = new QVBoxLayout;
        pLayout->addWidget(createTextEdit());

        return pLayout;
    }

    QWidget* ProgressWidget::createTextEdit()
    {
        m_pTextEdit = new QTextEdit;

        m_pTextEdit->setReadOnly(true);
        m_pTextEdit->setBackgroundRole(QPalette::Background);
        m_pTextEdit->setForegroundRole(QPalette::Text);
        m_pTextEdit->append(tr("Progress messages appear here\n"));
        m_pTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);

        return m_pTextEdit;
    }

    void ProgressWidget::hookUpSignals()
    {
        connect(m_pTextEdit, SIGNAL(customContextMenuRequested(QPoint)),
                this,          SLOT(contextMenu(QPoint)));
    }

    void ProgressWidget::contextMenu(const QPoint& pos)
    {
        QPoint globalPos = m_pTextEdit->mapToGlobal(pos);

        QMenu context;

        context.addAction(tr("Copy"), m_pTextEdit, SLOT(copy()));
        context.addAction(QIcon(":/icons/16x16/edit-copy-4.png"), tr("Copy All"), this, SLOT(copyAll()));
        context.addSeparator();
        context.addAction(QIcon(":/icons/16x16/edit-clear-2.png"), tr("Clear All"), m_pTextEdit, SLOT(clear()));

        context.exec(globalPos);
    }

    void ProgressWidget::clear()
    {
        m_pTextEdit->clear();
    }

    void ProgressWidget::copyAll()
    {
        QString text = m_pTextEdit->toPlainText();

        if (text != "")
        {
            QApplication::clipboard()->setText(text);
        }
    }

    void ProgressWidget::append(const QString& text)
    {
        // If no text has been added yet then clear out the initial message
        if (m_noTextAdded)
        {
            clear();
            m_noTextAdded = false;
        }

        m_pTextEdit->append(text);

        // Call repaint so that the message appears immediately. The Qt documentation recommends
        // using update() rather than repaint() but this progress box informs the user of activity.
        // When VISA equipment is being manipulated some operations can take seconds and in that time
        // the main event loop is held up - these progress messages help keep the application looking
        // alive.
        m_pTextEdit->repaint();

        // Move slider to bottom
        m_pTextEdit->verticalScrollBar()->setSliderPosition(
                    m_pTextEdit->verticalScrollBar()->maximum());
    }
}
