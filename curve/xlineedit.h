#ifndef XLINEEDIT_H
#define XLINEEDIT_H

#include <QtGui>
#include"QLineEdit"
#include <QPushButton>
#include <QCompleter>
class CXlineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CXlineEdit(QWidget * parent = 0 );
    void enterEvent( QEvent * e);
    void leaveEvent( QEvent * e);
    void addHintStrings(QStringList strlis);
    void cliHintStrings();
    void setHintNum(int num);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
private slots:
    void SlotCliText();
private:
    QPushButton *m_pbCli;
    QCompleter *completer;
};

#endif // XLINEEDIT_H
