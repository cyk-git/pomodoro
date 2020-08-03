#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include "pomodoro.h"
#include "exception.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void flush_tomato();//刷新番茄计数显示
    ~MainWindow();

private slots:
    void start();
    void rest();
    void end_t();
    void end_r();
    void flush_clock();
    void tomato_ring();
    void rest_ring();
    void tomato_ring_stop();

private:
    Ui::MainWindow *ui;
    tmt::pomodoro *tmt_clock;
    QTime basetime;
    QTimer flush_timer,tomato_timer,rest_timer,ring_timer;
};
#endif // MAINWINDOW_H
