#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class CSetDialog;
}

class CSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSetDialog(QWidget *parent = 0);
    ~CSetDialog();

    QString GetTitle();
    int GetHistoryNum();
    QString GetSaveURL();
    int GetNameStye();
    QString GetPageStye();
    QString GetUnitStye();

signals:
    void SigCliHintData();
    void SigCurrentIndexChanged(int index);
public slots:

    void on_btnOK_clicked();
    void on_btnNO_clicked();
    void on_btnCliHint_clicked();
    void on_btnChoseUrl_clicked();
    void on_m_CB_Name1_currentIndexChanged(int index);
    void on_m_CB_Name2_currentIndexChanged(int index);


private:
    Ui::CSetDialog *ui;
};

#endif // SETDIALOG_H
