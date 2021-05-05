#include "SerialPortDialog.h"
#include "ui_SerialPortDialog.h"
#include <QMessageBox>
#include <QDesktopWidget>

CSerialPortDialog::CSerialPortDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::CSerialPortDialog),
    m_pSerialPort(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("串口设置"));

    //m_pSerialPort = new QextSerialPort();

    m_bOpen = false;
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

}

CSerialPortDialog::~CSerialPortDialog()
{
    delete ui;
    if (m_pSerialPort)
    {
        m_pSerialPort->flush();
        m_pSerialPort->close();
        delete m_pSerialPort;
        m_pSerialPort = nullptr;
    }
}

void CSerialPortDialog::showEvent(QShowEvent *event)
{
    QString path="HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
    QSettings *settings=new QSettings(path,QSettings::NativeFormat);
    QStringList key=settings->allKeys();
    QStringList comlist ;
    comlist.clear();

    for(int i=0;i<key.size();i++)
    {
        comlist << GetComm(i,"value");
    }

    qDebug() << comlist;
    ui->BoxCKH->clear();
    ui->BoxCKH->addItems(comlist);
}
QSerialPort * CSerialPortDialog::GetSerialPort(bool *bOpen)
{
    *bOpen = this->m_bOpen;
    return this->m_pSerialPort;
}

void CSerialPortDialog::readDataCom()
{
    qDebug() << "DataCom";

}

void CSerialPortDialog::on_btnEnter_clicked()
{
    //m_pSerialPort->close();
    //m_pSerialPort->setPortName(this->ui->BoxCKH->currentText()); //2
    //m_pSerialPort->setQueryMode(QextSerialBase::EventDriven);
//    m_pSerialPort = new QextSerialPort(this->ui->BoxCKH->currentText(),QextSerialBase::EventDriven);
    m_pSerialPort = new QSerialPort(this->ui->BoxCKH->currentText());

//    BaudRateType baudRate;
//    QString tem = this->ui->BoxBRT->currentText();
//    if(tem == QString("9600")) baudRate = BAUD9600;
//    else if(tem == QString("19200")) baudRate = BAUD19200;
//    else if(tem == QString("38400")) baudRate = BAUD38400;
//    else if(tem == QString("57600")) baudRate = BAUD57600;
//    else if(tem == QString("115200")) baudRate = BAUD115200;
//    else if(tem == QString("1382400")) baudRate = BAUD1382400;
//    else if(tem == QString("921600")) baudRate = BAUD921600;
//    else if(tem == QString("460800")) baudRate = BAUD460800;
//    else if(tem == QString("256000")) baudRate = BAUD256000;
//    else if(tem == QString("230400")) baudRate = BAUD230400;
//    else if(tem == QString("128000")) baudRate = BAUD128000;
//    else if(tem == QString("76800")) baudRate = BAUD76800;
//    else if(tem == QString("43000")) baudRate = BAUD43000;
//    else if(tem == QString("14400")) baudRate = BAUD14400;
//    else if(tem == QString("4800")) baudRate = BAUD4800;
//    else if(tem == QString("2400")) baudRate = BAUD2400;
//    else if(tem == QString("1200")) baudRate = BAUD1200;
//    else baudRate = BAUD9600;


    m_pSerialPort->setBaudRate(this->ui->BoxBRT->currentText().toInt());
    //波特率设置，我们设置为9600

    m_pSerialPort->setDataBits((QSerialPort::DataBits)this->ui->BoxSJW->currentText().toInt());
    //数据位设置，我们设置为8位数据位

    if ("None" == this->ui->BoxJYW->currentText())
    {
        m_pSerialPort->setParity((QSerialPort::NoParity));
        //奇偶校验设置，我们设置为无校验
    }
    else if ("Odd" == this->ui->BoxJYW->currentText())
    {
        m_pSerialPort->setParity((QSerialPort::OddParity));
        //奇偶校验设置，我们设置为无校验
    }
    else if ("Event" == this->ui->BoxJYW->currentText())
    {
        m_pSerialPort->setParity((QSerialPort::EvenParity));
        //奇偶校验设置，我们设置为无校验
    }
    else if ("Mark" == this->ui->BoxJYW->currentText())
    {
        m_pSerialPort->setParity((QSerialPort::MarkParity));
        //奇偶校验设置，我们设置为无校验
    }
    else if ("Space" == this->ui->BoxJYW->currentText())
    {
        m_pSerialPort->setParity((QSerialPort::SpaceParity));
        //奇偶校验设置，我们设置为无校验
    }

    m_pSerialPort->setStopBits((QSerialPort::StopBits)this->ui->BoxTZW->currentText().toInt());
    //停止位设置，我们设置为1位停止位

    m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    //数据流控制设置，我们设置为无数据流控制

//    m_pSerialPort->setTimeout(10);
    //延时设置，我们设置为延时10ms

    m_bOpen = m_pSerialPort->open(QIODevice::ReadWrite);//以读写方式打开串口
    if(m_bOpen == false)
    {
        qDebug() << "error==="<<m_pSerialPort->errorString();
        QMessageBox::about(this, tr("error"), tr("串口打开错误"));

        this->reject();
        return ;
    }

    //connect(m_pSerialPort,SIGNAL(readyRead()),this,SLOT(readDataCom()));
    m_pSerialPort->flush();
    qDebug() << this->ui->BoxCKH->currentText() << "open success";
    this->accept();

}

void CSerialPortDialog::on_btnCancle_clicked()
{
    this->reject();
}

void CSerialPortDialog::CloseSerialPort()
{
    m_pSerialPort->close();
}

QString CSerialPortDialog::GetComm(int index,QString keyorvalue)
{
    wchar_t subkey[80];
    wchar_t keyname[256]; //键名数组
    char keyvalue[256];  //键值数组
    HKEY hKey;
    QString commresult="";
    QString strkey="HARDWARE\\DEVICEMAP\\SERIALCOMM";//子键路径
    int a=strkey.toWCharArray(subkey);
    subkey[a]=L'\0';
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ|KEY_QUERY_VALUE,&hKey)!=0)
    {
        QString error="Cannot open regedit!";
    }

    QString keymessage="";//键名
    QString message="";
    QString valuemessage="";//键值

    int indexnum=index;//要读取键值的索引号
    DWORD keysize=sizeof(keyname);
    DWORD valuesize=sizeof(keyvalue);
    DWORD type;
    if(::RegEnumValue(hKey,indexnum,keyname,&keysize,0,&type,(BYTE*)keyvalue,&valuesize)==0)
    {
        //读取键名
        for(int i=0;i<keysize;i++)
        {
            message=QString::fromStdWString(keyname);
            keymessage.append(message);
        }
        //读取键值
        for(int j=0;j<valuesize;j++)
        {
            if(keyvalue[j]!=0x00)
            {
                valuemessage.append(keyvalue[j]);
            }
        }

        if(keyorvalue=="key")
        {
            commresult=keymessage;
        }
        if(keyorvalue=="value")
        {
            commresult=valuemessage;
        }
    }
    else
    {
        commresult="nokey";
    }
    ::RegCloseKey(hKey);//关闭注册表
    return commresult;
}



