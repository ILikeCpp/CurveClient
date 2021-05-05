#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H
#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
class AccountItem : public QWidget
{
    Q_OBJECT
public:
    explicit AccountItem(QWidget *parent = 0,bool bDele = true);
    ~AccountItem();
    void setAccountStr(QString account_text);
    QString getAccountStr();
    void setDeleteViable(bool can);
    bool isDeleteViable();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void sigremoveAccount(QString text);
    void sigshowAccount(QString text);

public slots:
    void removeAccount();
private:
    bool mouse_press;
    QLabel *account_str;
    QPushButton *delede_button;
    bool isdelete;

};

#endif // ACCOUNTITEM_H
