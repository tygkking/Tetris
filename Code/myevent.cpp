#include "myevent.h"
#include "ui_myevent.h"

myevent::myevent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myevent)
{
    ui->setupUi(this);
}

myevent::~myevent()
{
    delete ui;
}
