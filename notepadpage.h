#ifndef NOTEPADPAGE_H
#define NOTEPADPAGE_H

#include <QWidget>

class QTextEdit;
class QLabel;
class KeyboardWidget;

class NotepadPage : public QWidget
{
    Q_OBJECT

public:
    explicit NotepadPage(QWidget *parent = nullptr);

private slots:
    void saveText();
    void clearText();

private:
    QTextEdit *m_textEdit;
    QLabel *m_statusLabel;
    KeyboardWidget *m_keyboard;
};

#endif // NOTEPADPAGE_H
