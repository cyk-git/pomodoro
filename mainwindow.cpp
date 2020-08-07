#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString username;
    //TODO:账号控制组件
    username="default";
    try {
        tmt_clock=new tmt::pomodoro(username);
    } catch (std::bad_alloc &bad_alloc) {
        QMessageBox::critical(this,"错误","无法创建番茄钟，程序终止！","确定");
        abort();
    }
    tmt::pomodoro::print_tomato(username);
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(&flush_timer,SIGNAL(timeout()),this,SLOT(flush_clock()));
    connect(&tomato_timer,SIGNAL(timeout()),this,SLOT(tomato_ring()));
    connect(&rest_timer,SIGNAL(timeout()),this,SLOT(rest_ring()));
    connect(&ring_timer,SIGNAL(timeout()),this,SLOT(tomato_ring_stop()));
}

MainWindow::~MainWindow()
{
    tmt_clock->stop_ring();
    delete tmt_clock;
    delete ui;
}

void MainWindow::start(){
    ui->btn_start->disconnect();//断开开始按钮信号与槽
    rest_timer.stop();//结束休息计时
    basetime=basetime.currentTime();//设置基准时间
    tomato_timer.start(tmt_clock->work_time());//开始番茄计时
    flush_timer.start(100);//开始时钟刷新
    tmt_clock->stop_ring();//停止响铃
    ui->label_title->setText("工作中");//更改标题
    ui->btn_start->setText("休息");//更改开始按钮为休息按钮
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(rest()));//链接休息按钮信号与槽
    ui->btn_end->disconnect();//断开结束按钮信号与槽
    connect(ui->btn_end,SIGNAL(clicked(bool)),this,SLOT(end_t()));//链接结束按钮信号与槽
    ui->btn_end->setCheckable(true);//启用结束按钮
}

void MainWindow::rest(){
    ui->btn_start->disconnect();//断开休息按钮信号与槽
    ring_timer.stop();//结束番茄响铃计时
    tomato_timer.stop();//结束番茄计时

    int tomato_gain=basetime.msecsTo(QTime::currentTime());//获取番茄计时时长(毫秒)
    tmt_clock->tomato_gain(tomato_gain/(tmt_clock->work_time()));//收获番茄
//TODO:此处可能抛出文件异常！！
    flush_tomato();//刷新番茄计数显示

    basetime=basetime.currentTime();//设置基准时间
    if(tmt_clock->is_long_rest()){
        rest_timer.start(tmt_clock->long_rest_time());//开始长休息计时
    }else{
        rest_timer.start(tmt_clock->short_rest_time());//开始短休息计时
    }
    flush_timer.start(100);//开始时钟刷新
    tmt_clock->stop_ring();//停止响铃
    ui->label_title->setText("休息中");//更改标题
    ui->btn_start->setText("开始");//更改休息按钮为开始按钮
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(start()));//链接开始按钮信号与槽
    ui->btn_end->disconnect();//断开结束按钮信号与槽
    connect(ui->btn_end,SIGNAL(clicked(bool)),this,SLOT(end_r()));//链接结束按钮信号与槽
}

void MainWindow::end_t(){
    ui->btn_start->disconnect();//断开休息按钮信号与槽
    tomato_timer.stop();//结束番茄计时

    int tomato_gain=basetime.msecsTo(QTime::currentTime());//获取番茄计时时长(毫秒)
    tmt_clock->tomato_gain(tomato_gain/(tmt_clock->work_time()));//收获番茄
//TODO:此处可能抛出文件异常！！
    flush_tomato();//刷新番茄计数显示

    flush_timer.stop();//开始时钟刷新
    tmt_clock->stop_ring();//停止响铃
    ui->label_clock->setText("00:00:00");//时钟置零
    ui->label_title->setText("准备工作");//更改标题
    ui->btn_start->setText("开始");//更改开始按钮为休息按钮
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(start()));//链接休息按钮信号与槽
    ui->btn_end->setCheckable(false);//禁用结束按钮
}

void MainWindow::end_r(){
    rest_timer.stop();//结束休息计时
    flush_timer.stop();//开始时钟刷新
    tmt_clock->stop_ring();//停止响铃
    ui->label_clock->setText("00:00:00");//时钟置零
    ui->label_title->setText("准备工作");//更改标题
    ui->btn_end->setCheckable(false);//禁用结束按钮
}

void MainWindow::flush_clock(){
    //int t=basetime.msecsTo(QTime::currentTime());//获取计时时长(毫秒)
    QTime time(0,0,0,0);
    time=time.addMSecs(basetime.msecsTo(QTime::currentTime()));
    ui->label_clock->setText(time.toString("hh:mm:ss"));
}

void MainWindow::flush_tomato(){
    ui->label_t_total->setNum(tmt_clock->tomato_total());
    ui->label_t_today->setNum(tmt_clock->t_today());
    ui->label_t_continue->setNum(tmt_clock->t_continue());
    ui->label_goal->setNum(tmt_clock->goal_today());
}

void MainWindow::tomato_ring(){
    tmt_clock->tomato_ring();
    ui->label_title->setText("稍事休息");
    ring_timer.start(30000);
}

void MainWindow::rest_ring(){
    tmt_clock->rest_ring();
    ui->label_title->setText("开始工作");
}

void MainWindow::tomato_ring_stop(){
    ring_timer.stop();
    tmt_clock->stop_ring();
    ui->label_title->setText("工作中");
}
