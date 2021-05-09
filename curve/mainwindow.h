/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPrinter>
#include "ui_mainwindowbase.h"
#include "SerialPort/SerialPortDialog.h"
#include "datacom.h"
#include "ui_helpdialog.h"
#include "aboutdialog.h"
#include "setdialog.h"
#include <QList>
#include <QStringList>
#include "xlineedit.h"
#include "xcombobox.h"
#include "listwidget.h"
#include "commoninterface.h"

#include <QSettings>
#include <QThread>

#include "qwt-6.1.3/qwt_plot.h"
#include "qwt-6.1.3/qwt_plot_curve.h"
// #include "qwt-6.1.3/qwt_scale_widget.h"
#include "qwt-6.1.3/qwt_plot_canvas.h"
#include "qwt-6.1.3/qwt_plot_magnifier.h"
#include "qwt-6.1.3/qwt_plot_panner.h"
#include "qwt-6.1.3/qwt_plot_marker.h"
#include "qwt-6.1.3/qwt_plot_zoomer.h"
#include "qwt-6.1.3/qwt_plot_grid.h"


#include "qwt-6.1.3/qwt_analog_clock.h"
#include "qwt-6.1.3/qwt_series_data.h"
#include "qwt-6.1.3/qwt_global.h"
#include "qwt-6.1.3/qwt_symbol.h"
#include "qwt-6.1.3/qwt_text_label.h"
#include "qwt-6.1.3/qwt_system_clock.h"
#include "qwt-6.1.3/qwt_picker.h"
#include "qwt-6.1.3/qwt_wheel.h"
#include "qwt-6.1.3/qwt_plot_grid.h"
#include "qwt-6.1.3/qwt_abstract_slider.h"
#include "qwt-6.1.3/qwt_legend.h"
#include "qwt-6.1.3/qwt_legend_label.h"
#include "qwt-6.1.3/qwt_plot_renderer.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

#define MAX_DTA4_LINE 20
#define MAX_DTA4H_LINE 10
#define MAX_DTA5_LINE 10
#define REFER_DB "refer.ini"
#define PROJECT_NAME "project.ini"
#define HINT_DB "hishint.ini"
#define HISH_DB "hishintdb.ini"
#define CONFIG_DB "config.ini"

#define TIMEOUT_T (9000)

#define CURVE_COUNT 1

typedef QList<QTreeWidgetItem *> StyleItems;

class QUdpSocket;
class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getLocalIpAddr();


    typedef enum{
        CONNECT = 0,
        DISCONT,
        GETDATA,
        TONUM
    }TIMEOUT_t;

   void SetTimeOut(TIMEOUT_t mode,bool bStop);
   void InitDataToSys();
   void closeEvent(QCloseEvent *event);

signals:
    void sigDataChange(QString str);
    //void SigCnntSucc(bool);
    //void SigNewRpt(int,QByteArray);
    void SigNewData(QStringList hisdata);
    void signal_start_udp(QString ip, QString data);
    void signal_send_cmd(QString cmd);

public slots:

#ifndef QT_NO_PRINTER
    void on_printAction_triggered();
    void on_printPreviewAction_triggered();
#endif
    void on_atNew_triggered();
    void on_atOpen_triggered();
    void on_atSave_triggered();
    void on_atSaveAs_triggered();
    void on_atCopy_triggered();
    void on_atPaste_triggered();
    void on_atDele_triggered();
    void on_atSet_triggered();
    void on_atConnect_triggered();
    void on_atManual_triggered();
    void on_atAbout_triggered();


    void SlotNewRpt(int pos,int data);

    void SlotCnntStatus(bool status);
    void SlotSetPBStatu(int pc);
    void SetTimeOut_show();

    void slotSendTimeout();

    void slotQuit();

    void slot_updateImage(const int row, QImage *image);
    void slot_clearGrayImage(bool stop);


#ifndef QT_NO_PRINTER
    void printDocument(QPrinter *printer);
    void printPage(int index, QPainter *painter, QPrinter *printer);
#endif

private slots:




    void on_START_clicked();


    void on_fitscale_clicked();

    void on_savedata_clicked();

//    void on_actionAutoSave_triggered(bool checked);

//    void on_checkBox_clicked(bool checked);

    void on_radioButton_start_toggled(bool checked);

    void on_radioButton_close_toggled(bool checked);

    void on_radioButton_manual_toggled(bool checked);

    void on_radioButton_auto_toggled(bool checked);

    void on_radioButton_txt_toggled(bool checked);

    void on_radioButton_excel_toggled(bool checked);

//    void on_radioButton_single_toggled(bool checked);

//    void on_radioButton_multi_toggled(bool checked);

    void on_pushButton_clear_clicked();

    void on_pushButton_autoSavePath_clicked();

    void on_pushButton_start_udp_clicked();

    void on_pushButton_cmd_clicked();

private:
    void saveTxt(QString fileName);
    void saveXlsx(QString fileName);
    void saveCurrentCurve(int index);
    void saveCurrentCurve(QwtPlotCurve *curve);
    void saveCurrentCurveTxt(int index);
    void saveCurrentCurveXlsx(int index);
    void saveCurrentCurveTxt(QwtPlotCurve *curve);
    void saveCurrentCurveXlsx(QwtPlotCurve *curve);

private:

    QList<CXlineEdit *> m_LE_List;
    QList<QStringList> m_hintData;
    QList<QStringList> qlist;

    QMap<int,QStringList> currentPageMap();
    QMap<int,QStringList> pageMap;
    QStringList family;
    int f_num ;
    QStringList name;

    QStringList m_datatitle;

    CDataCom *m_datacom;
    QTimer *m_showHisData;

    CAboutDialog *m_aboutDialog;
    CSetDialog *m_setDialog;

    QList<QStringList> m_hisList;

    QString m_referURL;
    QStringList m_referData;

    QString m_projectURL;
    QStringList m_projectData;

    QList<int> m_compoSpac;

    QString m_hintURL;

    QString m_saveDataURL;

    QClipboard *m_pCclipborad;


    QTimer *m_timeout;
    TIMEOUT_t m_timode;


    QMap<int,double>qres;
    QMap<int,double>m_row;

    RES_DATA res_data;
    QStringList res_list;

    QwtPlot * m_pPlot;
    QwtPlotCurve *curve_ref;
    QwtPlotCurve *curve_res;
    int temp_num ;
    QTimer *m_timeout_show;

    QList<QwtPlotCurve*> m_curveList;
    QList< QPair< QVector<double>*, QVector<double>* >* > m_curveDataList;
    int m_currentCurveIndex;
    QTimer *m_SendTimer;

    QSettings *m_settings;

    QThread m_thread;
};

#endif
