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
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(showtime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showtime(){
    static QTime qtime;
    static QString t;
    qtime=qtime.currentTime();
    t = qtime.toString("hh:mm");
    this->ui->Clock->setText(t);
}
