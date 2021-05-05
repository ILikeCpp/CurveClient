#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#define VERSIONS "1.05.081"
namespace Ui {
class CAboutDialog;
}

class CAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAboutDialog(QWidget *parent = 0);
    ~CAboutDialog();

private:
    Ui::CAboutDialog *ui;
};

#endif // ABOUTDIALOG_H
