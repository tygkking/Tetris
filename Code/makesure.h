#ifndef MAKESURE_H
#define MAKESURE_H

#include <QDialog>

namespace Ui {
class makesure;
}

class makesure : public QDialog
{
    Q_OBJECT

public:
    explicit makesure(QWidget *parent = 0);
    ~makesure();

private slots:
    void on_yes_clicked();

    void on_no_clicked();

private:
    Ui::makesure *ui;
};

#endif // MAKESURE_H
