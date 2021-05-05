#include "listwidget.h"

listwidget::listwidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setLineWidth(21.75);
}

void listwidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    //如果单击了就触发clicked信号
    if(event->button() == Qt::LeftButton)
    {
        emit SigMouseClick(this->currentRow());
    }
    //将该事件传给父类处理
    else
        QListWidget::mouseDoubleClickEvent(event);
}

void listwidget::mousePressEvent(QMouseEvent *event)
{
    //如果单击了就触发clicked信号
    if(event->button() == Qt::RightButton)
    {
       emit SigMouseClickOne(this->currentRow());
    }
    else
        QListWidget::mousePressEvent(event);//将该事件传给父类处理
}


