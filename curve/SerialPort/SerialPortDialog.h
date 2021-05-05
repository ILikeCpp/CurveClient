#ifndef SERIALPORTDIALOG_H
#define SERIALPORTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QtGui>
#include <QtCore>
#include "qextserialport.h"

#include <QSerialPort>

namespace Ui {
    class CSerialPortDialog;
}

class CSerialPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSerialPortDialog(QDialog *parent = 0);
    ~CSerialPortDialog();

    QSerialPort *GetSerialPort( bool *bOpen );

    void CloseSerialPort();
    QString GetComm(int index,QString keyorvalue);

    void showEvent ( QShowEvent * event );


private slots:
    void on_btnEnter_clicked();

    void on_btnCancle_clicked();
    void readDataCom();

private:
    Ui::CSerialPortDialog *ui;

//    QextSerialPort * m_pSerialPort;

    QSerialPort *m_pSerialPort;

    bool m_bOpen;


};

#endif // SERIALPORTDIALOG_H
