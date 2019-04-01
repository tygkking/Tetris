#ifndef MYEVENT_H
#define MYEVENT_H

#include <QDialog>

namespace Ui {
class myevent;
}

class myevent : public QDialog
{
    Q_OBJECT

public:
    explicit myevent(QWidget *parent = 0);
    ~myevent();

private:
    Ui::myevent *ui;
};

#endif // MYEVENT_H
