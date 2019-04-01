#include "chooseoption.h"
#include "ui_chooseoption.h"
#include <QPainter>
#include <QMessageBox>

chooseoption::chooseoption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseoption)
{
    ui->setupUi(this);
    setWindowTitle("Option");
    type=0;
    speed=1;

    //隐藏问号
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    //隐藏控件
    ui->speed->hide();
    ui->speedbox->hide();
    ui->type->hide();
    ui->IShape->hide();
    ui->JShape->hide();
    ui->ZShape->hide();
    ui->XShape->hide();
    ui->SShape->hide();
    ui->LShape->hide();
    ui->TShape->hide();
    ui->LongJShape->hide();
    ui->tianShape->hide();
    ui->Allcheck->hide();
    ui->Ip->hide();
    ui->Jp->hide();
    ui->Zp->hide();
    ui->Sp->hide();
    ui->Tp->hide();
    ui->Op->hide();
    ui->Xp->hide();
    ui->LJp->hide();
    ui->Lp->hide();
    ui->Ip->setIcon(QPixmap("img/I.png"));
    ui->Ip->setIconSize(QSize(70,70));
    ui->Jp->setIcon(QPixmap("img/J.png"));
    ui->Jp->setIconSize(QSize(70,70));
    ui->Sp->setIcon(QPixmap("img/S.png"));
    ui->Sp->setIconSize(QSize(70,70));
    ui->Xp->setIcon(QPixmap("img/X.png"));
    ui->Xp->setIconSize(QSize(70,70));
    ui->Zp->setIcon(QPixmap("img/Z.png"));
    ui->Zp->setIconSize(QSize(70,70));
    ui->Lp->setIcon(QPixmap("img/L.png"));
    ui->Lp->setIconSize(QSize(70,70));
    ui->Op->setIcon(QPixmap("img/O.png"));
    ui->Op->setIconSize(QSize(70,70));
    ui->Tp->setIcon(QPixmap("img/T.png"));
    ui->Tp->setIconSize(QSize(70,70));
    ui->LJp->setIcon(QPixmap("img/LJ.png"));
    ui->LJp->setIconSize(QSize(70,70));
}

chooseoption::~chooseoption()
{
    delete ui;
}

void chooseoption::paintEvent(QPaintEvent *)//draw background
{
    QPainter painter(this);
        QPixmap pix;
        pix.load("img/bg2.jpg");
        painter.drawPixmap(0, 0, width(), height(), pix);
}

void chooseoption::on_yes_clicked()
{
    emit returntype(type);
    emit returnspeed(speed);
    if(type==2)
        emit shape_new();//向piece返回指令，默认自由模式不选块
    if(checkshape()==1)//至少选择了一个块
    accept();
}

void chooseoption::on_no_clicked()
{
    close();
}

int chooseoption::checkshape()
{
    if(type!=2)
    {
        return 1;
    }
    else
    {
        int num=0;
        if(ui->IShape->isChecked())
        {
            num++;
            emit CIshape();
        }
        if(ui->JShape->isChecked())
        {
            num++;
            emit CJshape();
        }
        if(ui->LongJShape->isChecked())
        {
            num++;
            emit CLJshape();
        }
        if(ui->LShape->isChecked())
        {
            num++;
            emit CLshape();
        }
        if(ui->SShape->isChecked())
        {
            num++;
            emit CSshape();
        }
        if(ui->ZShape->isChecked())
        {
            num++;
            emit CZshape();
        }
        if(ui->XShape->isChecked())
        {
            num++;
            emit CXshape();
        }
        if(ui->tianShape->isChecked())
        {
            num++;
            emit CTianshape();
        }
        if(ui->TShape->isChecked())
        {
            num++;
            emit CTshape();
        }
        if(num==0&&type==2)
        {
            QMessageBox::critical(this,tr("Warning"),
                                  tr("You didn't choose any block"));
            return 0;
        }
        emit fcnum(num);
        return 1;
    }
}

void chooseoption::checkeveryclick()
{
    int num2=0;
    if(type==2)
    {
        if(ui->IShape->isChecked())
        {
            num2++;
        }
        if(ui->JShape->isChecked())
        {
            num2++;
        }
        if(ui->LongJShape->isChecked())
        {
            num2++;
        }
        if(ui->LShape->isChecked())
        {
            num2++;
        }
        if(ui->SShape->isChecked())
        {
            num2++;
        }
        if(ui->ZShape->isChecked())
        {
            num2++;
        }
        if(ui->XShape->isChecked())
        {
            num2++;
        }
        if(ui->tianShape->isChecked())
        {
            num2++;
        }
        if(ui->TShape->isChecked())
        {
            num2++;
        }
        if(num2 < 9 && ui->Allcheck->isChecked())
        {
            ui->Allcheck->setChecked(false);
        }
        else if(num2==9 && !ui->Allcheck->isChecked())
        {
            ui->Allcheck->setChecked(true);
        }
    }
}

void chooseoption::on_choose_currentIndexChanged(int index)
{
      if(index==1)//经典
      {
          ui->mode->setNum(index);
          ui->speed->hide();
          ui->speedbox->hide();
          ui->type->hide();
          ui->IShape->hide();
          ui->JShape->hide();
          ui->ZShape->hide();
          ui->XShape->hide();
          ui->SShape->hide();
          ui->LShape->hide();
          ui->TShape->hide();
          ui->LongJShape->hide();
          ui->tianShape->hide();
          ui->Allcheck->hide();
          ui->Ip->hide();
          ui->Jp->hide();
          ui->Zp->hide();
          ui->Sp->hide();
          ui->Tp->hide();
          ui->Op->hide();
          ui->Xp->hide();
          ui->LJp->hide();
          ui->Lp->hide();
          ui->mode->setText(tr("经典模式：只有初始块，最经典玩法"));
      }
      else if(index==2)//自由
      {
          ui->mode->setNum(index);
          ui->speed->show();
          ui->speedbox->show();
          ui->type->show();
          ui->IShape->show();
          ui->JShape->show();
          ui->ZShape->show();
          ui->XShape->show();
          ui->SShape->show();
          ui->LShape->show();
          ui->TShape->show();
          ui->LongJShape->show();
          ui->tianShape->show();
          ui->Allcheck->show();
          ui->Ip->show();
          ui->Jp->show();
          ui->Zp->show();
          ui->Sp->show();
          ui->Tp->show();
          ui->Op->show();
          ui->Xp->show();
          ui->LJp->show();
          ui->Lp->show();
          ui->mode->setText(tr("自由模式：自定义初始速度以及可下降的方块"));

      }
      else if(index==3)//娱乐
      {
          ui->mode->setNum(index);
          ui->speed->hide();
          ui->speedbox->hide();
          ui->type->hide();
          ui->IShape->hide();
          ui->JShape->hide();
          ui->ZShape->hide();
          ui->XShape->hide();
          ui->SShape->hide();
          ui->LShape->hide();
          ui->TShape->hide();
          ui->LongJShape->hide();
          ui->tianShape->hide();
          ui->Allcheck->hide();
          ui->Ip->hide();
          ui->Jp->hide();
          ui->Zp->hide();
          ui->Sp->hide();
          ui->Tp->hide();
          ui->Op->hide();
          ui->Xp->hide();
          ui->LJp->hide();
          ui->Lp->hide();
          ui->mode->setText(tr("娱乐模式：有神奇的方块，白色小块消除一行，\n"
                               "黑色小块消除一列，"
                               "金色小块消除掉边长为"
                               "3"
                               "的正方形"));
      }
      else
      {
          ui->mode->setNum(index);
          ui->speed->hide();
          ui->speedbox->hide();
          ui->type->hide();
          ui->IShape->hide();
          ui->JShape->hide();
          ui->ZShape->hide();
          ui->XShape->hide();
          ui->SShape->hide();
          ui->LShape->hide();
          ui->TShape->hide();
          ui->LongJShape->hide();
          ui->tianShape->hide();
          ui->Allcheck->hide();
          ui->Ip->hide();
          ui->Jp->hide();
          ui->Zp->hide();
          ui->Sp->hide();
          ui->Tp->hide();
          ui->Op->hide();
          ui->Xp->hide();
          ui->LJp->hide();
          ui->Lp->hide();
          ui->mode->setText(tr("State"));
      }
      type=index;

}

void chooseoption::on_speedbox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0://choose
        speed=1;
        break;
    case 1://x1
        speed=1;
        break;
    case 2://x1.5
        speed=1.5;
        break;
    case 3://x2
        speed=2;
        break;
    case 4://x2.5
        speed=2.5;
        break;
    case 5://x3
        speed=3;
        break;
    case 6://x4
        speed=4;
        break;
    }
}

void chooseoption::on_Allcheck_clicked()//全选or全不选
{
    if(ui->Allcheck->isChecked())//选中未被选中的
    {
        if(!ui->IShape->isChecked())
            ui->IShape->click();
        if(!ui->JShape->isChecked())
            ui->JShape->click();
        if(!ui->LongJShape->isChecked())
            ui->LongJShape->click();
        if(!ui->LShape->isChecked())
            ui->LShape->click();
        if(!ui->SShape->isChecked())
            ui->SShape->click();
        if(!ui->ZShape->isChecked())
            ui->ZShape->click();
        if(!ui->XShape->isChecked())
            ui->XShape->click();
        if(!ui->tianShape->isChecked())
            ui->tianShape->click();
        if(!ui->TShape->isChecked())
            ui->TShape->click();
    }
    else//退选已被选中的
    {
        if(ui->IShape->isChecked())
            ui->IShape->click();
        if(ui->JShape->isChecked())
            ui->JShape->click();
        if(ui->LongJShape->isChecked())
            ui->LongJShape->click();
        if(ui->LShape->isChecked())
            ui->LShape->click();
        if(ui->SShape->isChecked())
            ui->SShape->click();
        if(ui->ZShape->isChecked())
            ui->ZShape->click();
        if(ui->XShape->isChecked())
            ui->XShape->click();
        if(ui->tianShape->isChecked())
            ui->tianShape->click();
        if(ui->TShape->isChecked())
            ui->TShape->click();
    }
}

void chooseoption::on_IShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_JShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_LongJShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_LShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_tianShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_XShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_SShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_TShape_clicked()
{
    checkeveryclick();
}

void chooseoption::on_ZShape_clicked()
{
    checkeveryclick();
}
