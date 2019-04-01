#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsView>
#include "chooseoption.h"

class BoxGroup;
namespace Ui {
class gamepage;
}

class gamepage : public QMainWindow
{
    Q_OBJECT
public:
    explicit gamepage(QWidget *parent = 0);
    virtual~gamepage();
    void paintEvent(QPaintEvent *);
    int nowlines;
    void readrecord();//读取记录
public slots:
    void start_game();//开始游戏
    void clear_full_rows();//在需要新方块时清除满行
    void clear_HL_rows(int,int);//清除横行
    void clear_ZL_rows(int,int);//清除纵列
    void clear_Triple_rows(int,int);//清除3*3
    void move_box();//清除行后移动上方方块
    void game_over();//游戏结束
    void restart_game();//重新开始
    void finish_game();//返回游戏主界面
    void pause_game();//暂停游戏
    void return_game();//返回游戏
    void showoption();//显示设置
    void showhelp();//显示帮助
    void settype(int);//设置模式
    void setspeed(double);//设置初始速度
    void getfcnum(int);//设置由方块数量更改分数倍数
    void Gspecialclean(int,int,int);//获取piece特殊方块类型，x坐标，y坐标
    void Gclean();//清除排行榜
    void Gshowhighscore();//显示排行榜

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    int nowspeed;//获取当前速度来设置分数倍数
    Ui::gamepage *ui;
    QGraphicsScene *scene;
    QGraphicsScene *nextscene;
    QGraphicsLineItem *top_line;
    QGraphicsLineItem *bottom_line;
    QGraphicsLineItem *left_line;
    QGraphicsLineItem *right_line;
    BoxGroup *box_group;
    BoxGroup *next_box_group;
    chooseoption *pg;
    qreal game_speed;
    QList<int> rows;
    QGraphicsWidget *mask_widget;
    //首页和游戏中需要用到的各种按钮
    QGraphicsWidget *return_button;

    //显示人机交互的文本信息
    QGraphicsTextItem *game_pause_text;
    QGraphicsTextItem *game_over_text;

    void init_view();
    void init_game();
    void update_score(const int full_row_num = 0);


signals:
    void backtogame();
    void backtomenu();
    void restart();
    void help();
    void option();
    void exitgame();
    void start();
    void sendcount(int);//向piece发送数字令其求模随机变化以适应不同模式的状态
    void clean();
    void showhighscore();

private slots:
    void on_highscore_clicked();
    void on_cleanrecord_clicked();
};

#endif // GAMEPAGE_H
