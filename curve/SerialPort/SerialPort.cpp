#include "SerialPort.h"
#include <QtGui>
#include <QString>

QextSerialPort * CSerialPort::s_pUart0 = new QextSerialPort();
QextSerialPort * CSerialPort::s_pUart1 = new QextSerialPort();
QextSerialPort * CSerialPort::s_pUart2 = new QextSerialPort();
QextSerialPort * CSerialPort::s_pUart3 = new QextSerialPort();


CSerialPort::CSerialPort()
{
}


// 获得串口1指针
QextSerialPort * CSerialPort::GetSerialPort(UART_TYPE eType)
{
    switch(eType)
    {
    case UART_0_SCAN_GUN:      return s_pUart0; break;
    case UART_1_SCAN_MODEULE:  return s_pUart1; break;
    case UART_2_PC_PRINTER:    return s_pUart2; break;
    case UART_3_430:           return s_pUart3; break;
    default:
        break;
    }

    return NULL;
}

bool CSerialPort::OpenSerialPort(UART_TYPE eType)
{
    bool bOpen = false;
    QextSerialPort * pUart;

    switch(eType)
    {
    case UART_0_SCAN_GUN:  // 串口0扫描枪 串口1条形码扫描模块 串口2打印机和PC 串口3单片机430采集模块
    {
#ifdef VER_WINDOWS
        s_pUart0->setPortName("COM1");
#else
        s_pUart0->setPortName("/dev/ttyS0");
#endif
        pUart = s_pUart0;
        break;
    }
    case UART_1_SCAN_MODEULE:
    {
#ifdef VER_WINDOWS
        s_pUart1->setPortName("COM3");
#else
        s_pUart1->setPortName("/dev/ttyS1");
#endif
        pUart = s_pUart1;
        break;
    }
    case UART_2_PC_PRINTER:
    {
#ifdef VER_WINDOWS
        s_pUart2->setPortName("COM4");
#else
        s_pUart2->setPortName("/dev/ttyS2");
#endif
        pUart = s_pUart2;
        break;
    }
    case UART_3_430:
    {
#ifdef VER_WINDOWS
        s_pUart3->setPortName("COM2"); //2
#else
        s_pUart3->setPortName("/dev/ttyS3");
#endif
        pUart = s_pUart3;
        break;
    }
    default:
        break;
    }

    pUart->setQueryMode(QextSerialBase::Polling);

    bOpen = pUart->open(QIODevice::ReadWrite);
    //以读写方式打开串口

    pUart->setBaudRate(BAUD115200);
    //波特率设置，我们设置为9600

    pUart->setDataBits(DATA_8);
    //数据位设置，我们设置为8位数据位

    pUart->setParity(PAR_NONE);
    //奇偶校验设置，我们设置为无校验

    pUart->setStopBits(STOP_1);
    //停止位设置，我们设置为1位停止位

    pUart->setFlowControl(FLOW_OFF);
    //数据流控制设置，我们设置为无数据流控制

    pUart->setTimeout(10);
    //延时设置，我们设置为延时10ms

#if 0
    int nEnter = 10; // 打印机回车指令
    QString str;
    str.append("荧光免疫层析检验报告单");
    s_pUart1->write((char *)&nEnter);
    s_pUart1->write(str.toLocal8Bit());
#endif

    return bOpen;
}
