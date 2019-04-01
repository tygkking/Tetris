#include "piece.h"
#include "gamepage.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QDateTime>
#include <QGraphicsItemGroup>

int num=7;//求模运算需要，经典模式默认7，取前7个方块，自由模式默认9，娱乐模式默认12
int i=1,j=1,z=1,s=1,X=1,lj=1,t=1,tian=1,l=1;//初始化均为不可取，经典模式和娱乐模式在之前会对这里初始化
int shape_id;//方块形状
int freechoosenum=9;//自由模式方块选择数，目前没有用到


OneBox::OneBox(const QColor &color) : brushColor(color)//OneBox是从QGraphicsObject继承而来的
{
}


QRectF OneBox::boundingRect() const//该函数为指定后面的绘图区域的外边框
{
    qreal pen_width = 1;
    //小方块的边长为40.5像素
    return QRectF(-20-pen_width/2,-20-pen_width/2,40+pen_width/2,40+pen_width/2)/*(-10-pen_width/2, -10-pen_width/2, 20+pen_width, 20+pen_width)*/;

}

void OneBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //贴图，看起来有质感
    painter->drawPixmap(-20,-20,40,40, QPixmap("img/box.gif"));
    painter->setBrush(brushColor);//设置画刷颜色
    QColor penColor = brushColor;
    penColor.setAlpha(20);//将颜色的透明度减小，使方框边界和填充色直接能区分开
    painter->setPen(penColor);//色绘制画笔
    //这里画矩形框，框内填充部分用画刷画，框外线条用画笔画
    painter->drawRect(-20,-20,40,40);//画矩形框
}

QPainterPath OneBox::shape() const//在局部坐标点上返回item的shape,但是好像没有其它地方调用了该函数
{
    QPainterPath path;//QPainterPath是一个绘图操作的容器
    path.addRect(-19.5,-19.5,39,39);
    return path;
}

BoxGroup::BoxGroup()//BoxGroup是从QGraphicsItemGroup，QObject继承而来的
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//随机数种子为当前时间，确保每次方块顺序不同
    setFlags(QGraphicsItem::ItemIsFocusable);//允许设置输入焦点
    old_transform = transform();//返回当前item的变换矩阵,当BoxGroup进行旋转后，可以使用它来进行恢复
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move_one_step()));
    current_shape = RandomShape;
}

QRectF BoxGroup::boundingRect() const
{
    qreal pen_width = 1;
    return QRectF(-80-pen_width/2, -80-pen_width/2, 160+pen_width, 160+pen_width);//2*2个小方块组成一个小方块组
}

void BoxGroup::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        //向上变形
        case Qt::Key_Up:
            //如果是一些旋转不会改变形状的块，就禁用旋转
            if(getCurrentShape()==OShape||getCurrentShape()==XShape||getCurrentShape()==HLineShape
                ||getCurrentShape()==ZLineShape||getCurrentShape()==TripleShape)
            break;
            angle+=90;
            setRotation(angle);//该函数的旋转角度是相对于界面来说，对原图形旋转，所以用90，180，270，360
            if(isColliding())//I型块需要特殊判断
            {
                moveBy(40,0);
                if(isColliding())
                {
                    if(current_shape==IShape)
                    {
                        moveBy(40,0);
                        if(isColliding())
                        {
                            moveBy(-120,0);
                            if(isColliding())
                            {
                                moveBy(-40,0);
                                if(isColliding())
                                {
                                    moveBy(80,0);
                                    angle-=90;
                                    setRotation(angle);//变形后碰撞了，就逆向变形回去
                                }
                            }
                        }
                    }
                    else
                    {
                        moveBy(-40,0);
                        moveBy(-40,0);
                        if(isColliding())
                        {
                            moveBy(40,0);
                            angle-=90;
                            setRotation(angle);//变形后碰撞了，就逆向变形回去
                        }
                    }
                }
            }
            break;
        //向下键位下降
        case Qt::Key_Down:
            moveBy(0, 40);//moveBy是系统自带的函数，不需要我们自己去实现
            if(isColliding())
            {
                moveBy(0, -40);
                if(checknowbox()>0)
                {
                    clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
                    emit specialclean(checknowbox(),pos().x(),pos().y());

                }
                else
                {
                    clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
                    emit need_new_box();//发射信号，在MyView中接收
                }
            }
            //clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
            break;
        case Qt::Key_Left:
            moveBy(-40, 0);
            if(isColliding())
            {
                moveBy(40, 0);
            }
            break;
        case Qt::Key_Right:
            moveBy(40, 0);
            if(isColliding())
            {
                moveBy(-40, 0);
            }
            break;
         //实现方块组坠落
        case Qt::Key_Space:
            moveBy(0, 40);//moveBy是系统自带的函数，不需要我们自己去实现
            while(!isColliding())
            {
                moveBy(0, 40);
            }
            moveBy(0, -40);//往回跳
            if(checknowbox()>0)
            {
                clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
                emit specialclean(checknowbox(),pos().x(),pos().y());

            }
            else
            {
                clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
                emit need_new_box();//发射信号，在MyView中接收
            }
            break;
        case Qt::Key_P:
            emit game_pause();//游戏暂停
            break;
        case Qt::Key_M:
            emit backtomenu();//返回游戏主页面
            break;
        case Qt::Key_R:
            emit restart();//重新开始
            break;
            default:
            break;
    }
}


bool BoxGroup::isColliding()//检测是否有碰撞
{
    QList<QGraphicsItem *> item_list = childItems();//返回子item列表
    QGraphicsItem *item;
    foreach(item, item_list)
    {
        if(item->collidingItems().count()>1)//collidingItems返回与当前item碰撞的子item列表
            return true;//代表至少有一个item发生了碰撞
    }
    return false;
}

//将方块组从视图中移除掉，如果有需要(即参数为true的情况下)则销毁掉
//其本质是将所有的小方块从方块组中移除掉，达到从视图中将方块组移除的目的
void BoxGroup::clear_box_group(bool destroy_box)
{
    QList<QGraphicsItem *> item_list = childItems();
    QGraphicsItem *item;
    foreach(item, item_list)
    {
        removeFromGroup(item);//将item从方块组中移除掉
        if(destroy_box)
        {
            OneBox *box = (OneBox*)item;
            box->deleteLater();//当控制返回到事件循环时，该目标被删除，即销毁
        }
    }
}



void BoxGroup::create_box(const QPointF &point, BoxShape shape)//创建俄罗斯方块组，根据形状参数选择方块组的颜色和形状
{
    angle=0;
    setRotation(0);
    static const QColor color_table[12] = {
        QColor(255, 0, 0, 100), QColor(255, 230, 0, 100), QColor(0, 0, 230, 100),
        QColor(0, 180, 0, 100), QColor(0, 255, 255, 100), QColor(200, 0, 200, 100),
        QColor(150, 100, 100, 100), QColor(150, 150, 150, 100), QColor(230, 150, 20, 100),
        QColor(245, 245, 245, 100), QColor(0, 0, 0, 100), QColor(255, 215, 0, 100)
    };
    shape_id = shape; //Box_Shape是枚举型，其实也是整型，因为它相当于整型的宏定义
    if(shape == RandomShape)
    {
        while(chooseshape()==0 || shape_id==RandomShape)
        shape_id = qrand()%num;//随机取一个颜色
    }
    QColor color = color_table[shape_id];//根据id选颜色
    QList<OneBox *> list;
    setTransform(old_transform);//恢复方块组前的变换矩阵
    if(shape_id < 7)
    for(int i = 0; i < 4; ++i)//4个小方块组成一个方块组
    {
        OneBox *temp  = new OneBox(color);
        list << temp;//将小方块加入list列表
        addToGroup(temp);
    }
    else if(shape_id < 9)
    {
        for(int i = 0; i < 5; ++i)//5个小方块组成一个方块组
        {
            OneBox *temp  = new OneBox(color);
            list << temp;//将小方块加入list列表
            addToGroup(temp);
        }
    }
    else
    {
        OneBox *temp  = new OneBox(color);
        list << temp;//将小方块加入list列表
        addToGroup(temp);
    }
    switch(shape_id)
    {
        case IShape:
            current_shape = IShape;//一型
            list.at(0)->setPos(-60,-20);//(-30, -10);
            list.at(1)->setPos(-20, -20);//(-10, -10);
            list.at(2)->setPos(20, -20);//(10, -10);
            list.at(3)->setPos(60, -20);//(30, -10);
            break;
        case JShape:
            current_shape = JShape;//J型
            list.at(0)->setPos(20, -20);//(10, -10);
            list.at(1)->setPos(20, 20);//(10, 10);
            list.at(2)->setPos(20, 60);//(10, 30);
            list.at(3)->setPos(-20, 60);//(-10, 30);
            break;
        case LShape:
            current_shape = LShape;//L型
            list.at(0)->setPos(-20, -20);//(-10, -10);
            list.at(1)->setPos(-20, 20);//(-10, 10);
            list.at(2)->setPos(-20, 60);//(-10, 30);
            list.at(3)->setPos(20, 60);//(10, 30);
            break;
        case OShape://田字型
            current_shape = OShape;
            list.at(0)->setPos(-20, -20);//(-10, -10);
            list.at(1)->setPos(20, -20);//(10, -10);
            list.at(2)->setPos(-20, 20);//(-10, 10);
            list.at(3)->setPos(20, 20);//(10, 10);
            break;
        case SShape://S型
            current_shape = SShape;
            list.at(0)->setPos(20, -20);//(10, -10);
            list.at(1)->setPos(60, -20);//(30, -10);
            list.at(2)->setPos(-20, 20);//(-10, 10);
            list.at(3)->setPos(20, 20);//(10, 10);
            break;
        case TShape: //T型
            current_shape = TShape;
            list.at(0)->setPos(-20, -20);//(-10, -10);
            list.at(1)->setPos(20, -20);//(10, -10);
            list.at(2)->setPos(60, -20);//(30, -10);
            list.at(3)->setPos(20, 20);//(10, 10);
            break;
        case ZShape://Z型
            current_shape = ZShape;
            list.at(0)->setPos(-20, -20);//(-10, -10);
            list.at(1)->setPos(20, -20);//(10, -10);
            list.at(2)->setPos(20, 20);//(10, 10);
            list.at(3)->setPos(60, 20);//(30, 10);
            break;
        case XShape://十型
            current_shape = XShape;
            list.at(0)->setPos(-20, 20);//(-10, -10);
            list.at(1)->setPos(20, 20);//(10, -10);
            list.at(2)->setPos(60, 20);//(30, -10);
            list.at(3)->setPos(20, 60);//(10, 10);
            list.at(4)->setPos(20,-20);
            break;
        case LJShape://LongJ型
            current_shape = LJShape;
            list.at(0)->setPos(20, -20);//(10, -10);
            list.at(1)->setPos(20, 20);//(10, 10);
            list.at(2)->setPos(20, 60);//(10, 30);
            list.at(3)->setPos(-20, 60);//(-10, 30);
            list.at(4)->setPos(-60, 60);
            break;
        case HLineShape://横消除
            current_shape = HLineShape;
            list.at(0)->setPos(20,-20);
            break;
        case ZLineShape://纵消除
            current_shape = ZLineShape;
            list.at(0)->setPos(20,-20);
            break;
        case TripleShape://3x3消除
            current_shape = TripleShape;
            list.at(0)->setPos(20,-20);
            break;
        default:
            break;
    }
    setPos(point);//将准备好的俄罗斯方块放入指定的位置，然后进行碰撞检测
    if(isColliding())
    {
        //如果俄罗斯方块一出现后就发生了碰撞，因为它是从中间出来的，所以一开始不可能是与左右两边发生碰撞，
        //只能是与下面碰撞，因此如果发生了碰撞，说明游戏已经结束，就可以发送游戏结束信号了，且定时器停止。
        stop_timer();
        emit game_finished();
    }
}

void BoxGroup::getcount(int n)
{
    num = n;
}

void BoxGroup::shapenew()//经典模式和娱乐模式初始化所有块都被选
{
    i=2; j=2; s=2; X=2; lj=2; t=2; tian=2; l=2; z=2;
}

void BoxGroup::shapenewfromchoose()//自由模式初始化块是否被选
{
    i=1; j=1; s=1; X=1; lj=1; t=1; tian=1; l=1; z=1;
}

int BoxGroup::chooseshape()//判断随机到的形状是否合适
{
    if(i!=2 && shape_id==IShape)
    {
        return 0;
    }
    if(l!=2 && shape_id==LShape)
    {
        return 0;
    }
    if(j!=2 && shape_id==JShape)
    {
        return 0;
    }
    if(s!=2 && shape_id==SShape)
    {
        return 0;
    }
    if(z!=2 && shape_id==ZShape)
    {
        return 0;
    }
    if(X!=2 && shape_id==XShape)
    {
        return 0;
    }
    if(lj!=2 && shape_id==LJShape)
    {
        return 0;
    }
    if(tian!=2 && shape_id==OShape)
    {
        return 0;
    }
    if(t!=2 && shape_id==TShape)
    {
        return 0;
    }
    else
    return 1;
}

//对于选中的方块初始化1为不可选，2为已经被选择
void BoxGroup::GIshape()
{
    i=2;
}

void BoxGroup::GLshape()
{
    l=2;
}

void BoxGroup::GJshape()
{
    j=2;
}

void BoxGroup::GXshape()
{
    X=2;
}

void BoxGroup::GZshape()
{
    z=2;
}

void BoxGroup::GSshape()
{
    s=2;
}

void BoxGroup::GTshape()
{
    t=2;
}

void BoxGroup::GTianshape()
{
    tian=2;
}

void BoxGroup::GLJshape()
{
    lj=2;
}

int BoxGroup::checknowbox()//判断当前下落方块的形状，如果为特殊功能，返回相应值
{
    if(shape_id_now == 9)//HLineShape
    {
        return 1;
    }
    if(shape_id_now == 10)//ZLineShape
    {
        return 2;
    }
    if(shape_id_now == 11)//TripleShape
    {
        return 3;
    }
    return 0;
}

//这个是系统里的函数，本程序中是在主函数中启动的
//其实是该子类中的timeEvent()函数调用的
void BoxGroup::startTimer(int interval)
{
    timer->start(interval);//启动定时器并且设置定时器间隔，然后在BoxGroup()的构造函数中设置了该定时器的信号与槽函数
}


//每当定时器到时间了，小方块组就向下移一步
void BoxGroup::move_one_step()
{
    moveBy(0, 40);//该函数是父类的函数，这里指向下移动一个单位，因为向下为正坐标
    if(isColliding())
    {//发生碰撞的情况下
        moveBy(0, -40);
        if(checknowbox()>0)
        {
            clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
            emit specialclean(checknowbox(),pos().x(),pos().y());

        }
        else
        {
            clear_box_group();//到达底部后就将当前方块组的4个item移除,不销毁方块组
            emit need_new_box();//发射信号，在MyView中接收
        }
    }
}

void BoxGroup::stop_timer()
{
    timer->stop();//定时器停止
}

