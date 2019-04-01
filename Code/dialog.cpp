#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QPainter>

Dialog::Dialog(QWidget *parent) ://欢迎界面
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("WELCOME");

    //隐藏问号
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_login_clicked()
{
    accept();
}
