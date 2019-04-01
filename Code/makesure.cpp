#include "makesure.h"
#include "ui_makesure.h"

makesure::makesure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::makesure)
{
    ui->setupUi(this);
    setWindowTitle("Sure?");

    //隐藏问号
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}

makesure::~makesure()
{
    delete ui;
}

void makesure::on_yes_clicked()
{
    accept();
}

void makesure::on_no_clicked()
{
    close();
}
