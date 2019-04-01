#include "gamepage.h"
#include "ui_gamepage.h"
#include "piece.h"
#include <QIcon>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QLabel>
#include <QFileInfo>
#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "makesure.h"
#include "chooseoption.h"
#include "highscore.h"

static int INITSSPEED = 1000;//游戏初始化速度
int gamescore;//记录游戏分数
int speedup;//记录消除50行加速
double sp2;//由chooseoption返回的初始化速度倍数
double scoreFnum=1;//根据自由模式的方块数定义分数倍数(0.2,0.5...)
int freecnum=5;//自由模式选块数
int Jingdian=0, Free=0, Fun=0;//排行榜分数
int type=1;//目前模式

gamepage::gamepage(QWidget *parent) ://游戏界面
    QMainWindow(parent),
    ui(new Ui::gamepage)
{
    ui->setupUi(this);
    setWindowTitle("GamePage");
    init_view();
    //默认为经典模式
    gamescore=0;
    speedup=1;
    sp2=1;
    scoreFnum=1;
    emit sendcount(7);//默认为经典模式

    //delete maxsize and sizechange
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    //button hover color
    ui->start->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->restart->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->option->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->help->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->mainmenu->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->exit->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->pause->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->highscore->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");
    ui->cleanrecord->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");

    readrecord();//读取文件排行榜设置最高分
}

void gamepage::paintEvent(QPaintEvent *)//draw background
{
    QPainter painter(this);
        QPixmap pix;
        pix.load("img/bg2.jpg");
        painter.drawPixmap(0, 0, width(), height(), pix);
}

void gamepage::readrecord()//读取文件设置最高分
{
    QFile file1( "Jscore.txt" );//经典模式
    if ( file1.open( QIODevice::ReadOnly ) )//只读方式打开文件
    {
       QTextStream stream( &file1 );
       QString line;
       while ( !stream.atEnd() ) {
           line = stream.readLine();
           Jingdian = line.toInt();
       }
       file1.close();
    }
    QFile file2( "Frscore.txt" );//自由模式
    if ( file2.open( QIODevice::ReadOnly ) )
    {
       QTextStream stream( &file2 );
       QString line;
       while ( !stream.atEnd() ) {
           line = stream.readLine();
           Free = line.toInt();
       }
       file2.close();
    }
    QFile file3( "Funscore.txt" );//娱乐模式
    if ( file3.open( QIODevice::ReadOnly ) )
    {
       QTextStream stream( &file3 );
       QString line;
       while ( !stream.atEnd() ) {
           line = stream.readLine();
           Fun = line.toInt();
       }
       file3.close();
    }
}

void gamepage::init_view()//初始化游戏界面
{
    INITSSPEED = 1000;
    ui->view->setRenderHint(QPainter::Antialiasing);//抗锯齿方式渲染
    scene = new QGraphicsScene;//新建场景
    nextscene = new QGraphicsScene;
    scene->setSceneRect(10,15,420,810);//场景大小
    ui->view->setBackgroundBrush(QPixmap("img/gamebg2.png"));
    ui->nextView->setBackgroundBrush(QPixmap("img/gamebg2.png"));
    nextscene->setSceneRect(0,0,180,180);
    ui->view->setScene(scene);//视图关联场景
    ui->view->show();//显示视图
    ui->nextView->setScene(nextscene);//下个块显示场景
    ui->nextView->show();

    //可移动区域外界的4条线,与外界预留3个像素是为了方便进行碰撞检测
    top_line = scene->addLine(14,14,426,14);
    bottom_line = scene->addLine(14,826,426,826);
    left_line = scene->addLine(14,14,14,826);
    right_line = scene->addLine(426,14,426,826);

    //添加当前方块组
    box_group = new BoxGroup;//通过新建BoxGroup对象间接达到调用box的2个类
    connect(box_group, SIGNAL(need_new_box()), this, SLOT(clear_full_rows()));//信号连接
    connect(box_group, SIGNAL(game_finished()), this, SLOT(game_over()));
    scene->addItem(box_group);
    box_group->shapenew();//默认为经典模式，初始化piece默认块都可以选择

    //添加提示方块组
    next_box_group = new BoxGroup;
    nextscene->addItem(next_box_group);
    next_box_group->shapenew();
    //隐藏边界
    top_line->hide();
    bottom_line->hide();
    left_line->hide();
    right_line->hide();

    //添加黑色遮罩
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 20)));//最后一个值是不透明度
    mask->resize(550,950);
    mask_widget = scene->addWidget(mask);
    mask_widget->setPos(-49,-49);
    mask_widget->setZValue(1);//该层放在原图的上层

    //游戏暂停文本
    game_pause_text = scene->addText("游戏暂停");
    game_pause_text->setHtml(tr("<font color=black>游戏暂停</font>"));
    game_pause_text->setFont(QFont("华文楷体", 30, QFont::Bold));
    game_pause_text->setPos(60,200);//(300, 100);
    game_pause_text->setZValue(2);//放在第2层
    game_pause_text->hide();

    //游戏结束文本
    game_over_text = scene->addText("GAME OVER");
    game_over_text->setHtml(tr("<font color=red>GAME OVER</font>"));
    game_over_text->setFont(QFont("Times New Roman", 25, QFont::Bold));
    game_over_text->setPos(10,400);//(300, 100);
    game_over_text->setZValue(2);//放在第2层
    game_over_text->hide();

    // 游戏中使用的非ui按钮
    QPushButton *button8 = new QPushButton(tr("Back To Game(B)"));
    button8->setFont(QFont("Times New Roman", 12, QFont::Bold));
    button8->setStyleSheet("QPushButton:hover{background-color:rgb(230, 225, 100, 150)}");

    //按钮信号 this指自身
    connect(ui->start, SIGNAL(clicked()), this, SLOT(start_game()));//开始游戏
    connect(this, SIGNAL(start()), this, SLOT(start_game()));

    connect(ui->option, SIGNAL(clicked()), this, SLOT(showoption()));//显示设置
    connect(this, SIGNAL(option()), this, SLOT(showoption()));

    connect(ui->help, SIGNAL(clicked()), this, SLOT(showhelp()));//显示帮助
    connect(this, SIGNAL(help()), this, SLOT(showhelp()));

    connect(ui->exit, SIGNAL(clicked()), this, SLOT(close()));//关闭
    connect(this, SIGNAL(exitgame()), this, SLOT(close()));

    connect(ui->restart, SIGNAL(clicked()), this, SLOT(restart_game()));//重新开始
    connect(box_group, SIGNAL(restart()), this, SLOT(restart_game()));//游戏进行时按键重新开始
    connect(this, SIGNAL(restart()), this, SLOT(restart_game()));//游戏结束时

    connect(ui->pause, SIGNAL(clicked()), this, SLOT(pause_game()));//暂停
    connect(box_group, SIGNAL(game_pause()),this,SLOT(pause_game()));

    connect(ui->mainmenu, SIGNAL(clicked()), this, SLOT(finish_game()));//返回游戏主界面
    connect(box_group, SIGNAL(backtomenu()), this, SLOT(finish_game()));
    connect(this, SIGNAL(backtomenu()),this,SLOT(finish_game()));

    connect(button8, SIGNAL(clicked()), this, SLOT(return_game()));//返回游戏
    connect(this,SIGNAL(backtogame()),this,SLOT(return_game()));

    connect(this, SIGNAL(sendcount(int)), box_group, SLOT(getcount(int)));//关于求模运算随机选择方块形状，确定选取前几个
    connect(box_group, SIGNAL(specialclean(int,int,int)), this, SLOT(Gspecialclean(int,int,int)));//特殊方块功能调用，三个值为功能类型，x坐标，y坐标

    connect(this, SIGNAL(clean()), this, SLOT(Gclean()));//清除排行榜
    connect(this, SIGNAL(showhighscore()), this, SLOT(Gshowhighscore()));//显示排行榜

    return_button = scene->addWidget(button8);
    return_button->setPos(100, 400);
    //将按钮放在第二层
    return_button->setZValue(2);

    //一部分按钮隐藏起来
    ui->restart->hide();
    ui->pause->hide();
    ui->mainmenu->hide();
    return_button->hide();
}

void gamepage::start_game()//开始游戏
{
    ui->start->hide();
    ui->option->hide();
    ui->help->hide();
    ui->exit->hide();
    ui->highscore->hide();
    ui->cleanrecord->hide();
    mask_widget->hide();
    init_game();//初始化游戏
}


void gamepage::init_game()//初始化游戏
{
    gamescore=0;
    speedup=1;
    sp2=1;
    scoreFnum=1;
    nowlines=0;//消除的行数
    ui->lcdNumber->display(0);//显示分数
    ui->scorenum->setNum(0);//显示行数
    box_group->create_box(QPointF(220,60));//创建并在中间位置处出现方块组
    box_group->shape_id_now=box_group->getCurrentShape();
    box_group->setFocus();//设置焦点，使用键盘控制
    box_group->startTimer(INITSSPEED);//启动初始速度定时器
    game_speed = INITSSPEED;//游戏速度
    next_box_group->create_box(QPoint(80, 80));//创建提示方块组

    ui->restart->show();
    ui->pause->show();
    ui->mainmenu->show();
    top_line->show();
    bottom_line->show();
    left_line->show();
    right_line->show();
    box_group->show();
    ui->view->setFocus();

}

void gamepage::clear_full_rows()
{
    for(int y = 779; y > 20; y -= 40)
    {
        //每隔40行取一个item出来，注意参数不能弄错，否则无消失效果
        QList<QGraphicsItem *> list = scene->items(QRectF(19,y,402,42), Qt::ContainsItemShape);//返回指定区域内所有可见的item
        if(list.count() == 10)
        {
            //如果一行已满，则销毁该行的所有小方块
            foreach(QGraphicsItem *item, list)
            {
                OneBox *box = (OneBox *) item;
                //采用动态效果消失
                QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;//创建模糊效果对象
                box->setGraphicsEffect(blur_effect);//给每个小方块添加模糊设置
                QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");//添加动态效果，尺寸变换效果
                animation->setEasingCurve(QEasingCurve::OutBounce);//为动态效果设置缓冲取曲线，是用来插值的
                animation->setDuration(100);//持续100毫秒
                animation->setStartValue(1.3);//其实尺寸
                animation->setEndValue(0.25);//结束尺寸
                animation->start(QAbstractAnimation::DeleteWhenStopped);//动画结束后删除该类
                connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));//动画结束完后才调用小方块销毁函数
            }
            rows << y;//将满行的行号保存到rows中
        }
    }

    if(rows.count()>0)//如果满行，则下移上面的方块
    {
        QTimer::singleShot(150, this, SLOT(move_box()));//只执行一次定时器，等动态效果完后再下移上面的方块
    }
    else//没有满行，则新出现提示方块，且提示方块出更新新的提示方块
    {
        box_group->create_box(QPointF(220,60), next_box_group->getCurrentShape());
        box_group->shape_id_now=next_box_group->getCurrentShape();
        next_box_group->clear_box_group(true);
        next_box_group->create_box(QPointF(80, 80));
    }
}

void gamepage::clear_HL_rows(int nx,int ny)//消除横行
{
    //取当前行
    QList<QGraphicsItem *> list = scene->items(QRectF(19,ny-41,402,42), Qt::ContainsItemShape);//返回指定区域内所有可见的item
    foreach(QGraphicsItem *item, list)
    {
        OneBox *box = (OneBox *) item;
        //采用动态效果消失方块行
        QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;//创建模糊效果对象
        box->setGraphicsEffect(blur_effect);//给每个小方块添加模糊设置
        QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");//添加动态效果，尺寸变换效果
        animation->setEasingCurve(QEasingCurve::OutBounce);//为动态效果设置缓冲取曲线，是用来插值的
        animation->setDuration(100);//持续100毫秒
        animation->setStartValue(1.3);//其实尺寸
        animation->setEndValue(0.25);//结束尺寸
        animation->start(QAbstractAnimation::DeleteWhenStopped);//动画结束后删除该类
        connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));//动画结束完后才调用小方块销毁函数
    }
    rows << ny-41;//将满行的行号保存到rows中
    if(rows.count()>0)//移动上方方块
    {
        QTimer::singleShot(150, this, SLOT(move_box()));//只执行一次定时器，等动态效果完后再下移上面的方块
    }
    else {
        //没有满行，则新出现提示方块，且提示方块出更新新的提示方块
        box_group->create_box(QPointF(220,60), next_box_group->getCurrentShape());
        box_group->shape_id_now=next_box_group->getCurrentShape();
        next_box_group->clear_box_group(true);
        next_box_group->create_box(QPointF(80, 80));//
    }
}

void gamepage::clear_ZL_rows(int nx,int ny)//消除纵列
{
    QList<QGraphicsItem *> list = scene->items(QRectF(nx-1,19,42,802), Qt::ContainsItemShape);//返回指定区域内所有可见的item
    foreach(QGraphicsItem *item, list)
    {
        OneBox *box = (OneBox *) item;
        //采用动态效果消失方块行
        QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;//创建模糊效果对象
        box->setGraphicsEffect(blur_effect);//给每个小方块添加模糊设置
        QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");//添加动态效果，尺寸变换效果
        animation->setEasingCurve(QEasingCurve::OutBounce);//为动态效果设置缓冲取曲线，是用来插值的
        animation->setDuration(100);//持续100毫秒
        animation->setStartValue(1.3);//其实尺寸
        animation->setEndValue(0.25);//结束尺寸
        animation->start(QAbstractAnimation::DeleteWhenStopped);//动画结束后删除该类
        connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));//动画结束完后才调用小方块销毁函数
    }
    update_score(1);
    box_group->create_box(QPointF(220,60), next_box_group->getCurrentShape());
    box_group->shape_id_now=next_box_group->getCurrentShape();
    next_box_group->clear_box_group(true);
    next_box_group->create_box(QPointF(80, 80));
}

void gamepage::clear_Triple_rows(int nx,int ny)//消除3*3方块
{
    QList<QGraphicsItem *> list = scene->items(QRectF(nx-41,ny-81,122,122), Qt::ContainsItemShape);//返回指定区域内所有可见的item
    foreach(QGraphicsItem *item, list)
    {
        OneBox *box = (OneBox *) item;
        //采用动态效果消失方块行
        QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;//创建模糊效果对象
        box->setGraphicsEffect(blur_effect);//给每个小方块添加模糊设置
        QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");//添加动态效果，尺寸变换效果
        animation->setEasingCurve(QEasingCurve::OutBounce);//为动态效果设置缓冲取曲线，是用来插值的
        animation->setDuration(100);//持续100毫秒
        animation->setStartValue(1.3);//其实尺寸
        animation->setEndValue(0.25);//结束尺寸
        animation->start(QAbstractAnimation::DeleteWhenStopped);//动画结束后删除该类
        connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));//动画结束完后才调用小方块销毁函数
    }
    update_score(1);
    box_group->create_box(QPointF(220,60), next_box_group->getCurrentShape());
    box_group->shape_id_now=next_box_group->getCurrentShape();
    next_box_group->clear_box_group(true);
    next_box_group->create_box(QPointF(80, 80));
}

void gamepage::move_box()//有满行则下移
{
    for(int i = rows.count(); i > 0; --i)
    {
        int row = rows.at(i-1);//取出满行的行号,从最上面的位置开始
        //取出从区域上边界到当前满行之间所形成的矩形区域
        foreach(QGraphicsItem *item, scene->items(QRectF(19,19,402,row-17), Qt::ContainsItemShape))
        {
            item->moveBy(0, 40);
        }
    }
    //更新分数
    update_score(rows.count());
    //出现新的方块组
    rows.clear();
    box_group->create_box(QPointF(220,60), next_box_group->getCurrentShape());
    box_group->shape_id_now=next_box_group->getCurrentShape();
    next_box_group->clear_box_group(true);
    next_box_group->create_box(QPoint(80, 80));
}


void gamepage::update_score(const int full_row_num)//增加分数
{
    getfcnum(freecnum);//获取方块数来判断相应分数倍数
    long score = full_row_num*10*(speedup*sp2*scoreFnum);
    int current_score = gamescore;
    gamescore = current_score += score;
    nowlines += full_row_num;//当前行数
    ui->lcdNumber->display(current_score);
    ui->scorenum->setNum(nowlines);
    //change speed
    if(nowlines%50==0)//每50行，速度增加
    {
        speedup ++;
        game_speed = INITSSPEED / (speedup);
        box_group->stop_timer();//重新设置定时器参数
        box_group->startTimer(game_speed);
    }
}

void gamepage::game_over()//游戏结束
{

    //判断是否打破纪录，打破纪录则重新写入
    if(type==1)
    {
        if(gamescore>Jingdian)
        {
            QMessageBox::information(this,tr("提示"),tr("您已打破经典模式记录"));
            QString line;
            line = QString::number(gamescore, 10);
            QFile file4( "Jscore.txt" );
            if ( file4.open( QIODevice::WriteOnly ) ) {//只读模式
                QTextStream stream( &file4 );
                    stream << line;
                file4.close();
            }
        }
    }
    if(type==2)
    {
        if(gamescore>Free)
        {
            QMessageBox::information(this,tr("提示"),tr("您已打破自由模式记录"));
            QString line;
            line = QString::number(gamescore, 10);
            QFile file5( "Frscore.txt" );
            if ( file5.open( QIODevice::WriteOnly ) ) {//只读模式
                QTextStream stream( &file5 );
                    stream << line;
                file5.close();
            }
        }
    }
    if(type==3)
    {
        if(gamescore>Fun)
        {
            QMessageBox::information(this,tr("提示"),tr("您已打破娱乐模式记录"));
            QString line;
            line = QString::number(gamescore, 10);
            QFile file6( "Funscore.txt" );
            if ( file6.open( QIODevice::WriteOnly ) ) {//只读模式
                QTextStream stream( &file6 );
                    stream << line;
                file6.close();
            }
        }
    }
    box_group->clearFocus();
    ui->pause->hide();
    mask_widget->show();
    game_over_text->show();
}

void gamepage::restart_game()
{
    box_group->stop_timer();//中断游戏最主要的是停止方块下移的定时器工作
    box_group->clearFocus();
    makesure ms;//弹出确认对话框
    if(ms.exec()==QDialog::Accepted)
    {
        box_group->startTimer(game_speed);
        ui->view->setFocus();
        box_group->setFocus();
        mask_widget->hide();
        game_over_text->hide();
        //销毁当前方块组和当前方块中的所有小方块
        next_box_group->clear_box_group(true);
        box_group->clear_box_group();
        box_group->hide();
        foreach(QGraphicsItem *item, scene->items(QRectF(19,19,402,802),Qt::ContainsItemBoundingRect))
        {
            scene->removeItem(item);
            OneBox *box = (OneBox*)item;
            box->deleteLater();
        }
        init_game();
    }
    else
    {
        box_group->startTimer(game_speed);
        ui->view->setFocus();
        box_group->setFocus();
    }
}

void gamepage::finish_game()//返回游戏主界面
{
    box_group->stop_timer();//中断游戏最主要的是停止方块下移的定时器工作
    box_group->clearFocus();
    makesure ms;
    if(ms.exec()==QDialog::Accepted)
    {
        game_over_text->hide();
        ui->restart->hide();
        ui->pause->hide();
        ui->mainmenu->hide();
        top_line->hide();
        bottom_line->hide();
        left_line->hide();
        right_line->hide();
        next_box_group->clear_box_group(true);
        box_group->clear_box_group();
        box_group->hide();
        foreach(QGraphicsItem *item, scene->items(QRectF(19,19,402,802),Qt::ContainsItemBoundingRect))
        {
            scene->removeItem(item);
            OneBox *box = (OneBox*)item;
            box->deleteLater();
        }
        mask_widget->show();
        ui->start->show();
        ui->option->show();
        ui->help->show();
        ui->exit->show();
        ui->highscore->show();
        ui->cleanrecord->show();
        ui->cleanrecord->show();
        ui->scorenum->setNum(0);
        ui->lcdNumber->display(0);
    }
    else
    {
        if(ui->pause->isVisible())
        {
            box_group->startTimer(game_speed);
            ui->view->setFocus();
            box_group->setFocus();
        }
    }
}

void gamepage::pause_game()//暂停游戏
{
    box_group->stop_timer();//中断游戏最主要的是停止方块下移的定时器工作
    ui->restart->hide();
    ui->pause->hide();
    ui->mainmenu->hide();
    mask_widget->show();
    game_pause_text->show();
    return_button->show();
    box_group->clearFocus();
}

void gamepage::return_game()//返回游戏界面
{
    return_button->hide();
    game_pause_text->hide();
    mask_widget->hide();
    ui->restart->show();
    ui->pause->show();
    ui->mainmenu->show();
    box_group->startTimer(game_speed);
    ui->view->setFocus();
    box_group->setFocus();
}

void gamepage::showoption()//设置
{
    pg =  new chooseoption;//新建设置界面
    //连接设置界面返回信号与当前界面执行方案
    connect(pg, SIGNAL(returntype(int)), this, SLOT(settype(int)));
    connect(pg, SIGNAL(returnspeed(double)), this, SLOT(setspeed(double)));
    connect(pg, SIGNAL(CIshape()), box_group, SLOT(GIshape()));
    connect(pg, SIGNAL(CJshape()), box_group, SLOT(GJshape()));
    connect(pg, SIGNAL(CTshape()), box_group, SLOT(GTshape()));
    connect(pg, SIGNAL(CSshape()), box_group, SLOT(GSshape()));
    connect(pg, SIGNAL(CZshape()), box_group, SLOT(GZshape()));
    connect(pg, SIGNAL(CXshape()), box_group, SLOT(GXshape()));
    connect(pg, SIGNAL(CTianshape()), box_group, SLOT(GTianshape()));
    connect(pg, SIGNAL(CLJshape()), box_group, SLOT(GLJshape()));
    connect(pg, SIGNAL(CLshape()), box_group, SLOT(GLshape()));
    connect(pg, SIGNAL(shape_new()), box_group, SLOT(shapenewfromchoose()));
    connect(pg, SIGNAL(fcnum(int)), this, SLOT(getfcnum(int)));
    if(pg->exec()==QDialog::Accepted)
    {

    }
    delete pg;
    pg = nullptr;
}

void gamepage::Gclean()//获取清除排行榜信号
{
    ui->cleanrecord->clicked();
}

void gamepage::Gshowhighscore()//显示排行榜
{
    ui->highscore->clicked();
}

void gamepage::getfcnum(int m)//通过方块选择数更改分数倍数
{
    freecnum=m;
    switch (m)
    {
        case 1:
            scoreFnum = 0.2;
            break;
        case 2:
            scoreFnum = 0.4;
            break;
        case 3:
            scoreFnum = 0.6;
            break;
        case 4:
            scoreFnum = 0.8;
            break;
        case 5:
            scoreFnum = 1;
            break;
        case 6:
            scoreFnum = 1.2;
            break;
        case 7:
            scoreFnum = 1.4;
            break;
        case 8:
            scoreFnum = 1.6;
            break;
        case 9:
            scoreFnum = 2;
            break;
    }

}

void gamepage::settype(int style)//设置当前模式，通过模式设置初始化参数
{
    switch (style)
    {
    case 1:
        ui->nowmode->setText(tr("经典模式"));
        INITSSPEED = 1000;
        emit sendcount(7);
        freecnum=5;
        type=1;
        box_group->shapenew();
        break;
    case 2:
        ui->nowmode->setText(tr("自由模式"));
        emit sendcount(9);
        type=2;
        break;
    case 3:
        ui->nowmode->setText(tr("娱乐模式"));
        INITSSPEED = 1000;
        emit sendcount(12);
        freecnum=5;
        type=3;
        box_group->shapenew();
        break;
    case 0:
        ui->nowmode->setText(tr("经典模式"));
        INITSSPEED = 1000;
        emit sendcount(7);
        freecnum=5;
        type=1;
        box_group->shapenew();
        break;
    }
}

void gamepage::setspeed(double sp)//设置初始速度
{
    INITSSPEED = 1000;
    INITSSPEED /= sp;
    sp2=sp;
}

void gamepage::Gspecialclean(int t, int Nx, int Ny)//对于piece传回特殊方块类型确定消除类型
{
    switch (t)
    {
    case 1:
        clear_HL_rows(Nx,Ny);
        break;
    case 2:
        clear_ZL_rows(Nx,Ny);
        break;
    case 3:
        clear_Triple_rows(Nx,Ny);
        break;
    default:
        break;
    }
}

void gamepage::showhelp()//帮助界面
{
    QMessageBox help;
    help.setWindowTitle(tr("Help"));
    help.setText(tr("你可以选择多种模式\n\n"
                    "1.经典模式：只有最基础的方块，每次消除"
                    "50"
                    "行，初始速度加快，得分也相应增加"
                    "\n\n"
                    "2.自由模式：你可以自由选择方块和下落速度，但是消除得分会根据方块的选择以及速度的选择改变"
                    "\n\n"
                    "3.娱乐模式：会有多种方块，以及会有一些具有特殊功能的方块"));
    help.setFont(QFont("华文楷体",13,QFont::Bold));
    help.exec();
}

void gamepage::on_highscore_clicked()//排行榜
{
    readrecord();
    highscore hs;
    hs.setscore(Jingdian,Free,Fun);
    hs.exec();
}
void gamepage::on_cleanrecord_clicked()//清除记录，重新写入文件
{
    makesure ms;
    if(ms.exec()== QDialog::Accepted)
    {
        QString line;
        line = QString::number(0, 10);//十进制类型转化
        QFile file7( "Jscore.txt" );
        if ( file7.open( QIODevice::WriteOnly ) )
        {//只读模式
            QTextStream stream( &file7 );
                stream << line;
            file7.close();
        }

        line = QString::number(0, 10);
        QFile file8( "Frscore.txt" );
        if ( file8.open( QIODevice::WriteOnly ) )
        {//只读模式
            QTextStream stream( &file8 );
                stream << line;
            file8.close();
        }

        line = QString::number(0, 10);
        QFile file9( "Funscore.txt" );
        if ( file9.open( QIODevice::WriteOnly ) )
        {//只读模式
            QTextStream stream( &file9 );
                stream << line;
            file9.close();
        }
    }
}

void gamepage::keyPressEvent(QKeyEvent *event)//按键响应
{
    if(ui->pause->isVisible())//可以看到暂停按钮时，代表游戏还在运行，这时候的焦点给下降中的方块组
    {
        box_group->setFocus();
    }
    else
    {
        box_group->clearFocus();//如果游戏暂停，则方块组不能获得焦点
        if(ui->start->isVisible())//游戏主界面
        {
            switch (event->key())
            {
                case Qt::Key_H:
                    emit help();//发送帮助信号
                    break;
                case Qt::Key_O:
                    emit option();//发送设置信号
                    break;
                case Qt::Key_E:
                    emit exitgame();//发送退出信号
                    break;
                case Qt::Key_S:
                    emit start();//发送游戏开始信号
                    break;
                case Qt::Key_C:
                    emit clean();//发送清除排行榜信号
                    break;
                case Qt::Key_G:
                    emit showhighscore();//发送显示排行榜信号
                    break;
                default:
                    break;
            }
        }
        else if(ui->mainmenu->isVisible())//游戏结束界面
        {
            switch (event->key())
            {
                case Qt::Key_R:
                    emit restart();
                    break;
                case Qt::Key_M:
                    emit backtomenu();
                    break;
                default:
                    break;
            }
        }
        else//游戏暂停界面
        {
            switch (event->key())
            {
                case Qt::Key_B:
                    emit backtogame();
                    break;
                case Qt::Key_Space:
                    emit backtogame();
                    break;
                case Qt::Key_P:
                    emit backtogame();
                    break;
                default:
                    break;
            }
        }
    }
}

void gamepage::mousePressEvent(QMouseEvent *event)//避免误触，进行焦点设置
{
    ui->nextView->clearFocus();
    if(ui->pause->isVisible())
    {
        ui->view->setFocus();
        box_group->setFocus();
    }
    else
        box_group->clearFocus();
}

gamepage::~gamepage()
{
    delete ui;
}
