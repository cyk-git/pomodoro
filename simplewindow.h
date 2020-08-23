#ifndef SIMPLEWINDOW_H
#define SIMPLEWINDOW_H

#include <QMainWindow>
#include <QTimer>


class MainWindow;

namespace Ui {
class SimpleWindow;
}

class SimpleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleWindow(MainWindow *main_w,QWidget *parent);
    void set_start_btn();
    ~SimpleWindow();

private slots:
    void on_btn_full_clicked();
    void flush();
    void start();
    void end();

//    void on_btn_end_clicked();

//    void on_btn_top_clicked(bool checked);

private:
    Ui::SimpleWindow *ui;
    MainWindow *main_window;
    QTimer flush_timer;
    void set_fonts();
};

#endif // SIMPLEWINDOW_H
