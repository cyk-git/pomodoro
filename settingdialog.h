#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "pomodoro.h"
#include "exception.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(tmt::pomodoro *tmt,QWidget *parent = nullptr);
    void get_setting();
    ~SettingDialog();

private slots:
    void on_btn_cancel_clicked();

    void on_btn_media_t_clicked();

    void on_btn_media_r_clicked();

    void on_btn_OK_clicked();

    void on_btn_OK_2_clicked();

private:
    Ui::SettingDialog *ui;
    tmt::pomodoro *tmt_clock;
    void init() const;


};

#endif // SETTINGDIALOG_H
