#include "setdialog.h"
#include "ui_setdialog.h"

CSetDialog::CSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetDialog)
{
    QString str;
    ui->setupUi(this);
    this->setWindowTitle(tr("设置"));
    QSettings settings("WEIZHENG", "hospt");
    str = settings.value("title",QVariant(tr("检验报告单"))).toString();
    ui->m_LE_Title->setText(str);
    str = settings.value("save_url",QVariant(tr("/"))).toString();
    ui->m_LE_URL->setText(str);
    int num = settings.value("history_num",QVariant(tr("3"))).toInt();
    ui->m_CB_History->setCurrentIndex(num);
    num = settings.value("save_name1",QVariant(tr("0"))).toInt();
    ui->m_CB_Name1->setCurrentIndex(num);
    num = settings.value("save_name2",QVariant(tr("1"))).toInt();
    ui->m_CB_Name2->setCurrentIndex(num);
    num = settings.value("page",QVariant(tr("0"))).toInt();
    ui->m_CB_Page->setCurrentIndex(num);
    num = settings.value("unit",QVariant(tr("0"))).toInt();
}

CSetDialog::~CSetDialog()
{
    delete ui;
}

QString CSetDialog::GetTitle()
{
    return ui->m_LE_Title->text();
}

int CSetDialog::GetHistoryNum()
{
    return ui->m_CB_History->currentIndex();
}

QString CSetDialog::GetSaveURL()
{
    return ui->m_LE_URL->text();
}

int CSetDialog::GetNameStye()
{
    return ui->m_CB_Name1->currentIndex()*10+ui->m_CB_Name2->currentIndex();
}

QString CSetDialog::GetPageStye()
{
    return ui->m_CB_Page->currentText();
}

QString CSetDialog::GetUnitStye()
{
    return "";
}

void CSetDialog::on_btnOK_clicked()
{   
    QSettings settings(" ", " ");
    settings.setValue("title",QVariant(ui->m_LE_Title->text()));
    settings.setValue("save_url",QVariant(ui->m_LE_URL->text()));
    settings.setValue("history_num",QVariant(ui->m_CB_History->currentIndex()));
    settings.setValue("save_name1",QVariant(ui->m_CB_Name1->currentIndex()));
    settings.setValue("save_name2",QVariant(ui->m_CB_Name2->currentIndex()));
    settings.setValue("page",QVariant(ui->m_CB_Page->currentIndex()));

    this->accept();
}


void CSetDialog::on_btnNO_clicked()
{
    this->reject();
}

void CSetDialog::on_btnCliHint_clicked()
{
    emit this->SigCliHintData();
}

void CSetDialog::on_btnChoseUrl_clicked()
{
    QString tempName = "./";
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        "./",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if(!fileName.isEmpty())
        ui->m_LE_URL->setText(fileName);
}

void CSetDialog::on_m_CB_Name1_currentIndexChanged(int index)
{
    int cid = ui->m_CB_Name2->currentIndex();
    if(cid == index)
    {
        if(cid == (ui->m_CB_Name2->count()-1))
            ui->m_CB_Name2->setCurrentIndex(1);
        else
            ui->m_CB_Name2->setCurrentIndex(cid+1);
    }
}

void CSetDialog::on_m_CB_Name2_currentIndexChanged(int index)
{
    int cid = ui->m_CB_Name1->currentIndex();
    if(cid == index)
    {
        if(cid == (ui->m_CB_Name1->count()-1))
            ui->m_CB_Name1->setCurrentIndex(0);
        else
            ui->m_CB_Name1->setCurrentIndex(cid+1);
    }
}



