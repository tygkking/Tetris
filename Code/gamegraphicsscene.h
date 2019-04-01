#ifndef GAMEGRAPHICSSCENE_H
#define GAMEGRAPHICSSCENE_H


#include <QGraphicsScene>

/*
这个是一个游戏场景的类，主要是通过重载这个类，添加啦一些新的信号，这里就很方便在
第二层的箭头的Item上获取键盘事件，通过这个类进行转发，操作第四层的俄罗斯方块，类似于一通道。
这样就不会影响到其他的东西，而且模块化的思想很强
*/
class GameGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
signals:
    //改变当前的这个箭头Item是否被遮蔽的状态的信号
    void changeCoveredStateSignal( bool state ) ;


    /*******************这是定义的一些对方块进行移动变形操作的信号*******************/
    //通过这里进行中转然后传给GameAreaItem中进行处理
    void leftKeyDownSignal() ;
    void rightKeyDownSignal() ;
    void upKeyDownSignal() ;
    void downKeyDownSignal() ;
    void spaceKeyDownSignal() ;
    void pauseKeyDownSignal() ;             //按下p键的暂停游戏的信号

    //这个是中转的GameGameOverItem上面的重新开始按钮的点击的信号
    void restartButtonClickedSignal() ;
    //这个是中转的GameForeGroundItem上面的重新开始游戏返回到游戏首页的信号
    void backToGameHomeSignal() ;

    /**********************这个是当游戏被停用时的发送的信号********************/
    //这个是当游戏被停用时的发送的信号
    //就是当鼠标点击电脑桌面其他位置时，就是我们平常所说的整个游戏软件失去焦点时。发送这个信号
    void gameDeactivateSignal() ;

    /*************这个发送的改变游戏音乐与音效音量大小的信号*************/
    void changeVolumeSignal( int musicVolume , int audioVolume ) ;

    /***************************************************************
      这下面是当滑块落到底部和消除满行时的声音特效的控制信号
    ***************************************************************/
    //这个是满行时产生爆炸声音的信号
    void crushAudioSoundSignal() ;
    //这个是当滑块落到最低点时撞击的声音的信号
    void fellAudioSoundSignal() ;
public:
    explicit GameGraphicsScene(QObject *parent = 0);

public slots:

};

#endif // GAMEGRAPHICSSCENE_H
