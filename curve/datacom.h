#ifndef DATACOM_H
#define DATACOM_H

#include <QObject>
#include <QThread>

#include <stdio.h>
#include <QByteArray>
#include <QTimer>
#include <QtCore>
#include <QObject>

#include "packet_def.h"
#include "SerialPort/SerialPortDialog.h"

#include <QSerialPort>

#define FRAME_MAX_LEN (64)


typedef enum ComType //通讯类型
{
    COM_NONE = 0x00000000, //
    COM_CNT = 0x00000001, //连接信号
    COM_HBT = 0x00000002, //心跳信号
    COM_GET = 0x00000010, //获取类型
    COM_SET = 0x00000020, //设置类型
    COM_CMD = 0x00000100, //命令模式
    COM_UPG = 0x00000200, //升级模式
    COM_RPT = 0x00001000  //提交报告
}COM_TYPE;

typedef enum DataType //数据类型
{
    DATA_NONE = 0x00000000, //

    DATA_FONT = 0x00000001, //下载字体
    CLR_FONT = 0x00000002, //清空字体
    CLR_TEST = 0x00000003, //

    DATA_BREK = 0x10000000, //连接
    DATA_CNNT = 0x20000000, //断开
    DATA_DERR = 0x40000000, //数据出错
    DATA_SUCC = 0x80000000  //成功
}DATA_TYPE;

typedef struct DelPackage
{
    int com;              //类型
    int pos;              //地址
    int num;              //个数
}DEL_PACKAGE;

typedef struct _pro_res_
{
    QString name;           //项目
    QString result;         //浓度值
    QString unit;           //单位
    QString refer;          //参考值
}PRO_RES;

typedef struct _res_data_
{
    QString swinum;        //流水号
    QString batch;         //批号
    int unite;             //联卡
    PRO_RES pro[6];
    QDateTime endtime;     //测试结束时间
    QByteArray code;       //条码
}RES_DATA;

typedef struct DataCom
{
    bool can_send;
    int head;               //包头
    int comT;               //通信类型
    int dataT;              //数据类型
    int packN;              //数据包总数
    int currN;              //当前包号数
    int data_length;        //当前包长度
    QByteArray data;        //数据
    int CRC_BT;             //校验值
    int end;                //包结束
}DATA_COM;

typedef struct File_Struct
{
    int name_len;
    QByteArray file_name;
    int data_len;
    QByteArray file_data;
    QByteArray file_MD5;
}FILE_ST;

typedef struct Send_Struct
{
    COM_TYPE comT;
    DATA_TYPE dataT;
    QByteArray data;
}SEND_ST;




class CDataCom : public QThread
{
    Q_OBJECT
public:
    typedef enum
    {
        COM_RUN,
        WAIT_RUN
    }RUNTYPE;
    explicit CDataCom(QObject *parent=0);
    ~CDataCom();
    void run();

    QByteArray  intToByte(int i);
    int ByteToint(QByteArray ba);

    bool anaRecvData(QByteArray *data);//分析接收数据

    bool isBuildCommt();
    bool ConnectToSerPort();
    bool BreakToSerPort();

    bool anaHistData(QByteArray &data,RES_DATA *hist);//解析接收到的历史数据
    int getCRC_BT(DATA_COM *data);//获取校验码
    bool SendData( COM_TYPE comT, DATA_TYPE dataT,const QByteArray & data = QByteArray());
    bool orgSendData(DATA_COM *send,QByteArray *data, COM_TYPE comT, DATA_TYPE dataT,int packn=1,int currn=1);//组织发送数据
    bool subPackageData(QList<DATA_COM> *buf,SEND_ST &ts);//分包
    bool DataToSend(DATA_COM &data, QByteArray *send);
    bool sendgetres();
    bool sendgetcure();
    bool sendstart(int time);
    bool sendstop();
    bool sendsetledcunt(int gree,int uv,int white);
    bool sendsetledtime(int type,int frq,int smd,int dly,int smp);
    bool sendsetgainidx(int type,int cap,int res,int gain,int cunt);
    bool sendsetmtpos(bool dir,int pos);
    bool sendsetsmpinfo(bool dir,int type,int dly,int num,int dis);

    void stopThread();

private slots:

    void SlotRdLoop(); // 接收数据
    void SlotWdLoop();
signals:
    void SigCnntSucc(bool bsucc); //连接信号
    void SigSysPBStatus(int prc);
    void SigNewRpt(int pos,int data);

private:

    bool b_COMConnect;//连接标志

    QByteArray m_Dataend;

//    QextSerialPort * m_pSerialPort;

    QSerialPort *m_pSerialPort;

    QByteArray m_qByData;//串口接收缓冲器

    QTime m_beforetime;

    QStringList m_res_data;

    QList<SEND_ST> m_qSdData;//待发送数据

    QList<DATA_COM> m_qSdBuff;
    int curr_pos;

    bool bStop;
};

#endif // DATACOM_H
