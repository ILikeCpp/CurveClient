#include "readdatathread.h"
#include <QFile>
#include <QDebug>
#include <QUdpSocket>
#include <QPixmap>
#include <QDateTime>
#include <QTimer>

ReadDataThread::ReadDataThread(QObject *parent) :
    QObject(parent)
{
    m_recvEnd = false;
    m_row = 0;
    m_pos = 0;
    QPixmap pix(1420,4867);
    QColor color("white");
    pix.fill(color);
    m_image = pix.toImage();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]{
        if (m_RX_Data.size() >= 1420) {
            QByteArray bytes = m_RX_Data.left(1420);
            m_RX_Data.remove(0,1420);

            for(int i = 0; i < bytes.size(); i++)
            {
                int hex = bytes.at(i);
//                qDebug() << hex;
                m_image.setPixel(i,m_row,qRgb(hex,hex,hex));

                emit SigNewRpt(m_pos++,hex);
            }

            m_row++;

            emit signal_updateImage(m_row, &m_image);
        }
        else {
            //没数据了，并且接收到结束指令
            if (m_recvEnd) {
                QString file = QString("image_%1.bmp")
                        .arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
                m_image.save(file,"BMP");

                m_row = 0;
                m_pos = 0;
                QPixmap pix(1420,4867);
                QColor color("white");
                pix.fill(color);
                m_image = pix.toImage();
                emit signal_clearGrayImage(true);
                m_recvEnd = false;
            }
        }

    });

    timer->start(15);
}

void ReadDataThread::slotStart()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(23333);

    connect(udpSocket, &QUdpSocket::readyRead, this,&ReadDataThread::slot_readyRead);
//    connect(udpSocket, &QUdpSocket::readyRead, this, [=]{
//        static int row = 0;
//        if (udpSocket->bytesAvailable() == 12)
//        {
//            QByteArray recv = udpSocket->readAll();
//            if (QString(recv) == "@@@@FFFF####")//表示开始
//            {
//                row = 0;
//                emit signal_clearGrayImage();
//            }
//            else if (QString(recv) == "@@@@!!!!####")//表示结束
//            {
//                row = 0;
//                emit signal_clearGrayImage();
//            }
//        }
//        else
//        {
//            while (udpSocket->bytesAvailable() >= 1420)//图片数据
//            {
//                QByteArray recv = udpSocket->read(1420);
//                QList<int> grayList;
//                for(int i = 0; i < recv.size(); i++)
//                {
//                    int hex = recv.at(i);
////                    qDebug() << hex;
//                    grayList << hex;
//                }

//                row++;

//                emit signal_updateImage(row, grayList);
//                qDebug() << "emit ...";
//            }
//        }
//    });
}

void ReadDataThread::slot_start_udp(QString ip, QString port)
{
    m_ip = ip;
    m_port = port;
    udpSocket->connectToHost(QHostAddress(ip), port.toUInt());
    udpSocket->waitForConnected();
    QByteArray data("start");
    udpSocket->write(data);
}

void ReadDataThread::slot_send_cmd(QString cmd)
{
    int num = cmd.toInt(nullptr,16);
    qDebug() << "num="<<num;
    QByteArray data;
    data.append(num);

    udpSocket->writeDatagram(data, QHostAddress(m_ip), m_port.toUInt());
}

void ReadDataThread::slot_readyRead()
{
    QByteArray rxdata;
    static quint8 state = UART_FRAME_HEAD1;
    static quint8 RX_Data_Cmd = 0;
    static QByteArray RX_Data_Data;

    if(udpSocket->bytesAvailable() > 0) //表示接收缓冲区内有数据
    {
        rxdata = udpSocket->readAll();

        for (int k = 0; k < rxdata.size(); k++)
        {
            switch (state)
            {
            case UART_FRAME_HEAD1://读取帧头1  1字节
            {
                if (rxdata[k] == char(FRAME_HEAD1))
                {
                    state = UART_FRAME_HEAD2;
                }
            }
                break;
            case UART_FRAME_HEAD2://读取帧头2  1字节
            {
                if (rxdata[k] == char(FRAME_HEAD2))
                {
                    state = UART_FRAME_CMD;
                }
            }
                break;
            case UART_FRAME_CMD://读取指令 1字节
            {
                RX_Data_Cmd = rxdata[k];
                if (0x02 == RX_Data_Cmd) {
                    state = UART_FRAME_QUERY_DATA;
                }
                else {
                    state = UART_FRAME_END1;
                }
            }
                break;
            case UART_FRAME_QUERY_DATA://读取数据内容
            {
                RX_Data_Data.append(rxdata[k]);
                if (RX_Data_Data.size() == 1420)//数据读取完毕
                {
                    state = UART_FRAME_END1;
                }
            }
                break;
            case UART_FRAME_END1://读取帧尾1
            {
                if (rxdata[k] == char(FRAME_TAIL1)) {
                    state = UART_FRAME_END2;
                }
            }
                break;
            case UART_FRAME_END2://读取帧尾2
            {
                if (rxdata[k] == char(FRAME_TAIL2))//包头包尾，都正常，后面校验累计和
                {
                    //处理命令
                    switch (RX_Data_Cmd) {
                    case 0x01://开始
                    {
                        m_recvEnd = false;
                        m_row = 0;
                        QPixmap pix(1420,4867);
                        QColor color("white");
                        pix.fill(color);
                        m_image = pix.toImage();
                        emit signal_clearGrayImage(false);
                    }
                        break;
                    case 0x02://图片数据
                    {
                        m_RX_Data.append(RX_Data_Data);
                    }
                        break;
                    case 0x03://结束
                    {
                        m_recvEnd = true;
//                        QString file = QString("image_%1.bmp")
//                                .arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
//                        m_image.save(file,"BMP");

//                        m_row = 0;
//                        QPixmap pix(1420,4867);
//                        QColor color("white");
//                        pix.fill(color);
//                        m_image = pix.toImage();
//                        emit signal_clearGrayImage(true);
                    }
                        break;
                    default:
                        break;
                    }
                }

                //重置参数
                state = UART_FRAME_HEAD1;
                RX_Data_Cmd = 0;
                RX_Data_Data.clear();
            }
                break;
            }
        }
    }
}
