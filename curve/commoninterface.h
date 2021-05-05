#ifndef COMMONINTERFACE_H
#define COMMONINTERFACE_H

#include "datacom.h"

#define X_PROINFO_MAX 100

#define CURVE_DATA_SIZE 600
typedef struct _flcurve_data_
{
    QString ser;
    int data[CURVE_DATA_SIZE];

}FLCURVE_DATA;


class CCommonInterface
{
public:
    CCommonInterface();


    static CDataCom* get_datacom();

    static bool flstring_tofldata(QByteArray &str,int *data,int len);
    static bool fldata_scalecov(int *data,float *fldata,int len,int scale);
    static bool add_flcurve(QString ser,int *data,int len);
    static bool get_flcurve(QString ser,int *data,int len);
    static bool sel_flcurve(QString ser);
    static bool del_flcurve(QString ser);
    static bool cle_flcurve();
    static QStringList get_flser();

private:

    static CDataCom m_datacom;
    static QList<FLCURVE_DATA> m_fllist;
};

#endif // COMMONINTERFACE_H
