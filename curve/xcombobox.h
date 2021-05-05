#ifndef XCOMBOBOX_H
#define XCOMBOBOX_H

#include <QtGui>
#include <QComboBox>
#include <QListWidget>
#include "accountitem.h"
class XComboBox : public QComboBox
{
    Q_OBJECT
public:
    XComboBox(QWidget * parent = 0 );
    void addAccount(QString account,bool bDele = true);
    int getAcCount();
    QString getAccountText(int cnt);
    bool isequAccountText(QString str);
    bool isCountDefault(int cnt);
    void setHintNum(int num);
signals:
    void sighisDataChange(QString str);
protected:
    void keyPressEvent( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
private slots:
    void showAccount(QString account);
    void removeAccount(QString account);
    void textchanged(QString account);
private:
    QListWidget *list_widget;
};

#endif // XCOMBOBOX_H
