#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamepage.h"
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);//paint
    void closeEvent(QCloseEvent *);//右上close事件
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_exit_clicked();
    void timerUpdate();
    void on_start_clicked();
    void on_introduction_clicked();
    void on_about_clicked();
private:
    Ui::MainWindow *ui;
    gamepage *game;
};

#endif // MAINWINDOW_H
