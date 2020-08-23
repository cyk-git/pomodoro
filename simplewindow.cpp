#include "simplewindow.h"
#include "ui_simplewindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFont>

SimpleWindow::SimpleWindow(MainWindow *main_w,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleWindow),
    main_window(main_w)
{
    ui->setupUi(this);
    set_fonts();
    setWindowFlags(Qt::WindowStaysOnTopHint);
    flush_timer.start(100);
    connect(&flush_timer,SIGNAL(timeout()),this,SLOT(flush()));
    set_start_btn();
    connect(ui->btn_print,SIGNAL(clicked(bool)),main_window,SLOT(print_tomato()));
    connect(ui->btn_setting,SIGNAL(clicked(bool)),main_window,SLOT(setting()));
    connect(ui->btn_end,SIGNAL(clicked(bool)),this,SLOT(end()));
    connect(ui->btn_end,SIGNAL(clicked(bool)),main_window,SLOT(click_end()));
}

SimpleWindow::~SimpleWindow()
{
    delete ui;
}

void SimpleWindow::on_btn_full_clicked()
{
    main_window->show();
    this->hide();
    ui->btn_full->setChecked(false);
}

void SimpleWindow::flush(){
    ui->label_clock->setText(main_window->get_ui()->label_clock->text());
    ui->label_title->setText(main_window->get_ui()->label_title->text());
    ui->label_t_total->setText(main_window->get_ui()->label_t_total->text());
    ui->label_t_today->setText(main_window->get_ui()->label_t_today->text());
    ui->label_t_continue->setText(main_window->get_ui()->label_t_continue->text());
    ui->label_goal->setText(main_window->get_ui()->label_goal->text());
    ui->btn_end->setCheckable(main_window->get_ui()->btn_end->isCheckable());
    ui->btn_start->setChecked(false);
    ui->btn_end->setChecked(false);
    ui->btn_print->setChecked(false);
    ui->btn_setting->setChecked(false);
}

//void SimpleWindow::on_btn_end_clicked()
//{
//    ui->btn_start->disconnect();
//    ui->btn_start->setText("");//将开始按钮设置为开始图标
//    connect(ui->btn_start,SIGNAL(clicked(bool)),main_window,SLOT(start()));
//}



void SimpleWindow::start(){
    ui->btn_start->disconnect();
    ui->btn_start->setText("");
    connect(ui->btn_start,SIGNAL(clicked(bool)),main_window,SLOT(rest()));
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(end()));
}

void SimpleWindow::end(){
    ui->btn_start->disconnect();
    ui->btn_start->setText("");
    connect(ui->btn_start,SIGNAL(clicked(bool)),main_window,SLOT(start()));
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(start()));
}

void SimpleWindow::set_start_btn(){
    if(main_window->isTomato()){
        start();
    }else{
        end();
    }
}

void SimpleWindow::set_fonts(){
    //TODO:字体文件缺失的异常处理

    int Sans_Heavy_Id = QFontDatabase::addApplicationFont(":/fonts/heavy");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString Sans_Heavy_Family = QFontDatabase::applicationFontFamilies ( Sans_Heavy_Id ).at(0);
    QFont Sans_Heavy(Sans_Heavy_Family,24);
    ui->label_clock->setFont(Sans_Heavy);
    ui->label_title->setFont(Sans_Heavy);

    int Sans_Bold_Id = QFontDatabase::addApplicationFont(":/fonts/bold");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString Sans_Bold_Family = QFontDatabase::applicationFontFamilies ( Sans_Bold_Id ).at(0);
    QFont Sans_Bold(Sans_Bold_Family,14);
    ui->label_t_total->setFont(Sans_Bold);
    ui->label_t_today->setFont(Sans_Bold);
    ui->label_t_continue->setFont(Sans_Bold);
    ui->label_goal->setFont(Sans_Bold);


    /*int fas_Id = */QFontDatabase::addApplicationFont(":/fonts/fas");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    //QString fas_Family = QFontDatabase::applicationFontFamilies ( fas_Id ).at(0);
    //QFont fas(fas_Family,32);
    //ui->btn_setting->setFont(fas);
    //ui->btn_print->setFont()
}

//void SimpleWindow::on_btn_top_clicked(bool checked)
//{
//    if(checked){
//        hide();
//        this->setWindowFlags(Qt::WindowStaysOnTopHint);
//        show();
//        ui->btn_top->setStyleSheet("QToolButton{\n	background: transparent;\n	color: rgba(255, 255, 0,200);\n	border:none;\n}\nQToolButton:hover,QToolButton:focus{\n	color: rgba(255, 255, 255,150);\n}\nQToolButton:pressed{\n	color: rgba(255, 255, 255,100);\n}\n");
//    }else{
//        hide();
//        setWindowFlags(Qt::Widget);
//        show();
//        ui->btn_top->setStyleSheet("QToolButton{\n	background: transparent;\n	color: rgba(255, 255, 255,150);\n	border:none;\n}\nQToolButton:hover,QToolButton:focus{\n	color: rgba(255, 255, 255,200);\n}\nQToolButton:pressed{\n	color: rgba(255, 255, 255,100);\n}\n");
//        ui->btn_top->setChecked(false);
//    }

//}
