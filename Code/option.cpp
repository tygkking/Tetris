#include "chooseoption.h"
#include "ui_chooseoption.h"

chooseoption::chooseoption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseoption)
{
    ui->setupUi(this);
}

chooseoption::~chooseoption()
{
    delete ui;
}

void chooseoption::on_yes_clicked()
{
    accept();
}
