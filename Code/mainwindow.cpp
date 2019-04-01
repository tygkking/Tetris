#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QPixmap>
#include <QTextCodec>
#include <QTime>
#include <QSplashScreen>
#include <QMessageBox>
#include "gamepage.h"

MainWindow::MainWindow(QWidget *parent) ://主页面
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    game = nullptr;
    ui->setupUi(this);
    setWindowTitle("Tetris");

    //left down time textzuo左下计时文本
    ui->statusBar->showMessage(tr("Welcome to this game!"));

    //right down forever text右下永久文本
    QLabel *permanent = new QLabel;
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanent->setText(tr("Copyright tygkking"));
    permanent->setTextFormat(Qt::RichText);
    permanent->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(permanent);

    //delete maxsize and sizechange
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    //buttoncolor
    ui->start->setStyleSheet("QPushButton:hover{background-color:rgb(255, 245, 225, 200)}");
    ui->about->setStyleSheet("QPushButton:hover{background-color:rgb(255, 245, 225, 200)}");
    ui->exit->setStyleSheet("QPushButton:hover{background-color:rgb(255, 245, 225, 200)}");
    ui->introduction->setStyleSheet("QPushButton:hover{background-color:rgb(255, 245, 225, 200)}");
}

MainWindow::~MainWindow()
{
    delete ui;
    if(game)
        game->close();
    delete game;
    game=nullptr;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event)
    {
        if(game)
            game->close();
        delete game;
        game=nullptr;
        close();
    }
}

void MainWindow::on_exit_clicked()
{
    if(game)
        game->close();
    delete game;
    game=nullptr;
    close();
}

void MainWindow::timerUpdate()//获取当前时间，这里并没有用到
{
   //获取系统现在的时间
   QDateTime time = QDateTime::currentDateTime();
   //设置系统时间显示格式
   QString str = time.toString("yyyy-MM-dd hh:mm:ss ");//dddd");
}

void MainWindow::paintEvent(QPaintEvent *)//绘制背景
{
    QPainter painter(this);
        QPixmap pix;
        pix.load("img/bg1.jpg");
        painter.drawPixmap(0, 0, width(), height()-37, pix);
}

void MainWindow::on_start_clicked()//打开游戏界面
{
    game = new gamepage;
    game->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)//相应按钮不同功能
{
    switch(event->key())
    {
    case Qt::Key_P:
        on_start_clicked();
        break;
    case Qt::Key_R:
        on_introduction_clicked();
        break;
    case Qt::Key_A:
        on_about_clicked();
        break;
    case Qt::Key_E:
        on_exit_clicked();
        break;
    default:
        break;
    }
}

void MainWindow::on_introduction_clicked()//游戏介绍
{
    QMessageBox mb;
    mb.setWindowTitle(tr("GameRule"));
    mb.setText(tr("GameRule:\n\n"
                  "1. 'left' and 'right':move blocks,\n"
                  "    'up':rotate the block,\n"
                  "    'down':speed up,\n"
                  "    'Space':to the bottom\n\n"
                  "2. You can choose different game type in 'Option' of GamePage\n\n"
                  "3. You can check more details in 'Help' of GamePage\n\n"));
    mb.setFont(QFont("Times New Roman",12,QFont::Bold));
    mb.exec();
//    "1.左右键移动方块，上键旋转方块，下键加速下降，空格键直接下降到底部\n"
//    "2.你可以在游戏界面的选项选择多种模式\n"
//    "3.你可以在游戏界面的帮助获取更多信息"

}

void MainWindow::on_about_clicked()//关于我们
{
//    QMessageBox::information(this, tr("About Us"),
//                    tr("Maker: tygkking \n Date:2017.7"),
//                      QMessageBox::Yes,QMessageBox::Yes);
    QMessageBox mb;
    mb.setWindowTitle(tr("About Us"));
    mb.setText(tr("Maker: tygkking \n\n"
                  "Date: 2017.7 \n"));
    mb.setFont(QFont("Times New Roman",12,QFont::Bold));
    mb.exec();
}
