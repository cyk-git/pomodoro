#ifndef POMODORO_H
#define POMODORO_H
#include <QString>
#include <QVector>
#include <QDateTime>

namespace tmt {
    struct tomato{
        int t_total;//记录昨日结余，仅在每日首次打开时结算改写
        int t_today;
        int t_continue;
    } ;//番茄计数
    struct timeset{
        int work_time;
        int long_rest_time;
        int short_rest_time;
        int long_rest_trigger;//长休息所需番茄数目
    } ;//设置
    struct goal{
        int g_total;//每日总目标
        int g_half;//番茄减半时长
        int g_zero;//无番茄收获时长
    } ;//目标
    struct mode{
        bool exercise_mode;//锻炼模式
        bool simple_mode;//精简模式
    } ;
    struct usage{
        int tomato;//消耗番茄数
        QString info;//详细信息
    };
    struct kind{
        QString kind;
        QVector<struct usage> usage_list;
    };
//以上为需要用到的结构体的定义

class pomodoro
{
private:
    QString username;//用户名

    struct tomato tomato;//番茄计数
    struct timeset timeset;//设置
    struct goal goal;//目标
    struct mode mode;//模式

    QVector<struct kind> kind_list;//结合结构体的定义，kind_list具有类型-具体列表两层结构

public:
    pomodoro(QString _username);//接受uesrname参数，读取"./user/username/"下相关文件进行初始化
    //TODO:账号管理系统

};

//下面是一些公用的函数，用于文件的读取
    int tomato_total(QString filepath);//将昨日结余与今日操作相加减，计算出总番茄结余
    QDate today();//判断在番茄钟语境下，今日的日期
    QDate today(QString filepath);//同上，且会首先判断是否有今日信息，是则返回今日日期,否则创建今日信息写入文件并返回今日日期
    //TODO:设置一日开始时间的模块设计
}

#endif // POMODORO_H
