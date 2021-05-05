#include "xcombobox.h"

XComboBox::XComboBox(QWidget * parent):
    QComboBox(parent)
{
    this->setEditable(true);
    this->setInsertPolicy(QComboBox::NoInsert);

    list_widget = new QListWidget(this);
    this->setModel(list_widget->model());
    this->setView(list_widget);

    connect(this,SIGNAL(editTextChanged(QString)),this,SLOT(textchanged(QString)));
    /*
    setStyleSheet("QComboBox{border:1px solid gray;}"
           "QComboBox QAbstractItemView::item{height:50px;}" //下拉选项高度
           );
    */
}

void XComboBox::addAccount(QString account,bool bDele)
{
    //qDebug()<<"account "<<account<<bDele;
    AccountItem *account_item = new AccountItem(this,bDele);
    account_item->setAccountStr(account);

    connect(account_item, SIGNAL(sigshowAccount(QString)), this, SLOT(showAccount(QString)));
    connect(account_item, SIGNAL(sigremoveAccount(QString)), this, SLOT(removeAccount(QString)));

    QListWidgetItem *list_item = new QListWidgetItem(list_widget);
    list_widget->setItemWidget(list_item, account_item);


    this->setCurrentIndex(list_widget->count());
}

void XComboBox::showAccount(QString account)
{
    list_widget->setCurrentRow(0);
    this->setEditText(account);

    this->hidePopup();
}

void XComboBox::removeAccount(QString account)
{

    QString tmp = this->currentText();
    int list_count = list_widget->count();
    for(int i=(list_count-1); i>=0; i--)
    {
        QString account_str = getAccountText(i);
        if(account == account_str)
        {
            delete list_widget->item(i);

            emit sighisDataChange(this->objectName());
            break;
        }
    }
    this->setEditText(tmp);
    this->showPopup();
}

int XComboBox::getAcCount()
{
    return list_widget->count();
}

QString XComboBox::getAccountText(int cnt)
{
    QString ret;
    int list_count = list_widget->count();
    if(( cnt >= 0)&&(cnt < list_count))
    {
        QListWidgetItem *item = list_widget->item(cnt);
        AccountItem *account_item = (AccountItem *)(list_widget->itemWidget(item));
        ret = account_item->getAccountStr();
    }
    return ret;
}

bool XComboBox::isequAccountText(QString str)
{
    bool ret = false;
    int list_count = list_widget->count();
    for(int i=0; i<list_count; i++)
    {
        QString account_str = getAccountText(i);
        if(str == account_str)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

bool XComboBox::isCountDefault(int cnt)
{
    bool ret = false;
    int list_count = list_widget->count();
    if(( cnt >= 0)&&(cnt < list_count))
    {

        QListWidgetItem *item = list_widget->item(cnt);
        AccountItem *account_item = (AccountItem *)(list_widget->itemWidget(item));
        ret = account_item->isDeleteViable();

    }
    return ret;
}

void XComboBox::keyPressEvent(QKeyEvent *e)
{

    if(e->key() == Qt::Key_Return)
    {
        if(!this->currentText().isEmpty()&&!isequAccountText(this->currentText()))
        {
            QString tmp = this->currentText();
            addAccount(this->currentText());
            this->setEditText(tmp);
            emit sighisDataChange(this->objectName());
        }
    }
    else
        QComboBox::keyPressEvent(e);
}

void XComboBox::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)
    {

    }
    else
        QComboBox::keyReleaseEvent(e);
}

void XComboBox::setHintNum(int num)
{
    this->setMaxVisibleItems(num);
}

void XComboBox::textchanged(QString account)
{

}

