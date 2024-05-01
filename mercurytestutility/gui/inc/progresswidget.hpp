#ifndef PROGRESSWIDGET_HPP
#define PROGRESSWIDGET_HPP

#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

namespace gui
{
    class ProgressWidget : public QGroupBox
    {
        Q_OBJECT

    public:
        ProgressWidget(QWidget* parent = 0);
        ~ProgressWidget();

    signals:

    public slots:
        void clear();
        void copyAll();
        void append(const QString& text);

    private slots:
        void contextMenu(const QPoint &pos);

    private:
        QLayout* createLayout();
        QWidget* createTextEdit();
        void hookUpSignals();

        QTextEdit* m_pTextEdit;
        bool       m_noTextAdded;
    };
}

#endif // PROGRESSWIDGET_HPP
