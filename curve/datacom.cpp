#include "datacom.h"

CDataCom::CDataCom(QObject *parent):
    QThread(parent)
{
    m_Dataend.append("OK");
    m_beforetime = QTime::currentTime();
    curr_pos = 0;
    bStop = false;
}

CDataCom::~CDataCom()
{
    qDebug() << "CDataCom::~CDataCom";
    stopThread();
}

void CDataCom::run()
{
    while(1 && !bStop)
    {
        QThread::usleep(1000);
        SlotRdLoop();
        SlotWdLoop();

    }
    qDebug() << "==run quit===";
}



QByteArray  CDataCom::intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}
int CDataCom::ByteToint(QByteArray ba)
{
    int it=0;
    for(int i=3;i>=0;i--)
    {
        it<<=8;
        it+=(ba.at(i))&0xff;
    }
    return it;
}



int CDataCom::getCRC_BT(DATA_COM *data)
{
    int recrc=0;
    QByteArray tmpData;
    tmpData.append(intToByte(data->comT));
    tmpData.append(intToByte(data->dataT));
    tmpData.append(intToByte(data->packN));
    tmpData.append(intToByte(data->currN));
    tmpData.append(intToByte(data->data_length));
    tmpData.append(data->data);
    recrc = qChecksum(tmpData.constData(),tmpData.length());
    return recrc;
}

bool CDataCom::SendData(COM_TYPE comT, DATA_TYPE dataT,const QByteArray &data)
{
    //if(!b_COMConnect) return false;

    SEND_ST tmp;
    tmp.comT = comT;
    tmp.dataT = dataT;
    tmp.data.append(data);
    m_qSdData.append(tmp);

    return true;
}

bool CDataCom::orgSendData(DATA_COM *send,QByteArray *data, COM_TYPE comT, DATA_TYPE dataT,int packn,int currn)
{
    send->can_send = false;
    send->head = START_PCK_STATE;
    send->comT = comT;
    send->dataT = dataT;
    send->packN = packn;
    send->currN = currn;
    send->data_length = data->length();
    if(data->length()>0)
        send->data.append(*data);
    else
        send->data.clear();
    send->CRC_BT = getCRC_BT(send);
    send->end = END_PCK_STATE;
    return true;
}

bool CDataCom::subPackageData(QList<DATA_COM> *buf, SEND_ST &ts)
{
    QByteArray tmpdata = ts.data;
    int packn = tmpdata.length()/FRAME_MAX_LEN + 1;
    for(int i=0;i<packn;i++)
    {
        QByteArray senddata;
        if(tmpdata.length()>FRAME_MAX_LEN)
        {
            senddata.append(tmpdata.left(FRAME_MAX_LEN));
            tmpdata.remove(0,FRAME_MAX_LEN);
        }
        else
        {
            senddata.append(tmpdata);
        }
        DATA_COM send;
        orgSendData(&send,&senddata,ts.comT,ts.dataT,packn,i);
        buf->append(send);
    }

}

bool CDataCom::anaRecvData(QByteArray *data)
{

    return true;

}


bool CDataCom::isBuildCommt()
{
    return b_COMConnect;
}

bool CDataCom::ConnectToSerPort()
{
    bool bOpen = false;
    m_qByData.clear();
    m_res_data.clear();

    CSerialPortDialog *pSPDialog = new CSerialPortDialog();
    if(pSPDialog->exec() == QDialog::Accepted)
    {
        m_pSerialPort = pSPDialog->GetSerialPort(&bOpen);
    }
    if(bOpen)
    {
        b_COMConnect = true;
        m_pSerialPort->flush();
        emit SigCnntSucc(true);

    }
    else
    {
        b_COMConnect = false;
        emit SigCnntSucc(false);
    }

    return bOpen;
}
bool CDataCom::BreakToSerPort()
{
    b_COMConnect = false;
    emit SigCnntSucc(false);
    m_pSerialPort->close();
    m_pSerialPort = NULL;
    return true;
}


bool CDataCom::anaHistData(QByteArray &data, RES_DATA *hist)
{
    return false;

}

void CDataCom::SlotRdLoop()
{
//    qDebug() << "==m_pSerialPort==="<<m_pSerialPort;
    if(m_pSerialPort != NULL)
    {
        char temReBuf[1024];
#if 0
        if((m_pSerialPort->bytesAvailable() >= 2) )
        {
qDebug()<<"--"<<m_pSerialPort->bytesAvailable();

            //m_beforetime = QTime::currentTime();

            memset(temReBuf,0,1024);
            int nRetVar = m_pSerialPort->read(temReBuf, 1024);
            if(nRetVar > 0)
            {
                int findAt = -1;
                int findEnd = -1;

                m_qByData.append(temReBuf,nRetVar);
                QString tmp = QString::fromLocal8Bit(m_qByData);

                //QByteArray head;
                //head.append(4,0xAA);

                while(m_qByData.length()>=10)
                {
                    findAt =  m_qByData.indexOf("  ");
                    findEnd = m_qByData.indexOf('\n');
                    qDebug()<<"find"<<findAt<<findEnd;
                    if((findAt >= 0)&&(findEnd >= 0) && (findEnd-findAt>=3))
                    {
                        QString data;
                        data.append(m_qByData.mid(findAt+4,findEnd-4));
                        QStringList data_int;
                        data_int = data.split(',');
                        qDebug()<<"str"<<data_int.at(0)<<data_int.at(1);
                        int pos = QString(data_int.at(0)).toInt();
                        int val = QString(data_int.at(1)).toInt();

                        emit SigNewRpt(pos,val);
                        m_qByData.remove(0,findEnd+1);
                    }
                    else
                    {
                        m_qByData.remove(0,findEnd+1);
                        break;
                    }


                }


            }
        }
 #else
        if((m_pSerialPort->bytesAvailable() >= 2) )
        {
            static int isfindhead = 0;
            memset(temReBuf,0,1024);
            int nRetVar = m_pSerialPort->read(temReBuf, 1024);
            //for(int ii=0;ii<nRetVar;ii++)
            {
                //qDebug("%02x ",temReBuf[ii]);

            }
            if(nRetVar > 0)
            {
                m_qByData.append(temReBuf,nRetVar);
                if(curr_pos == 0)
                {
                   isfindhead = 0;
                   while(m_qByData.length() >= 4)
                   {
                       if(((uint8_t)m_qByData.at(0) == 0x3c) &&\
                          ((uint8_t)m_qByData.at(1) == 0xc3) &&\
                          ((uint8_t)m_qByData.at(2) == 0x33) &&\
                          ((uint8_t)m_qByData.at(3) == 0xcc))
                       {

                           isfindhead = 1;
                           m_qByData.remove(0,4);
                           break;
                       }
                       else
                        m_qByData.remove(0,1);

                   }
                }
                if(isfindhead == 1)
                {
                    while(m_qByData.length() >= 2)
                    {
                        if(((uint8_t)m_qByData.at(0) == 0x4f) &&\
                           ((uint8_t)m_qByData.at(1) == 0x4b))
                        {
                            curr_pos+=0x80000000;
                            isfindhead = 0;
                        }
                        unsigned char a0= m_qByData.at(0);
                        unsigned char a1= m_qByData.at(1);

                        unsigned int val = a0*256+a1;

                        m_qByData.remove(0,2);
                        emit SigNewRpt(curr_pos++,val);

                    }
                }


            }
        }
 #endif
    }
}
bool CDataCom::DataToSend(DATA_COM &data, QByteArray *send)
{
    /**/
    send->append(intToByte(data.head));
    send->append(intToByte(data.comT));
    send->append(intToByte(data.dataT));
    send->append(intToByte(data.packN));
    send->append(intToByte(data.currN));
    send->append(intToByte(data.data_length));
    send->append(data.data);
    send->append(intToByte(data.CRC_BT));
    send->append(intToByte(data.end));

    return true;
}

bool CDataCom::sendgetres()
{
    if(!b_COMConnect) return 0;
    m_pSerialPort->write("YRESULT \n");
}

bool CDataCom::sendgetcure()
{
    if(!b_COMConnect) return 0;

    m_pSerialPort->write("YRAWDATA \n");
}

bool CDataCom::sendstart(int time)
{
    if(!b_COMConnect) return 0;
    QByteArray tmp_send;
    tmp_send.append("@c");
    QString tmp = QString::number(time);
    if(tmp.length() < 2) tmp.insert(0,'0');
    if(tmp.length() < 3) tmp.insert(0,'0');
    if(tmp.length() < 4) tmp.insert(0,'0');

    tmp_send.append(tmp+"#@");
    qDebug(tmp_send);
    curr_pos = 0;
    m_qByData.clear();
    m_pSerialPort->write(tmp_send);

}

bool CDataCom::sendstop()
{
    if(!b_COMConnect) return 0;

    m_pSerialPort->write("YSTOP \n");
}

bool CDataCom::sendsetledcunt(int gree, int uv, int white)
{
    if(!b_COMConnect) return 0;

    QByteArray tmp_send;
    tmp_send.append("YLEDCUNT = ");
    tmp_send.append(QString::number(gree)+",");
    tmp_send.append(QString::number(uv)+",");
    tmp_send.append(QString::number(white)+" \n");

    qDebug(tmp_send);

    m_pSerialPort->write(tmp_send);
}

bool CDataCom::sendsetledtime(int type, int frq, int smd, int dly, int smp)
{
    if(!b_COMConnect) return 0;

    QByteArray tmp_send;
    tmp_send.append("YLEDTIME = ");
    tmp_send.append(QString::number(type)+",");
    tmp_send.append(QString::number(frq)+",");
    tmp_send.append(QString::number(smd)+",");
    tmp_send.append(QString::number(dly)+",");
    tmp_send.append(QString::number(smp)+" \n");

    qDebug(tmp_send);

    m_pSerialPort->write(tmp_send);

}

bool CDataCom::sendsetgainidx(int type, int cap, int res, int gain, int cunt)
{
    if(!b_COMConnect) return 0;

    QByteArray tmp_send;
    tmp_send.append("YGAINIDX = ");
    tmp_send.append(QString::number(type)+",");
    tmp_send.append(QString::number(cap)+",");
    tmp_send.append(QString::number(res)+",");
    tmp_send.append(QString::number(gain)+",");
    tmp_send.append(QString::number(cunt)+" \n");

    qDebug(tmp_send);

    m_pSerialPort->write(tmp_send);

}

bool CDataCom::sendsetmtpos(bool dir, int pos)
{
    if(!b_COMConnect) return 0;

    QByteArray tmp_send;
    tmp_send.append("YRESET = ");
    tmp_send.append(QString::number(dir)+",");
    tmp_send.append(QString::number(pos)+" \n");
    qDebug(tmp_send);

    m_pSerialPort->write(tmp_send);

}

bool CDataCom::sendsetsmpinfo(bool dir, int type, int dly, int num, int dis)
{
    if(!b_COMConnect) return 0;

    QByteArray tmp_send;
    tmp_send.append("YSMPINFO = ");
    tmp_send.append(QString::number(dir)+",");
    tmp_send.append(QString::number(type)+",");
    tmp_send.append(QString::number(dly)+",");
    tmp_send.append(QString::number(num)+",");
    tmp_send.append(QString::number(dis)+" \n");

    qDebug(tmp_send);

    m_pSerialPort->write(tmp_send);
}

void CDataCom::stopThread()
{
    qDebug() << "CDataCom::stopThread";
    bStop = true;
}

void CDataCom::SlotWdLoop()
{
    if(m_pSerialPort != NULL)
    {
        if(m_qSdData.count()>0)
        {
            subPackageData(&m_qSdBuff,m_qSdData[0]);
            m_qSdBuff.first().can_send = true;
            m_qSdData.removeFirst();
            emit SigSysPBStatus(0);
        }

        if((m_pSerialPort->bytesToWrite()==0)&&(m_qSdBuff.count()>0)&&(m_qSdBuff.first().can_send == true))
        {
            QByteArray senddata;
            DataToSend(m_qSdBuff.first(),&senddata);
            m_pSerialPort->write(senddata);
            qDebug("Send a new message len = %d %08x %d %d",senddata.length(),m_qSdBuff.first().comT,m_qSdBuff.first().packN,m_qSdBuff.first().currN);
            m_qSdBuff.first().can_send = false;
            if(m_qSdBuff.first().packN > 1)
                emit SigSysPBStatus((((m_qSdBuff.first().currN+1)*1.0)/(m_qSdBuff.first().packN*1.0))*100);
            m_beforetime = QTime::currentTime();
            if(m_qSdBuff.count() == 1)m_qSdBuff.removeFirst();

        }

    }
    if(!isBuildCommt())
    {
        m_qSdBuff.clear();
    }
}










