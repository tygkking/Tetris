#ifndef CHOOSEOPTION_H
#define CHOOSEOPTION_H

#include <QDialog>

namespace Ui {
class chooseoption;
}

class chooseoption : public QDialog
{
    Q_OBJECT

public:
    explicit chooseoption(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);//paint background
    int checkshape();//当自由模式时结束时检查选择的方块
    ~chooseoption();

private slots:
    void on_yes_clicked();//确定按钮点击
    void on_no_clicked();//取消按钮点击
    void on_choose_currentIndexChanged(int index);//改变了模式
    void on_speedbox_currentIndexChanged(int index);//改变了速度
    void on_Allcheck_clicked();//如果点击了全选
    //判断是否全选
    void checkeveryclick();
    void on_IShape_clicked();
    void on_JShape_clicked();
    void on_LongJShape_clicked();
    void on_LShape_clicked();
    void on_tianShape_clicked();
    void on_XShape_clicked();
    void on_SShape_clicked();
    void on_TShape_clicked();
    void on_ZShape_clicked();

signals:
    void returntype(int);//向gamepage返回类型初始化界面
    void returnspeed(double);//向gamepage返回初始速度

    //向piece返回选择的方块初始化自由模式
    void CIshape();
    void CLshape();
    void CJshape();
    void CXshape();
    void CTshape();
    void CLJshape();
    void CTianshape();
    void CSshape();
    void CZshape();
    void shape_new();
    void fcnum(int);//free choose num根据自由模式选块初始化基本分数

private:
    Ui::chooseoption *ui;
    int type;//记录类型
    double speed;//记录速度
};

#endif // CHOOSEOPTION_H
