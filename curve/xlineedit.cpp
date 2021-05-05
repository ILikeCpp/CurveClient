#include "xlineedit.h"
#include"QDebug"
CXlineEdit::CXlineEdit(QWidget * parent):
    QLineEdit (parent)
{
    m_pbCli = new QPushButton(this);
    m_pbCli->hide();
    m_pbCli->setText("x");
    m_pbCli->setStyleSheet("background-color:transparent;font: 16pt");
    connect(m_pbCli, SIGNAL(clicked()), this, SLOT(SlotCliText()));

    completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setModelSorting(QCompleter::UnsortedModel);
    this->setCompleter(completer);
}
void CXlineEdit::enterEvent(QEvent *e)
{
    //qDebug()<<"enterEvent"<<this->maxLength();
    m_pbCli->setGeometry(this->geometry().width()- this->geometry().height()+10,0,
                         this->geometry().height()-10,this->geometry().height()-3);
    m_pbCli->show();
}
void CXlineEdit::leaveEvent(QEvent *e)
{
    //qDebug()<<"leaveEvent";
    m_pbCli->hide();
}

void CXlineEdit::addHintStrings(QStringList strlis)
{
    strlis.removeLast();
    completer->setModel(new QStringListModel(strlis, this));
}

void CXlineEdit::cliHintStrings()
{
    completer->setModel(new QStringListModel( this));
}

void CXlineEdit::setHintNum(int num)
{
    completer->setMaxVisibleItems(num);
}
void CXlineEdit::SlotCliText()
{
    this->clear();
    this->setText("");
}
void CXlineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    //如果单击了就触发clicked信号
    if (event->button() == Qt::LeftButton)
    {
          completer->complete();
    }
    //将该事件传给父类处理
    QLineEdit::mouseDoubleClickEvent(event);
}
