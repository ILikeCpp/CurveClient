#include "commoninterface.h"
#include <QFile>


CDataCom CCommonInterface::m_datacom;
QList<FLCURVE_DATA> CCommonInterface::m_fllist;

CCommonInterface::CCommonInterface()
{

}

CDataCom * CCommonInterface::get_datacom()
{
    return &m_datacom;
}

bool CCommonInterface::flstring_tofldata(QByteArray &str, int *data, int len)
{
    if(str.length() >= len*4 )
    {
        for(int i=0;i<len;i++)
        {
            unsigned int tmp = 0;
            for(int j=0;j<4;j++)
            {
                tmp<<=8;
                tmp += (uchar)str.at(i*4+j);
            }
            //data[i] = tmp*(2500.0/524287.0);
            data[i] = tmp;
        }
        return true;
    }
    else
        return false;
}

bool CCommonInterface::fldata_scalecov(int *data, float *fldata, int len,int scale)
{
    for(int i=0;i<len;i++)
    {
        fldata[i] = data[i]*(scale/524287.0);
    }
    return true;
}

bool CCommonInterface::add_flcurve(QString ser, int *data, int len)
{
    FLCURVE_DATA tmp;
    tmp.ser.append(ser);
    for(int i=0;i<len;i++)
        tmp.data[i] = data[i];
    m_fllist.append(tmp);
    return true;
}

bool CCommonInterface::get_flcurve(QString ser, int *data, int len)
{
    for(int i=0;i<m_fllist.count();i++)
    {
        if(ser == m_fllist.at(i).ser)
        {
            for(int j=0;j<len;j++)
                data[j] = m_fllist[i].data[j];
            return true;
        }
    }
    return false;
}

bool CCommonInterface::sel_flcurve(QString ser)
{
    for(int i=0;i<m_fllist.count();i++)
    {
        if(ser == m_fllist.at(i).ser)
        {
            return true;
        }
    }
    return true;
}

bool CCommonInterface::del_flcurve(QString ser)
{
    for(int i=0;i<m_fllist.count();i++)
    {
        if(ser == m_fllist.at(i).ser)
        {
            m_fllist.removeAt(i);
            return true;
        }
    }
    return true;
}

bool CCommonInterface::cle_flcurve()
{
    m_fllist.clear();
    return true;
}

QStringList CCommonInterface::get_flser()
{
    QStringList ret;
    for(int i=0;i<m_fllist.count();i++)
    {
        ret.append(m_fllist.at(i).ser);
    }
    return ret;
}


