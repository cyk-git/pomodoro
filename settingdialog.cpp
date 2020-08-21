#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QFileDialog>
#include <QMessageBox>

SettingDialog::SettingDialog(tmt::pomodoro *tmt,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    tmt_clock=tmt;
    ui->setupUi(this);
    init();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_btn_cancel_clicked()
{
    this->close();
}

void SettingDialog::on_btn_media_t_clicked()
{
    ui->lineEdit_media_t->setText(QFileDialog::getOpenFileName(this,tr("选择文件"),"./Media",tr("可能支持的音频文件(*.wav;*.mp3;*.ape;*.aac;*.ogg)\n所有文件(*.*)")));
}

void SettingDialog::on_btn_media_r_clicked()
{
    ui->lineEdit_media_r->setText(QFileDialog::getOpenFileName(this,tr("选择文件"),"./Media",tr("可能支持的音频文件(*.wav;*.mp3;*.ape;*.aac;*.ogg)\n所有文件(*.*)")));
}

void SettingDialog::on_btn_OK_clicked()
{
    int i=QMessageBox::question(this,"保存设置","是否永久保存设置？\n（是：永久保存；否：单次保存）","取消","否","是");
    if(i){
        get_setting();
        if(i==2){
            tmt_clock->set_setting_file();
        }
        this->close();
    }
}

void SettingDialog::get_setting(){
    //TODO:获取设置，检查文件可用性，输入pomodoro
    tmt::timeset t_set={ui->timeEdit_start_time->time(),
                        ui->spinBox_work_time->value(),
                        ui->spinBox_short_rest_time->value(),
                        ui->spinBox_long_rest_time->value(),
                        ui->spinBox_long_rest_trigger->value()};
    tmt::goal g_set={(int)ui->spinBox_g_total->value()*2,
                     (int)ui->spinBox_g_half->value()*2,
                     (int)ui->spinBox_g_zero->value()*2};
    QString mt_set=ui->lineEdit_media_t->text();
    QString mr_set=ui->lineEdit_media_r->text();
    try{
        tmt_clock->init_setting(t_set,g_set,mt_set,mr_set);
    }catch(...){
        abort();//TODO:异常处理
    }
}

void SettingDialog::init() const{
    ui->timeEdit_start_time->setTime(tmt_clock->start_time());
    ui->spinBox_work_time->setValue(tmt_clock->work_time()/(1000*60));
    ui->spinBox_short_rest_time->setValue(tmt_clock->short_rest_time()/(1000*60));
    ui->spinBox_long_rest_time->setValue(tmt_clock->long_rest_time()/(1000*60));
    ui->spinBox_long_rest_trigger->setValue(tmt_clock->long_rest_trigger());
    ui->spinBox_g_total->setValue(tmt_clock->goal_total());
    ui->spinBox_g_half->setValue(tmt_clock->goal_half());
    ui->spinBox_g_zero->setValue(tmt_clock->goal_zero());
    ui->lineEdit_media_t->setText(tmt_clock->media_t_path());
    ui->lineEdit_media_r->setText(tmt_clock->media_r_path());
}

void SettingDialog::on_btn_OK_2_clicked()
{
    int i=QMessageBox::question(this,"恢复默认","是否恢复默认设置？\n","否","是");
    if(i){
        tmt_clock->init_setting();
        init();
    }
}
