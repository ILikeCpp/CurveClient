#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QDebug>


class listwidget : public QListWidget
{
    Q_OBJECT
public:
    explicit listwidget(QWidget *parent = 0);
signals:
    void SigMouseClick(int row);
    void SigMouseClickOne(int row);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent * event );

};

#endif // LISTWIDGET_H
