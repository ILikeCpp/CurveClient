#include "aboutdialog.h"
#include "ui_aboutdialog.h"

CAboutDialog::CAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("关于 V20191016"));
    //ui->m_LB_Ver->setText(VERSIONS);
}

CAboutDialog::~CAboutDialog()
{
    delete ui;
}
