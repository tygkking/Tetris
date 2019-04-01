#include "highscore.h"
#include "ui_highscore.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDialog>
#include <QDialogButtonBox>
#include <QString>
#include "makesure.h"

highscore::highscore(QWidget *parent) ://最高分系统
    QDialog(parent),
    ui(new Ui::highscore)
{
    ui->setupUi(this);
    ui->setupUi(this);
    setWindowTitle("High Scores");

    //隐藏问号
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}

highscore::~highscore()
{
    delete ui;
}

void highscore::setscore(int a,int b,int c)//设置页面分数
{
    ui->Jingdian->setNum(a);
    ui->Free->setNum(b);
    ui->Fun->setNum(c);
}

void highscore::on_pushButton_clicked()
{
    close();
}
