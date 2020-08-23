#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QString>
#include <QFontDatabase>
#include <QFont>
#include <QDesktopServices>

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
        setting_dialog=nullptr;
        simple_window=nullptr;
    } catch (std::bad_alloc &bad_alloc) {
        QMessageBox::critical(this,"错误","无法创建番茄钟，程序终止！","确定");
        abort();
    }
    //tmt::pomodoro::print_tomato_file(username);
    set_fonts();//引入字体
    flush_tomato();//刷新番茄计数
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(&flush_timer,SIGNAL(timeout()),this,SLOT(flush_clock()));
    connect(&tomato_timer,SIGNAL(timeout()),this,SLOT(tomato_ring()));
    connect(&rest_timer,SIGNAL(timeout()),this,SLOT(rest_ring()));
    connect(&ring_timer,SIGNAL(timeout()),this,SLOT(tomato_ring_stop()));
    connect(ui->btn_print,SIGNAL(clicked(bool)),this,SLOT(print_tomato()));
    connect(ui->btn_setting,SIGNAL(clicked(bool)),this,SLOT(setting()));
    connect(ui->btn_simple,SIGNAL(clicked(bool)),this,SLOT(simple()));

}

MainWindow::~MainWindow()
{
    tmt_clock->stop_ring();
    //TODO:事件重写，让关闭的时候询问是否保存
    delete setting_dialog;
    delete simple_window;
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
    ring_timer.stop();//停止响铃计时
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
    ring_timer.stop();//停止响铃计时
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
    ui->label_title->setText("稍事休息");
    ring_timer.start(30000);
    try {
        tmt_clock->tomato_ring();
    } catch (tmt::exception &exc) {
        end_t();
        exc.show_massage_box(exc.errorname()+"请更改媒体文件地址信息！",QMessageBox::Critical,"错误");
        setting_dialog->exec();
    }
}

void MainWindow::rest_ring(){
    ui->label_title->setText("开始工作");
    try {
        tmt_clock->rest_ring();
    } catch (tmt::exception &exc) {
        end_r();
        exc.show_massage_box(exc.errorname()+"请更改媒体文件地址信息！",QMessageBox::Critical,"错误");
        setting_dialog->exec();
    }
}

void MainWindow::tomato_ring_stop(){
    ring_timer.stop();
    tmt_clock->stop_ring();
    ui->label_title->setText("工作中");
}

void MainWindow::set_fonts(){
    //TODO:字体文件缺失的异常处理

    int Sans_Heavy_Id = QFontDatabase::addApplicationFont(":/fonts/heavy");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString Sans_Heavy_Family = QFontDatabase::applicationFontFamilies ( Sans_Heavy_Id ).at(0);
    QFont Sans_Heavy(Sans_Heavy_Family,140);
    ui->label_clock->setFont(Sans_Heavy);
    Sans_Heavy.setPointSize(72);
    ui->label_title->setFont(Sans_Heavy);
    Sans_Heavy.setPointSize(26);
    ui->btn_start->setFont(Sans_Heavy);
    ui->btn_end->setFont(Sans_Heavy);

    int Sans_Bold_Id = QFontDatabase::addApplicationFont(":/fonts/bold");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString Sans_Bold_Family = QFontDatabase::applicationFontFamilies ( Sans_Bold_Id ).at(0);
    QFont Sans_Bold(Sans_Bold_Family,20);
    ui->label_t_total->setFont(Sans_Bold);
    ui->label_t_today->setFont(Sans_Bold);
    ui->label_t_continue->setFont(Sans_Bold);
    ui->label_goal->setFont(Sans_Bold);
    ui->label_t_total_title->setFont(Sans_Bold);
    ui->label_t_today_title->setFont(Sans_Bold);
    ui->label_t_continue_title->setFont(Sans_Bold);
    ui->label_goal_title->setFont(Sans_Bold);

    /*int fas_Id = */QFontDatabase::addApplicationFont(":/fonts/fas");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    //QString fas_Family = QFontDatabase::applicationFontFamilies ( fas_Id ).at(0);
    //QFont fas(fas_Family,32);
    //ui->btn_setting->setFont(fas);
    //ui->btn_print->setFont()
}

void MainWindow::setting(){
    if(setting_dialog==nullptr){
        try{
            setting_dialog=new SettingDialog(tmt_clock,this);
        }catch (std::bad_alloc &bad_alloc) {
            QMessageBox::critical(this,"错误","无法创建设置窗口，程序终止！","确定");
            abort();
        }
    }
    ui->btn_setting->setChecked(false);
    setting_dialog->show();
}

void MainWindow::simple(){
    if(simple_window==nullptr){
        try{
            simple_window=new SimpleWindow(this,nullptr);
        }catch (std::bad_alloc &bad_alloc) {
            QMessageBox::critical(this,"错误","无法创建小窗口，程序终止！","确定");
            abort();
        }
    }
    simple_window->set_start_btn();
    simple_window->show();
    this->hide();
    ui->btn_simple->setChecked(false);
}

void MainWindow::print_tomato(){
    tmt_clock->print_tomato_file();
    if(!QMessageBox::information(this,"提示","打印成功！\n点击确定查看tomato_print.txt文档。","确定","取消")){
        QDesktopServices::openUrl ( QUrl::fromLocalFile(tmt_clock->user_filepath()+"tomato_print.txt") );
    }
    ui->btn_print->setChecked(false);

}

void MainWindow::test(){}

void MainWindow::click_end(){
    ui->btn_end->click();
}
