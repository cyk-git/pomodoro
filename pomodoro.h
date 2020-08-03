#ifndef POMODORO_H
#define POMODORO_H
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QDataStream>
#include <QTextStream>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <cmath>

namespace tmt {
    struct tomato{
        int t_total;//记录昨日结余，仅在每日首次打开时结算改写
        int t_today;//记录今日获取
        int t_use;//记录今日使用
        int t_punish;//记录今日番茄惩罚
        int t_continue;//记录今日连续获得番茄数
    } ;//番茄计数
    struct timeset{
        QTime start_time;//设置一日起始时间
        int work_time;//单次工作时长
        int short_rest_time;//短休息时长
        int long_rest_time;//长休息时长
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

    //下面对这些结构体的流式传输进行重载
        QDataStream &operator<<(QDataStream &QDS,const struct tomato &tmt);
        QDataStream &operator>>(QDataStream &QDS,struct tomato &tmt);
        QDataStream &operator<<(QDataStream &QDS,const struct timeset &tset);
        QDataStream &operator>>(QDataStream &QDS,struct timeset &tset);
        QDataStream &operator<<(QDataStream &QDS,const struct goal &goal);
        QDataStream &operator>>(QDataStream &QDS,struct goal &goal);
        QDataStream &operator<<(QDataStream &QDS,const struct mode &mode);
        QDataStream &operator>>(QDataStream &QDS,struct mode &mode);

        //TODO:重载关于详细信息输出的>>运算符

class pomodoro{

private:
    QString username;//用户名

    struct tomato tomato;//番茄计数
    struct timeset timeset;//设置
    struct goal goal;//目标
    struct mode mode;//模式

    QString media_t;
    QString media_r;
    QMediaPlayer player;
    QMediaPlaylist playlist;

    QVector<struct kind> kind_list;//结合结构体的定义，kind_list具有类型-具体列表两层结构

public:
    pomodoro(const QString &_username = "default");//接受uesrname参数，读取"./user/username/"下相关文件进行初始化
    //TODO:账号管理系统
    //设置操作
    QDate today() const;//判断在番茄钟语境下，今日的日期
    bool set_setting_file() const;
    void init_setting();

    int work_time(){return timeset.work_time*1000*60;}//返回工作时间的毫秒值
    int short_rest_time(){return timeset.short_rest_time*1000*60;}//返回短休息时间的毫秒值
    int long_rest_time(){return timeset.long_rest_time*1000*60;}//返回长休息时间的毫秒值
    int long_rest_trigger(){return timeset.long_rest_trigger;}//返回长休息间隔
    int goal_total(){return goal.g_total;}//返回总计划数
    int goal_half(){return goal.g_half;}//返回半计划数
    int goal_zero(){return goal.g_zero;}//返回零计划数
    int goal_today(){return floor((goal.g_total-goal_half()-2*goal_zero())*0.8);}

    //番茄操作
    void init_tomato();//初始化tomato成员并创建今日番茄记录
    bool set_tomato_today() const;//将新收获的番茄写入番茄记录文件
    bool set_tomato_use() const;//设置番茄记录中的使用数
    bool set_tomato_punish() const;//设置番茄记录中的惩罚数
    bool set_tomato_goal() const;//设置番茄记录中的goal结构体

    bool is_long_rest(){return !(tomato.t_continue%timeset.long_rest_trigger);};//返回是否为长休息
    bool tomato_gain(const int &tomato_gain);

    int t_total(){return tomato.t_total;}//返回番茄总数(实际为昨日结余)
    int tomato_total(){return tomato.t_total+tomato.t_today-tomato.t_use-tomato.t_punish;}
    int t_today(){return tomato.t_today;}//返回今日番茄收获
    int t_continue(){return tomato.t_continue;}//返回持续收获番茄数
    int t_use(){return tomato.t_use;}//返回今日番茄使用
    int t_punish(){return tomato.t_punish;}//返回今日番茄惩罚

    //响铃函数
    void init_playlist() ;
    void tomato_ring() ;
    void rest_ring() ;
    void stop_ring() {player.stop();}

    void test();

    //下面是一些静态函数

    static void check_userfile(const QString &_username);//检查用户文件夹的完整性，通过异常返回结果
    static bool check_setting(const QString &_username);//检查用户设置文件完整性，若文件为空，返回false
    static bool check_tomato(const QString &_username);//检查用户番茄记录文件完整性，若文件为空，返回false
    static bool check_log(const QString &_username);//检查用户详情记录文件完整性，若文件为空，返回false
    static bool set_default_setting_file(const QString &_username);//创建默认设置文件，返回创建是否成功
    static bool repair_setting(const QString &_username);//修复setting文件
    static bool repair_tomato(const QString &_username);//修复tomato文件
    static bool repair_log(const QString &_username);//修复log文件

    static void print_tomato(const QString &_username);//打印输出tomato文件

    static int tomato_total_from_file(const QString &_username);//将昨日结余与今日操作相加减，计算出总番茄结余
//TODO:将使用uesrname读文件的函数用const QFile &file进行重载，使其可以使用文件路径直接读取
};


}

#endif // POMODORO_H
