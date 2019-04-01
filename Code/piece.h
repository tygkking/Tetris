#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsObject>
#include <QGraphicsItemGroup>

class OneBox : public QGraphicsObject
{
public:
    OneBox(const QColor &color = Qt::red);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

private:
    QColor brushColor;

};


class BoxGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum BoxShape {IShape, JShape, LShape, OShape, SShape, TShape, ZShape, XShape, LJShape, HLineShape, ZLineShape, TripleShape, RandomShape};//俄罗斯方块形状
    BoxGroup();
    QRectF boundingRect() const;//不改变类的成员
    void clear_box_group(bool destroy_box = false);
    void create_box(const QPointF &point, BoxShape shape = RandomShape);//设定为默认值，定义处不需要
    bool isColliding();
    BoxShape getCurrentShape() {return current_shape;}//获得当前的形状
    void keyPressEvent(QKeyEvent *event);
    int chooseshape();//判断形状是否被选
    void shapenew();//经典，娱乐初始化块
    int checknowbox();//当前块是否为特殊功能块
    int shape_id_now;//当前是哪个特殊功能块

protected:


signals:
    void need_new_box();
    void game_finished();
    void game_pause();
    void backtomenu();
    void restart();
    void help();
    void option();
    void exitgame();
    void start();
    void specialclean(int,int,int);//发送特殊功能块信号，来执行相应清除操作

public slots:
    void move_one_step();
    void startTimer(int interval);
    void stop_timer();
    void getcount(int);
    void GIshape();
    void GLshape();
    void GJshape();
    void GXshape();
    void GTshape();
    void GLJshape();
    void GTianshape();
    void GSshape();
    void GZshape();
    void shapenewfromchoose();//自由模式初始化块

private:
    BoxShape current_shape;
    QTransform old_transform;
    QTimer *timer;
    int angle;

};


#endif // PIECE_H
