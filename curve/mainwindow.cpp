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

#include <QtGui>

#include "mainwindow.h"
#include <QTextCodec>
#include <stdlib.h>
#include <QMessageBox>
#include <QPrintDialog>
#include <QProgressDialog>
#include <QPrintPreviewDialog>
#include <QtGui>
#define QT_NO_DRAGANDDROP
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

#include "xlsxdocument.h"
#include "xlsxformat.h"

#include <QUdpSocket>
#include <QNetworkInterface>
#include <QHostAddress>
#include "readdatathread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),m_currentCurveIndex(-1),m_SendTimer(new QTimer(this))
{
    setupUi(this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_SendTimer, &QTimer::timeout, this, &MainWindow::slotSendTimeout);

    this->comboBox->addItem("采集",0xAE);
    this->comboBox->addItem("开灯",0xAF);
    this->comboBox->addItem("关灯",0xB0);
    this->comboBox->addItem("积分1",0xB1);
    this->comboBox->addItem("积分2",0xB2);
    this->comboBox->addItem("积分3",0xB3);
    this->comboBox->addItem("积分4",0xB4);
    this->comboBox->addItem("积分5",0xB5);
    this->comboBox->addItem("积分6",0xB6);
    this->comboBox->addItem("积分7",0xB7);
    this->comboBox->addItem("积分8",0xB8);
    this->comboBox->addItem("积分9",0xB9);
    this->comboBox->addItem("积分10",0xBA);
    this->comboBox->addItem("积分11",0xBB);
    this->comboBox->addItem("积分12",0xBC);
    this->comboBox->addItem("积分13",0xBD);
    this->comboBox->addItem("积分14",0xBE);
    this->comboBox->addItem("积分15",0xBF);

    printAction->setEnabled(true);
    printPreviewAction->setEnabled(true);
    setWindowIcon(QIcon(":/ccdlog.png"));
    this->setWindowTitle(tr("Quickscan"));

    //this->setFixedSize(960,720);

//    connect(comp_lb,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));


    m_settings = new QSettings(QApplication::applicationDirPath()+"/ccd.ini",QSettings::IniFormat,this);

    this->lineEdit_autoSavePath->setText(m_settings->value("autoSave/path",QApplication::applicationDirPath()).toString());

    //m_datacom = new CDataCom;

    m_aboutDialog = new CAboutDialog;
    m_aboutDialog->hide();

    m_setDialog = new CSetDialog;
    m_setDialog->hide();

    m_pCclipborad = QApplication::clipboard();//清空



    CCommonInterface::get_datacom()->start();
    connect(CCommonInterface::get_datacom(), SIGNAL(SigCnntSucc(bool)), this, SLOT(SlotCnntStatus(bool)));
    connect(CCommonInterface::get_datacom(), SIGNAL(SigNewRpt(int ,int )), this, SLOT(SlotNewRpt(int ,int )));

    //InitDataToSys();
    connect(CCommonInterface::get_datacom(), SIGNAL(SigSysPBStatus(int )), this, SLOT(SlotSetPBStatu(int)));

    m_pPlot = new QwtPlot(this->groupBox);
    m_pPlot->setGeometry((this->groupBox->width()-750)/2+12, (this->groupBox->height()-400)/2, 750, 400);
    m_pPlot->canvas()->setStyleSheet("background-color: rgb(0xff, 0xff, 0xff);");
    //m_pPlot->canvas()->setMaximumHeight(200);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(m_pPlot);
    // grid->enableX(false);

    m_pPlot->setAxisScale(QwtPlot::xBottom, 0,  4000,400);
    m_pPlot->setAxisScale(QwtPlot::yLeft,   0, 4096,200);

    //使用滚轮放大/缩小
    QwtPlotMagnifier *pPlotMagnifier = new QwtPlotMagnifier( m_pPlot->canvas() );

    pPlotMagnifier->setMouseButton(Qt::MidButton, Qt::NoModifier);
    // 使用鼠标右键平移
    QwtPlotPanner * pPlotPanner = new QwtPlotPanner( m_pPlot->canvas() );
    //pPlotPanner->setMaximumSize(10000, 10000);
    pPlotPanner->setMouseButton(Qt::RightButton, Qt::NoModifier);
    //pPlotPanner->setMinimumHeight(0);

    QwtPlotZoomer* zoomer = new QwtPlotZoomer(m_pPlot->canvas() );
    zoomer->setRubberBandPen(QColor(Qt::black ));
    zoomer->setTrackerPen(QColor(Qt::black));
    //zoomer->setZoomBase(QRectF(0, 0, 600, m_flscale));

    zoomer->setMousePattern( QwtEventPattern::MouseSelect2, Qt::LeftButton, Qt::ControlModifier );
    m_pPlot->replot();

    curve_ref = new QwtPlotCurve("Standard");
    curve_ref->attach(m_pPlot);   //把曲线附加到图上
    //curve_ref->setPen(QColor(Qt::black),1);
    curve_ref->setStyle(QwtPlotCurve::Lines);
    curve_ref->setCurveAttribute(QwtPlotCurve::Fitted , true );
    curve_ref->setRenderHint(QwtPlotItem::RenderAntialiased,true);//抗锯齿
    curve_ref->setLegendAttribute(curve_ref->LegendShowLine);//

//    curve_res = new QwtPlotCurve("Result");
//    curve_res->attach(m_pPlot);   //把曲线附加到图上
//    curve_res->setPen(QColor(Qt::red),3);
//    curve_res->setStyle(QwtPlotCurve::Lines);
//    curve_res->setCurveAttribute(QwtPlotCurve::Fitted , true );
//    curve_res->setRenderHint(QwtPlotItem::RenderAntialiased,true);//抗锯齿
//    curve_res->setLegendAttribute(curve_res->LegendShowLine);//

    for (int i = 0; i < CURVE_COUNT; ++i)
    {
        QwtPlotCurve *curve = new QwtPlotCurve(QString("Curve%1").arg(i+1));
        m_curveList.append(curve);

//        curve->attach(m_pPlot);//把曲线附加到图上
        QColor color(qSin(i*0.3)*100+100, qSin(i*0.6+0.7)*100+100, qSin(i*0.4+0.6)*100+100);
        curve->setPen(color,1);
        curve->setStyle(QwtPlotCurve::Lines);
        curve->setCurveAttribute(QwtPlotCurve::Fitted , true );
        curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//抗锯齿
        curve->setLegendAttribute(curve->LegendShowLine);//

        QPair< QVector<double>*, QVector<double>* > *pair = new QPair< QVector<double>*, QVector<double>* >;
        pair->first = new QVector<double>;
        pair->second = new QVector<double>;
        m_curveDataList.append(pair);
    }

    QwtLegend *legend = new QwtLegend;

    m_pPlot->insertLegend( legend, QwtPlot::BottomLegend );

    m_timeout = new QTimer;
//    connect(m_timeout, SIGNAL(timeout()), this, SLOT(TimeOutPrss()));
    m_timeout_show = new QTimer;
    connect(m_timeout_show, SIGNAL(timeout()), this, SLOT(SetTimeOut_show()));

    temp_num = 0;

    QPixmap pix(1420,4867);
    QColor color("white");
    pix.fill(color);
    QImage image = pix.toImage();

    qDebug()<< image.rect();
    label_grayImage->setPixmap(pix);

    label_local->setText(getLocalIpAddr());

    ReadDataThread *readThread = new ReadDataThread();
    readThread->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, readThread, &ReadDataThread::slotStart);
    connect(&m_thread, &QThread::finished, readThread, &ReadDataThread::deleteLater);
    connect(this, &MainWindow::signal_start_udp, readThread, &ReadDataThread::slot_start_udp);
    connect(this, &MainWindow::signal_send_cmd, readThread, &ReadDataThread::slot_send_cmd);
    connect(readThread, &ReadDataThread::signal_updateImage, this, &MainWindow::slot_updateImage);
    connect(readThread, &ReadDataThread::signal_clearGrayImage, this, &MainWindow::slot_clearGrayImage);
    connect(readThread, &ReadDataThread::SigNewRpt, this, &MainWindow::SlotNewRpt);
    m_thread.start();
}

QString MainWindow::getLocalIpAddr()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

//    qDebug() << ipAddressesList;

    for(int i = 0; i < ipAddressesList.size(); ++i)
    {
        QHostAddress addr = ipAddressesList.at(i);
        // 找到不是本地ip，并且是ipv4协议，并且不是169开头的第一个地址
        if(addr != QHostAddress::LocalHost && addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.toString().startsWith("169"))
        {
            ipAddress = addr.toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return ipAddress;
}

MainWindow::~MainWindow()
{
    m_SendTimer->stop();
    m_thread.quit();
    m_thread.wait();
}

void MainWindow::slot_updateImage(const int row, QImage *image)
{
//    qDebug() << "222222222";
    label_grayImage->setPixmap(QPixmap::fromImage(*image));
    label_lineCounts->setText(QString("%1行").arg(row));
}

void MainWindow::slot_clearGrayImage(bool stop)
{
    if (stop) {
        this->pushButton_start_udp->setEnabled(true);
    }
    else {
        this->pushButton_start_udp->setEnabled(false);
    }

    label_grayImage->clear();
}

void MainWindow::InitDataToSys()
{
    QFile file(HINT_DB);

    if(file.open(QIODevice::ReadOnly))
    {
        char acBuff[1024];
        int numline = 0;
        while(!file.atEnd())
        {
            memset(acBuff, 0x00, sizeof(acBuff));
            int len = file.readLine(acBuff, sizeof(acBuff));
            QString tmpstr(acBuff);
            tmpstr.remove("\n");
            tmpstr.remove("\r");
            qDebug()<<QString::fromLocal8Bit(acBuff,len);

            QStringList tmplist = QString::fromLocal8Bit(acBuff,len).split(",");
            m_hintData.append(tmplist);
            m_LE_List.at(numline)->addHintStrings(tmplist);
            m_LE_List.at(numline)->setHintNum(m_setDialog->GetHistoryNum());
            if(++numline>=m_LE_List.count())
                break;
        }
        file.close();
    }
    while(m_hintData.count()<m_LE_List.count())
        m_hintData.append(QStringList(""));
    qDebug()<<m_hintData.count();
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    QMessageBox::StandardButton button;

    button = QMessageBox::question(this, tr("退出程序"),
        QString(tr("提示：软件即将关闭，请确保数据已保存!\n     Yes：继续退出  No：返回保存")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
   }
    else if (button == QMessageBox::Yes) {
        CCommonInterface::get_datacom()->stopThread();
        event->accept();  //接受退出信号，程序退出
        qDebug()<< "222222222222223333333333333333333";

   }
   //event->accept();
}
#ifndef QT_NO_PRINTER
void MainWindow::on_printAction_triggered()
{
    qDebug()<<"on_printAction_triggered";
//    pageMap = currentPageMap();

//    if(pageMap.count() == 0)
//        return;
    if(m_setDialog->GetPageStye() == "A5")
    {
        if(m_hisList.count()>MAX_DTA5_LINE)
        {
            QMessageBox::StandardButton button;

            button = QMessageBox::question(this, tr("退出程序"),
                                           QString(tr("提示：您要打印的条数超过%1条，请删减!")).arg(MAX_DTA5_LINE));
            return;

        }
    }
    else if(m_setDialog->GetPageStye() == "A4")
    {
        if(m_hisList.count()>MAX_DTA4_LINE)
        {
            QMessageBox::StandardButton button;

            button = QMessageBox::question(this, tr("退出程序"),
                                           QString(tr("提示：您要打印的条数超过%1条，请删减!")).arg(MAX_DTA4_LINE));
            return;
        }
    }
    else
    {
        if(m_hisList.count()>MAX_DTA4H_LINE)
        {
            QMessageBox::StandardButton button;

            button = QMessageBox::question(this, tr("退出程序"),
                                           QString(tr("提示：您要打印的条数超过%1条，请删减!")).arg(MAX_DTA4H_LINE));
            return;
        }
    }

    QPrinter printer(QPrinter::HighResolution);

    if(m_setDialog->GetPageStye() == "A5")
    {
        qDebug()<<"A5";
        printer.setPageSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);
    }
    else
    {
        qDebug()<<"A4";
        printer.setPageSize(QPrinter::A4);
    }

    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted)
        return; 

    printDocument(&printer);
}

void MainWindow::printDocument(QPrinter *printer)
{
    qDebug()<<"printDocument";

    if(m_setDialog->GetPageStye() == "A4")
    {
       printer->setFromTo(1,1);
    }
    else
    {
       //printer->setFromTo(1,pageMap.count());
       printer->setFromTo(1,1);

    }

    QProgressDialog progress(tr("报告单打印"), tr("&Cancel"),0, 1, this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("报告单"));
    progress.setMinimum(printer->fromPage() - 1);
    progress.setMaximum(printer->toPage());

    QPainter painter;
    painter.begin(printer);
    bool firstPage = true;

    for (int page = printer->fromPage(); page <= printer->toPage(); ++page)
    {
        if (!firstPage)
            printer->newPage();

        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        printPage(page - 1, &painter, printer);
        progress.setValue(page);
        firstPage = false;
    }
    painter.end();
}

void MainWindow::on_printPreviewAction_triggered()
{
    qDebug()<<"on_printPreviewAction_triggered";
    //pageMap = currentPageMap();

    //if(pageMap.count() == 0)
    //   return;

    if(m_setDialog->GetPageStye() == "A5")
    {
        if(m_hisList.count()>MAX_DTA5_LINE)
        {
            QMessageBox::StandardButton button;

            button = QMessageBox::question(this, tr("退出程序"),
                                           QString(tr("提示：您要打印的条数超过%1条，请删减!")).arg(MAX_DTA5_LINE));
            return;

        }
    }
    else if(m_setDialog->GetPageStye() == "A4")
    {
        if(m_hisList.count()>MAX_DTA4_LINE)
        {
            QMessageBox::StandardButton button;

            button = QMessageBox::question(this, tr("退出程序"),
                                           QString(tr("提示：您要打印的条数超过%1条，请删减!")).arg(MAX_DTA4_LINE));
            return;

        }
    }
    else
    {
        if(m_hisList.count()>MAX_DTA4H_LINE)
        {
            QMessageBox::StandardButton button;

            button = QMessageBox::question(this, tr("退出程序"),
                                           QString(tr("提示：您要打印的条数超过%1条，请删减!")).arg(MAX_DTA4H_LINE));
            return;
        }
    }

    QPrinter printer(QPrinter::HighResolution);
    if(m_setDialog->GetPageStye() == "A5")
    {
        qDebug()<<"A5";
        printer.setPageSize(QPrinter::A5);
        //setPaperSize(QSizeF(197,147),QPrinter::Point);
        printer.setOrientation(QPrinter::Landscape);
    }
    else
    {
        qDebug()<<"A4";
        printer.setPageSize(QPrinter::A4);

    }
    QPrintPreviewDialog preview(&printer, this);

    connect(&preview, SIGNAL(paintRequested(QPrinter*)),this, SLOT(printDocument(QPrinter*)));
    preview.exec();
}

void MainWindow::printPage(int index, QPainter *painter, QPrinter *printer)
{
    //f_num = pageMap.keys()[index];
    //family = pageMap[f_num] ;
    qDebug()<<"printPage";

    qreal width = printer->pageRect().width();
    qreal height = printer->pageRect().height();
    //qDebug()<<printer->pageSize()<<width<<height;

    painter->save();
    painter->translate(0, 0);
    painter->scale(1, 1);
    painter->setBrush(QBrush(Qt::black));

    painter->restore();
}
#endif
void MainWindow::on_atNew_triggered()
{
    m_hisList.clear();
}
void MainWindow::on_atOpen_triggered()
{

}
void MainWindow::on_atSave_triggered()
{

}
void MainWindow::on_atSaveAs_triggered()
{

}
void MainWindow::on_atCopy_triggered()
{

}
void MainWindow::on_atPaste_triggered()
{
    qDebug()<<QApplication::focusWidget()->winId();
}
void MainWindow::on_atDele_triggered()
{
}
void MainWindow::on_atSet_triggered()
{
    //if(m_setDialog->exec() == QDialog::Accepted)
    {
    }
}
void MainWindow::on_atManual_triggered()
{
    QDialog * w  = new QDialog;
    Ui::Chelpdialog help;
    help.setupUi(w);
    w->setWindowTitle(tr("帮助"));
    w->exec();

}
void MainWindow::on_atAbout_triggered()
{
    if(m_aboutDialog->exec() == QDialog::Accepted)
    {
    }
}

void MainWindow::SlotNewRpt(int pos,int data)
{
//    if (this->radioButton_single->isChecked())
//    {
//        static QVector<double> xData;
//        static QVector<double> yData;
//        static int maxval = 0;

//        //curve_ref->detach();
//        //curve_res->detach();

//        //unsigned int dot = (unsigned char)data[0]*256+(unsigned char)data[1];
//        //unsigned int res = ((unsigned char)data[2])*256*256*256+((unsigned char)data[3])*256*256+((unsigned char)data[4])*256+((unsigned char)data[5]);

//        //qDebug()<<(unsigned char)data[0]<<(unsigned char)data[1]<<(unsigned char)data[2]<<(unsigned char)data[3]<<(unsigned char)data[4]<<(unsigned char)data[5];
//        qDebug()<<pos<<data;
//        if(maxval<data)
//        {
//            maxval = data;
//            //m_pPlot->setAxisScale(QwtPlot::yLeft,   0, maxval+100,(maxval+100)/10);

//        }
//        if(pos == 0)
//        {
//            maxval = 0;
//            m_pPlot->setAxisScale(QwtPlot::xBottom, 0,  4000,400);
//            m_pPlot->setAxisScale(QwtPlot::yLeft,   0, 4096,200);
//            xData.clear();
//            yData.clear();
//            curve_ref->brush();
//            //return;
//            if (this->radioButton_auto->isChecked())
//            {
//                saveCurrentCurve(curve_ref);
//            }

//        }

//        if(pos&0x80000000)
//        {
//            m_pPlot->setAxisScale(QwtPlot::xBottom, 0,  pos^0x80000000,(pos^0x80000000)/10);

//            qDebug("%d,%d ",pos^0x80000000,(pos^0x80000000)/10);
//            m_pPlot->replot();
//            return;
//        }
//        this->getdatanum->setText(QString::number(pos));
//        xData.append(pos);
//        yData.append(data);

//        curve_ref->setSamples(&xData[0], &yData[0], xData.length());


//        m_pPlot->replot();
//    }
//    else
    {
        if (0 == pos)
        {
            m_currentCurveIndex += 1;
            this->label_currentCurve->setText(QString("Curve%1").arg(m_currentCurveIndex+1));
            //保存完成绘图的数据
            if (m_currentCurveIndex > 0 && m_currentCurveIndex <= m_curveList.size())
            {
                if (this->radioButton_auto->isChecked())
                {
                    saveCurrentCurve(m_currentCurveIndex-1);
                }
            }
        }

        if (m_currentCurveIndex >= CURVE_COUNT)
        {
            m_currentCurveIndex = 0;
            this->label_currentCurve->setText(QString("Curve%1").arg(m_currentCurveIndex+1));
            for(int i = 0; i < m_curveDataList.size(); ++i)
            {
                QPair< QVector<double>*, QVector<double>* >* pair = m_curveDataList.at(i);
                pair->first->clear();
                pair->second->clear();
            }
        }

        if(pos&0x80000000)
        {
            m_pPlot->setAxisScale(QwtPlot::xBottom, 0,  pos^0x80000000,(pos^0x80000000)/10);

            qDebug("%d,%d ",pos^0x80000000,(pos^0x80000000)/10);
            m_pPlot->replot();
            return;
        }

        this->getdatanum->setText(QString::number(pos));

        QwtPlotCurve *curve = m_curveList.at(m_currentCurveIndex);
        QPair< QVector<double>*, QVector<double>* >* pair = m_curveDataList.at(m_currentCurveIndex);
        pair->first->append(pos);
        pair->second->append(data);
        curve->setSamples((*pair->first),(*pair->second));

        m_pPlot->replot();
    }
}


void MainWindow::on_atConnect_triggered() // 连接
{
//    if(CCommonInterface::get_datacom()->isBuildCommt())
//    {
//        if(CCommonInterface::get_datacom()->BreakToSerPort())
//        {

//        }
//    }
//    else
//    {
//        if(CCommonInterface::get_datacom()->ConnectToSerPort())
//        {
//        }
//    }
}


void MainWindow::SlotCnntStatus(bool status)
{

    qDebug()<<"SlotCnntStatus = "<<status;

    if(status == true)
    {
        this->atConnect->setText(tr("断开"));
        this->atConnect->setIcon(QIcon(":/images/ok.png"));
    }
    else
    {
        this->atConnect->setText(tr("连接"));
        this->atConnect->setIcon(QIcon(":/images/apply.png"));
    }
}

void MainWindow::SlotSetPBStatu(int pc)
{
}

void MainWindow::SetTimeOut_show()
{
    this->START->setEnabled(true);

}

void MainWindow::slotSendTimeout()
{
    qDebug() << "MainWindow::slotSendTimeout";
//    CCommonInterface::get_datacom()->sendstart(this->comboBox->currentData().toInt());
//    this->getdatanum->setText("0");
    emit signal_send_cmd(QString::number(0xAE,16));

//    QTimer *timer = new QTimer;
//    connect(timer, &QTimer::timeout, [=]{
//        static int pos = 0;
//        static int factor = 1;
//        qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
//        int value = (qrand()*(pos+1))%100 + 200*factor;
//        SlotNewRpt(pos, value);
//        pos += 1;
//        if (pos > 100)
//        {
//            pos = 0;
//            factor += 1;
//            if (factor > CURVE_COUNT)
//            {
//                factor = 1;
//            }
//        }
//    });

    //    timer->start(50);
}

void MainWindow::slotQuit()
{
    CCommonInterface::get_datacom()->stopThread();
    qDebug() << "MainWindow::slotQuit";
    qApp->quit();
}









void MainWindow::SetTimeOut(TIMEOUT_t mode,bool bStop)
{
    if(!bStop)
    {
        m_timode = mode;
        m_timeout->start(TIMEOUT_T);
    }
    else
    {
        m_timeout->stop();
        m_timode = TONUM;
    }
}




void MainWindow::on_START_clicked()
{
//    this->START->setDisabled(true);
//    CCommonInterface::get_datacom()->sendstart(this->comboBox->currentData().toInt());
//    m_timeout_show->start(3000);
//    this->getdatanum->setText("0");
    qDebug() << this->comboBox->currentData().toInt();
    emit signal_send_cmd(QString::number(this->comboBox->currentData().toInt(),16));
}


void MainWindow::on_fitscale_clicked()
{
   int ymax = m_curveList.first()->maxYValue()+10;
   int ymin = m_curveList.first()->minYValue()-10;
   int xmax = m_curveList.first()->maxXValue()+10;
   qDebug("%d,%d,%d",ymax,ymin,xmax);
   m_pPlot->setAxisScale(QwtPlot::xBottom, 0,  xmax,xmax/10);
   m_pPlot->setAxisScale(QwtPlot::yLeft,   ymin, ymax,(ymax-ymin)/20);
    m_pPlot->replot();
}

void MainWindow::on_savedata_clicked()
{
    QString tempName;
    QString filter;
    if (this->radioButton_txt->isChecked())
    {
        tempName = QString("./%1untitled.txt").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
        filter = tr("TXT(*.txt);;Excel(*.xlsx)");
    }
    else
    {
        tempName = QString("./%1untitled.xlsx").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
        filter = tr("Excel(*.xlsx);;TXT(*.txt)");
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("save"),
                                                        tempName,
                                                        filter);

    if (fileName.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(fileName);

    QString suffix = fileInfo.suffix();
    if ("txt" == suffix)
    {
        saveTxt(fileName);
    }
    else if ("xlsx" == suffix)
    {
        saveXlsx(fileName);
    }
}

void MainWindow::saveTxt(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.close();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.seek(file.size());

//        if (this->radioButton_single->isChecked())
//        {
//            int xmax = curve_ref->maxXValue();
//            for(int i=0;i<xmax;i++)
//            {
//                //QString tt = QString::number(curve_ref->data()->sample(i).y(),);
//                stream << curve_ref->data()->sample(i).x() << "," << curve_ref->data()->sample(i).y() << "\n";
//            }
//        }
//        else
        {
            for (int i = 0; i < m_curveList.size(); ++i)
            {
                QwtPlotCurve *curve = m_curveList.at(i);
                int xmax = curve->maxXValue();
                for(int i=0;i<xmax;i++)
                {
                    //QString tt = QString::number(curve_ref->data()->sample(i).y(),);
                    stream << curve->data()->sample(i).x() << "," << curve->data()->sample(i).y() << "\n";
                }
            }
        }

        file.close();
    }
}

void MainWindow::saveXlsx(QString fileName)
{
    //打开excel
    QXlsx::Document xlsx;
    QXlsx::Format format1;
    format1.setFontSize(12);/*设置字体大小*/
    format1.setFontBold(true);

    QXlsx::Format format2;
    format2.setFontSize(10);/*设置字体大小*/

    //先写入标题
    QStringList titles;
    titles << "序号" << "值";

    for(int i = 0; i < titles.size(); ++i){
        xlsx.write(1,i+1,QVariant(titles.at(i)),format1);
    }

//    if (this->radioButton_single->isChecked())
//    {
//        int xmax = curve_ref->maxXValue();
//        for(int i = 0; i < xmax; ++i){
//            xlsx.write(i+2,1,QVariant(curve_ref->data()->sample(i).x()),format2);
//            xlsx.write(i+2,2,QVariant(curve_ref->data()->sample(i).y()),format2);
//        }
//    }
//    else
    {
        //然后逐行的写入
        for (int i = 0; i < m_curveList.size(); ++i)
        {
            QwtPlotCurve *curve = m_curveList.at(i);
            int xmax = curve->maxXValue();
            for(int i = 0; i < xmax; ++i){
                xlsx.write(i+2,1,QVariant(curve->data()->sample(i).x()),format2);
                xlsx.write(i+2,2,QVariant(curve->data()->sample(i).y()),format2);
            }
        }
    }



    //保存到文件
    bool ok = xlsx.saveAs(fileName);
    qDebug() << fileName;
}

void MainWindow::saveCurrentCurve(int index)
{
    if (this->radioButton_txt->isChecked())
    {
        saveCurrentCurveTxt(index);
    }
    else
    {
        saveCurrentCurveXlsx(index);
    }

}

void MainWindow::saveCurrentCurve(QwtPlotCurve *curve)
{
    if (this->radioButton_txt->isChecked())
    {
        saveCurrentCurveTxt(curve);
    }
    else
    {
        saveCurrentCurveXlsx(curve);
    }
}

void MainWindow::saveCurrentCurveTxt(int index)
{
    QwtPlotCurve *curve = m_curveList.at(index);
    if (NULL == curve)
        return;

    QString fileName = QString("%1/Curve%2_%3.txt").arg(this->lineEdit_autoSavePath->text()).arg(index+1).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.close();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.seek(file.size());
        int xmax = curve->maxXValue();
        for(int i=0;i<xmax;i++)
        {
            //QString tt = QString::number(curve_ref->data()->sample(i).y(),);
            stream << curve->data()->sample(i).x() << "," << curve->data()->sample(i).y() << "\n";
        }

        file.close();
    }
}

void MainWindow::saveCurrentCurveXlsx(int index)
{
    QwtPlotCurve *curve = m_curveList.at(index);
    if (NULL == curve)
        return;

    QString fileName = QString("%1/Curve%2_%3.xlsx").arg(this->lineEdit_autoSavePath->text()).arg(index+1).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    //打开excel
    QXlsx::Document xlsx;
    QXlsx::Format format1;
    format1.setFontSize(12);/*设置字体大小*/
    format1.setFontBold(true);

    QXlsx::Format format2;
    format2.setFontSize(10);/*设置字体大小*/

    //先写入标题
    QStringList titles;
    titles << "序号" << "值";

    for(int i = 0; i < titles.size(); ++i){
        xlsx.write(1,i+1,QVariant(titles.at(i)),format1);
    }

    //然后逐行的写入
    int xmax = curve->maxXValue();
    for(int i = 0; i < xmax; ++i){
        xlsx.write(i+2,1,QVariant(curve->data()->sample(i).x()),format2);
        xlsx.write(i+2,2,QVariant(curve->data()->sample(i).y()),format2);
    }


    //保存到文件
    bool ok = xlsx.saveAs(fileName);
    qDebug() << fileName;
}

void MainWindow::saveCurrentCurveTxt(QwtPlotCurve *curve)
{
    QString fileName = QString("%1/Curve_single_%2.txt").arg(this->lineEdit_autoSavePath->text()).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.close();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.seek(file.size());
        int xmax = curve->maxXValue();
        for(int i=0;i<xmax;i++)
        {
            //QString tt = QString::number(curve_ref->data()->sample(i).y(),);
            stream << curve->data()->sample(i).x() << "," << curve->data()->sample(i).y() << "\n";
        }

        file.close();
    }
}

void MainWindow::saveCurrentCurveXlsx(QwtPlotCurve *curve)
{
    QString fileName = QString("%1/Curve_single_%2.xlsx").arg(this->lineEdit_autoSavePath->text()).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    //打开excel
    QXlsx::Document xlsx;
    QXlsx::Format format1;
    format1.setFontSize(12);/*设置字体大小*/
    format1.setFontBold(true);

    QXlsx::Format format2;
    format2.setFontSize(10);/*设置字体大小*/

    //先写入标题
    QStringList titles;
    titles << "序号" << "值";

    for(int i = 0; i < titles.size(); ++i){
        xlsx.write(1,i+1,QVariant(titles.at(i)),format1);
    }

    //然后逐行的写入
    int xmax = curve->maxXValue();
    for(int i = 0; i < xmax; ++i){
        xlsx.write(i+2,1,QVariant(curve->data()->sample(i).x()),format2);
        xlsx.write(i+2,2,QVariant(curve->data()->sample(i).y()),format2);
    }


    //保存到文件
    bool ok = xlsx.saveAs(fileName);
    qDebug() << fileName;
}

//void MainWindow::on_actionAutoSave_triggered(bool checked)
//{
//    qDebug() << "checked==="<<checked;
//    if (checked)
//    {
//        this->actionAutoSave->setIcon(QIcon(":/images/radioCheck.png"));
//    }
//    else
//    {
//        this->actionAutoSave->setIcon(QIcon(":/images/radioUncheck.png"));
//    }
//}

//void MainWindow::on_checkBox_clicked(bool checked)
//{
//    if (checked)
//    {
//        if(CCommonInterface::get_datacom()->isBuildCommt())
//        {
//            if (m_SendTimer->isActive())
//            {
//                int ret = QMessageBox::question(this, tr("间隔设置"),tr("已经设置间隔时间， 是否重新设置?"),QMessageBox::Yes,QMessageBox::No);
//                if (QMessageBox::Yes == ret)
//                {
//                    m_SendTimer->stop();
//                }
//                else
//                {
//                    return;
//                }
//            }

//            //获取间隔时间
//            int interval = this->spinBox->value();//s
//            qDebug() << "m_SendTimer->start(interval):" << interval;
//            m_SendTimer->start(interval*1000);
//        }
//        else
//        {
//            QMessageBox::warning(this, tr("警告"),tr("请先连接!"));
//        }
//    }
//    else
//    {
//        m_SendTimer->stop();
//    }
//}

void MainWindow::on_radioButton_start_toggled(bool checked)
{
    if (!checked)
    {
        return;
    }

    if(true/*CCommonInterface::get_datacom()->isBuildCommt()*/)
    {
        if (m_SendTimer->isActive())
        {
            int ret = QMessageBox::question(this, tr("间隔设置"),tr("已经设置间隔时间， 是否重新设置?"),QMessageBox::Yes,QMessageBox::No);
            if (QMessageBox::Yes == ret)
            {
                m_SendTimer->stop();
            }
            else
            {
                return;
            }
        }

        //获取间隔时间
        int interval = this->spinBox->value();//ms
        qDebug() << "m_SendTimer->start(interval):" << interval;
        m_SendTimer->start(interval);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"),tr("请先连接!"));
        this->radioButton_close->setChecked(true);
    }
}

void MainWindow::on_radioButton_close_toggled(bool checked)
{
    m_SendTimer->stop();
}

void MainWindow::on_radioButton_manual_toggled(bool checked)
{

}

void MainWindow::on_radioButton_auto_toggled(bool checked)
{

}

void MainWindow::on_radioButton_txt_toggled(bool checked)
{

}

void MainWindow::on_radioButton_excel_toggled(bool checked)
{

}

//void MainWindow::on_radioButton_single_toggled(bool checked)
//{
//    for (int i = 0; i < m_curveList.size(); ++i)
//    {
//        QwtPlotCurve *curve = m_curveList.at(i);
//        curve->detach();
//    }
//    this->curve_ref->attach(m_pPlot);
//}

//void MainWindow::on_radioButton_multi_toggled(bool checked)
//{
//    this->curve_ref->detach();
//    for (int i = 0; i < m_curveList.size(); ++i)
//    {
//        QwtPlotCurve *curve = m_curveList.at(i);
//        curve->attach(m_pPlot);
//    }

//}

void MainWindow::on_pushButton_clear_clicked()
{
//    if (this->radioButton_single->isChecked())
//    {
//        QVector<double> xData;
//        QVector<double> yData;
//        curve_ref->setSamples(xData,yData);

//    }
//    else
    {
        for(int i = 0; i < m_curveDataList.size(); ++i)
        {
            QPair< QVector<double>*, QVector<double>* >* pair = m_curveDataList.at(i);
            pair->first->clear();
            pair->second->clear();
        }

        for (int i = 0; i < m_curveList.size(); ++i)
        {
            QwtPlotCurve *curve = m_curveList.at(i);
            QPair< QVector<double>*, QVector<double>* >* pair = m_curveDataList.at(i);
            curve->setSamples((*pair->first),(*pair->second));
        }
    }

    m_pPlot->replot();
}

void MainWindow::on_pushButton_autoSavePath_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("选择自动保存路径"),this->lineEdit_autoSavePath->text());
    if (dirPath.isEmpty())
        return;

    qDebug() << "dirPath==="<<dirPath;

    m_settings->setValue("autoSave/path",dirPath);

    this->lineEdit_autoSavePath->setText(dirPath);
}

void MainWindow::on_pushButton_start_udp_clicked()
{
    emit signal_start_udp(lineEdit_ip->text(),lineEdit_port->text());

    QMessageBox::information(this,tr("提示"), tr("连接udp成功"));
}

void MainWindow::on_pushButton_cmd_clicked()
{
    emit signal_send_cmd(lineEdit_cmd->text());
    QMessageBox::information(this,tr("提示"), tr("发送成功"));
}
