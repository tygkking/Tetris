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
    ~chooseoption();

private slots:
    void on_yes_clicked();

private:
    Ui::chooseoption *ui;
};

#endif // CHOOSEOPTION_H
