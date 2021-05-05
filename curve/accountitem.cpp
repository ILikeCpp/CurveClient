#include "accountitem.h"
///////////////////////////////////////////////////////////////
//函 数 名 : AccountItem
//函数功能 :
//处理过程 : 无
//参数说明 :
//          bool bDele
//返 回 值 : 无
///////////////////////////////////////////////////////////////
AccountItem::AccountItem(QWidget *parent,bool bDele) :
    QWidget(parent)
{
    mouse_press = false;
    account_str = new QLabel();
    delede_button = new QPushButton();

    QPixmap pixmap(":/images/delete.png");
    delede_button->setIcon(pixmap);
    delede_button->setIconSize(pixmap.size());
    delede_button->setStyleSheet("background:transparent;");
    connect(delede_button, SIGNAL(clicked()), this, SLOT(removeAccount()));

    //account_str->setStyleSheet("font: 16pt");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(account_str);
    isdelete = false;
    if(bDele)
    {
        layout->addStretch();
        layout->addWidget(delede_button);
        isdelete = true;
    }
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(3);

    this->setLayout(layout);  
}

AccountItem::~AccountItem()
{

}

void AccountItem::setAccountStr(QString account_text)
{
    account_str->setText(account_text);
}

QString AccountItem::getAccountStr()
{
    QString account_tmp = account_str->text();
    return account_tmp;
}

void AccountItem::setDeleteViable(bool can)
{

}

bool AccountItem::isDeleteViable()
{
    return isdelete;
}

void AccountItem::removeAccount()
{
    QString account_tmp = account_str->text();
    emit sigremoveAccount(account_tmp);
}

void AccountItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    QWidget::mousePressEvent(event);
}

void AccountItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouse_press)
    {
        emit sigshowAccount(account_str->text());
        mouse_press = false;
    }
    QWidget::mouseReleaseEvent(event);
}



