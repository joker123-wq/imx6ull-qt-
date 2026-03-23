#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QGridLayout>

class QTextEdit;

class KeyboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardWidget(QWidget *parent = nullptr);

    void setTarget(QTextEdit *edit);

private:
    QTextEdit *m_target;

    void createButton(const QString &text, QGridLayout *layout, int row, int col);
};

#endif
