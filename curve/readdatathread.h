#ifndef READDATATHREAD_H
#define READDATATHREAD_H

#include <QObject>
#include <QImage>

#define UART_FRAME_HEAD1        0
#define UART_FRAME_HEAD2        1
#define UART_FRAME_CMD          2
#define UART_FRAME_QUERY_DATA   3
#define UART_FRAME_END1         4
#define UART_FRAME_END2         5

#define FRAME_HEAD1 0xAB
#define FRAME_HEAD2 0xBA
#define FRAME_TAIL1 0xCD
#define FRAME_TAIL2 0xDC

class QUdpSocket;
class ReadDataThread : public QObject
{
    Q_OBJECT
public:
    explicit ReadDataThread(QObject *parent = nullptr);

signals:
    void signal_clearGrayImage(bool stop);
    void signal_updateImage(const int row, QImage *image);
    void SigNewRpt(int pos,int data);

public slots:
    void slotStart();
    void slot_start_udp(QString ip, QString port);
    void slot_send_cmd(QString cmd);
    void slot_readyRead();

private:
    QUdpSocket *udpSocket;
    QString m_ip;
    QString m_port;
    QImage m_image;

    QByteArray m_RX_Data;
    int m_row;
    bool m_recvEnd;
    int m_pos;
};

#endif // READDATATHREAD_H
