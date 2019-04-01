#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QTime>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;//新建主页面
    Dialog dlg;//新建欢迎页面
    if(dlg.exec()==QDialog::Accepted)
    {
        w.show();
        w.move ((QApplication::desktop()->width() - w.width())/2,
                (QApplication::desktop()->height() - w.height())/2);
        return a.exec();
    }
    else return 0;
}
