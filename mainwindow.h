#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include "settingdialog.h"
#include "pomodoro.h"
#include "exception.h"
#include "simplewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void flush_tomato();//刷新番茄计数显示
    Ui::MainWindow *get_ui(){return ui;}
    bool isTomato(){return tomato_timer.isActive();}
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
    void setting();
    void simple();
    void print_tomato();
    void test();
    void click_end();

private:
    Ui::MainWindow *ui;
    tmt::pomodoro *tmt_clock;
    QTime basetime;
    QTimer flush_timer,tomato_timer,rest_timer,ring_timer;
    SettingDialog *setting_dialog;
    SimpleWindow *simple_window;
    void set_fonts();//设置各个部件的字体。TODO:字体文件缺失异常处理
};
#endif // MAINWINDOW_H
