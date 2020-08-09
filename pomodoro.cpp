#include "pomodoro.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include "exception.h"


namespace tmt{

//下面对这些结构体的流式传输进行重载
    QDataStream &operator<<(QDataStream &QDS,const struct tomato &tomato){//输出到文件时不输出total
        QDS << tomato.t_today
            << tomato.t_use
            << tomato.t_punish ;
        return QDS;
    }

    QDataStream &operator>>(QDataStream &QDS,struct tomato &tomato){//输入到程序时输入total
        QDS >> tomato.last_t_total
            >> tomato.t_today
            >> tomato.t_use
            >> tomato.t_punish ;
        return QDS;
    }

    QDataStream &operator<<(QDataStream &QDS,const struct timeset &tset){
        QDS << tset.start_time << tset.work_time << tset.short_rest_time << tset.long_rest_time << tset.long_rest_trigger;
        return QDS;
    }

    QDataStream &operator>>(QDataStream &QDS,struct timeset &tset){
        QDS >> tset.start_time >> tset.work_time >> tset.short_rest_time >> tset.long_rest_time >> tset.long_rest_trigger;
        return QDS;
    }

    QDataStream &operator<<(QDataStream &QDS,const struct goal &goal){
        QDS << goal.g_total << goal.g_half << goal.g_zero;
        return QDS;
    }

    QDataStream &operator>>(QDataStream &QDS,struct goal &goal){
        QDS >> goal.g_total >> goal.g_half >> goal.g_zero;
        return QDS;
    }

    QDataStream &operator<<(QDataStream &QDS,const struct mode &mode){
        QDS << mode.exercise_mode << mode.simple_mode;
        return QDS;
    }

    QDataStream &operator>>(QDataStream &QDS,struct mode &mode){
        QDS >> mode.exercise_mode >> mode.simple_mode;
        return QDS;
    }

//TODO:重载关于详细信息输出和使用惩罚预设的>>运算符

pomodoro::pomodoro(const QString &_username){//将各种读取操作使用成员函数实现，构造函数中主要处理异常
    username = _username;
    //QString filepath = QString("./users/%1/").arg(_username);
    //QDir filedir(filepath);//个人文件夹路径
    bool check=true;//个人文件夹检查
    do{//用户文件检查
        try {
            check_userfile(_username);
            init_setting();
            init_tomato();
            init_playlist();
            check=false;
        } catch (path_exception &path_exc) {
            path_exc.show_massage_box(path_exc.errorname()+"缺失，将创建新的"+path_exc.errorname());
            QDir filedir(path_exc.pathname());
            if(!filedir.mkpath("./")){//创建路径
                exception("无法创建个人文件夹，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
                abort();
            }
            if(!set_default_setting_file(username)){//创建个人配置文件
                exception("无法创建个人配置文件，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
                abort();
            }
            QFile newfile(tomato_filepath());//创建番茄记录文件
            if(!newfile.open(QFile::WriteOnly)){
                exception("无法创建番茄记录文件，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
                abort();
            }
            newfile.close();
            newfile.setFileName(log_filepath());//创建详情记录文件
            if(!newfile.open(QFile::WriteOnly)){
                exception("无法创建详情日志文件，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
                abort();
            }
            newfile.close();
    //TODO:完善新建个人配置文件函数
        }catch (filePath_exception &file_exc){
            file_exc.show_massage_box(file_exc.errorname()+"缺失，将创建新的"+file_exc.errorname());
            QFile newfile(file_exc.filepathname());
            if(newfile.fileName()==(setting_filepath())){//对个人配置文件特殊处理
                if(!set_default_setting_file(username)){
                    exception("无法创建个人配置文件，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
                    abort();
                }
            }else{
                if(!newfile.open(QFile::WriteOnly)){
                    exception("无法创建个人文件，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
                    abort();
                }
                newfile.close();
            }
        }
        catch(fileOpen_exception &file_exc){
            file_exc.show_massage_box(file_exc.errorname()+"无法打开，程序终止！",QMessageBox::Critical,"错误");
            abort();
        }
        catch(...){
            exception("未知错误，程序终止！").show_massage_box(QMessageBox::Critical,"错误");
            abort();
        }
    }while(check);

    //TODO:异常处理
//    QFile tomato_f(filepath+"tomato.tmt");
//    tomato_f.open(QFile::ReadOnly);
//    data.setDevice(&tomato_f);
//TODO:番茄相关文件的读取,先设计番茄钟核心组件再解决这个问题
//    tomato.t_today=tomato.last_t_total=tomato.t_continue=0;
//    tomato_f.close();
}

QDate pomodoro::today() const{
    QTime now_t;
    QDate now_d;
    now_d = now_d.currentDate();
    now_t = now_t.currentTime();
    if (now_t>=timeset.start_time)
        return now_d;
    else
        return now_d.addDays(-1);
}

void pomodoro::check_userfile(const QString &_username){
    QString filepath = pomodoro::user_filepath(_username);
    QDir filedir(filepath);
    //检查个人文件夹是否存在，否则为初次创建该账户
    if(!filedir.exists()){
//TODO：无个人文件夹报错，更改登录账户或创建新账户
//TODO：没有user文件夹为首次使用软件，引导创建新账户
        throw path_exception("个人文件夹",filepath);
    }
    //个人文件夹检查完毕
    //检查个人配置文件、番茄记录文件、详情记录文件完整性
    QFile setting_f(filepath+"setting.cfg"),
          tomato_f(filepath+"tomato.tmt"),
          log_f(filepath+"log.txt");
    if (!setting_f.exists()){
        throw filePath_exception("个人配置文件",filepath+"setting.cfg");
    }
    if(!tomato_f.exists()){
        throw filePath_exception("番茄记录",filepath+"tomato.tmt");
    }
    if(!log_f.exists()){
        throw filePath_exception("详情记录文件",filepath+"log.txt");
    }
    check_setting(_username);
    check_tomato(_username);
    check_log(_username);
}


//TODO:检查文件完整性函数，使用异常返回结果
bool pomodoro::check_setting(const QString &_username){}
bool pomodoro::check_tomato(const QString &_username){
    QFile tomato_f(pomodoro::tomato_filepath(_username));
    if(!tomato_f.exists()){
        throw filePath_exception("番茄记录文件不存在！?",tomato_f.fileName());
    }//使用file异常排除文件不存在错误
    if(!tomato_f.open(QIODevice::ReadOnly)){
        throw fileOpen_exception("番茄记录文件无法打开！",tomato_f.fileName());
    }
    tomato_f.close();
    if(!tomato_f.size()){
        return false;
    }else{
        //TODO:检查操作
        return true;
    }

}
bool pomodoro::check_log(const QString &_username){}

bool pomodoro::set_default_setting_file(const QString &_username){
    QFile setting_f(pomodoro::setting_filepath(_username));
    if(!setting_f.open(QFile::WriteOnly)){
        return false;
    }
    QDataStream set(&setting_f);
    struct timeset timeset{QTime(7,0),25,5,15,4};//设置
    struct goal goal{20,0,0};//目标
    struct mode mode{false,false};//模式
    QString media_t("./Media/Alarm.wav"),media_r("./Media/Alarm.wav");
//TODO:使用与惩罚预设
    set << timeset << goal << mode << media_t << media_r;
    setting_f.close();
    return true;
}

void pomodoro::test(){
    qDebug() << tomato.t_today << "\n" << tomato.last_t_total <<"\n"<<tomato.t_continue<<"\n";
    qDebug() << timeset.work_time << "\n" << timeset.start_time <<"\n"<<timeset.short_rest_time<<"\n"<<timeset.long_rest_time<<"\n"<<timeset.long_rest_trigger<<"\n";
    qDebug() << goal.g_total << "\n" << goal.g_half <<"\n"<<goal.g_zero<<"\n";
    qDebug() << mode.simple_mode << "\n" << mode.exercise_mode <<"\n";
}

void pomodoro::init_setting(){
    QFile setting_f("./users/"+username+"/setting.cfg");
    if(!setting_f.exists()){
        throw filePath_exception("设置文件不存在！",setting_f.fileName());
    }//使用file异常排除文件不存在错误
    check_setting(username);
    if(!setting_f.open(QFile::ReadOnly)){
        throw fileOpen_exception("设置文件无法打开！",setting_f.fileName());
    }
    QDataStream data(&setting_f);
    data >> timeset >> goal >> mode >> media_t >> media_r ;
//TODO:使用与惩罚预设的导入
    setting_f.close();
}

void pomodoro::init_tomato(){
    QFile tomato_f(tomato_filepath());
    if(!tomato_f.exists()){
        throw filePath_exception("番茄记录文件不存在！",tomato_f.fileName());
    }//使用file异常排除文件不存在错误
    if(check_tomato(username)){
        if(!tomato_f.open(QIODevice::ReadWrite)){
            throw fileOpen_exception("番茄记录文件无法打开！",tomato_f.fileName());
        }
        tomato_f.seek(tomato_f.size()-(qint64)sizeof(int)*(3+3+1)-(qint64)sizeof(QDate));
        QDataStream in(&tomato_f);
        QDate day;
        in >> day ;
        if(day==today()){//当日有记录的情况
            in >> tomato >> goal;
            tomato_f.close();
            tomato.t_continue=0;
            return;
        }else{
            tomato_f.seek(tomato_f.size());
        }
    }else{
        if(!tomato_f.open(QIODevice::ReadWrite)){
            throw fileOpen_exception("番茄记录文件无法打开！",tomato_f.fileName());
        }
    }
//空文件或无今日内容文件初始化
    tomato={tomato_total_from_file(username),0,0,0,0};
    QDataStream out(&tomato_f);
    out << today()
        << tomato.last_t_total
        << tomato
        << goal;
    tomato_f.close();
}

int pomodoro::tomato_total_from_file(const QString &_username){
    QFile tomato_f(pomodoro::tomato_filepath(_username));
    if(!tomato_f.exists()){
        throw filePath_exception("番茄记录文件不存在！",tomato_f.fileName());
    }//使用file异常排除文件不存在错误
    if(!check_tomato(_username)){
        return 0;
    }else{
        if(!tomato_f.open(QIODevice::ReadOnly)){
            throw fileOpen_exception("番茄记录文件无法打开！",tomato_f.fileName());
        }
        tomato_f.seek(tomato_f.size()-(qint64)sizeof(int)*(3+3+1));//定位到最后一天
        QDataStream in(&tomato_f);
        int total,today,use,punish;
        in >> total >> today >> use >> punish;
        tomato_f.close();
        return (total+today-use-punish);
    }
}

void pomodoro::print_tomato_file(const QString &_username){
    QFile tomato_f(pomodoro::tomato_filepath(_username));
    if(!tomato_f.exists()){
        throw filePath_exception("番茄记录文件不存在！",tomato_f.fileName());
    }//使用file异常排除文件不存在错误
    if(!tomato_f.open(QIODevice::ReadOnly)){
        throw fileOpen_exception("番茄记录文件无法打开！",tomato_f.fileName());
    }
    QFile print_f("./users/"+_username+"/tomato_print.txt");
    if(!print_f.open(QIODevice::WriteOnly)){
        tomato_f.close();
        throw fileOpen_exception("番茄记录文件无法打开！",tomato_f.fileName());
    }
    QDataStream in(&tomato_f);
    QTextStream out(&print_f);
    out << QString("番茄记录日期\t昨日总计\t今日收获\t使用\t惩罚\t总目标\t减半时长\t排除时长\n");
    for(;!in.atEnd();){
        QDate day;
        struct tomato tomato;
        struct goal goal;
        in >> day >> tomato >> goal;
/*        out << "Date:" << day.toString("yyyy.MM.dd") << "\n"
            << "tomato.last_t_total:" << tomato.last_t_total << "\n"
            << "tomato.t_today:" << tomato.t_today << "\n"
            << "tomato.t_use:" << tomato.t_use << "\n"
            << "tomato.t_punish:" << tomato.t_punish << "\n"
            << "goal.g_total:" << goal.g_total << "\n"
            << "goal.g_half:" << goal.g_half << "\n"
            << "goal.g_zero:" << goal.g_zero << "\n\n";
*/

        out << day.toString("yyyy.MM.dd") << "\t"
            << tomato.last_t_total << "\t"
            << tomato.t_today << "\t"
            << tomato.t_use << "\t"
            << tomato.t_punish << "\t"
            << goal.g_total << "\t"
            << goal.g_half << "\t"
            << goal.g_zero << "\n";
    }
    print_f.close();
    tomato_f.close();
}

void pomodoro::print_tomato_file() const{
    pomodoro::print_tomato_file(username);
}

void pomodoro::init_playlist(){
    if(!QFile::exists(media_t)){
        throw filePath_exception("番茄响铃媒体文件不存在！",media_t);
    }
    if(!QFile::exists(media_r)){
        throw filePath_exception("休息响铃媒体文件不存在！",media_r);
    }
    playlist.addMedia(QUrl::fromLocalFile(media_r));
    playlist.addMedia(QUrl::fromLocalFile(media_t));
    playlist.addMedia(QUrl::fromLocalFile(media_t));
    playlist.addMedia(QUrl::fromLocalFile(media_t));
}

void pomodoro::tomato_ring() {
    QFile file(media_t);
    if(!file.exists()){
        throw filePath_exception("媒体文件不存在！",media_t);
    }
    playlist.setCurrentIndex(2);
    playlist.setPlaybackMode(QMediaPlaylist::Sequential);
    player.setPlaylist(&playlist);
//    player.setMedia(QUrl::fromLocalFile(media_t));
    if(!player.isAvailable()){
        throw fileOpen_exception("媒体文件无法打开！",media_t);
    }
    player.play();
}

void pomodoro::rest_ring() {
    if(!QFile::exists(media_t)){
        throw filePath_exception("媒体文件不存在！",media_r);
    }
    playlist.setCurrentIndex(1);
    playlist.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player.setPlaylist(&playlist);
    if(!player.isAvailable()){
        throw fileOpen_exception("媒体文件无法打开！",media_r);
    }
    player.play();
}

void pomodoro::set_today_file() const{
    QFile tomato_f(tomato_filepath());
    if(!check_tomato(username)){
        throw fileEmpty_exception("番茄文件为空文件！",tomato_filepath());
    }
    tomato_f.open(QIODevice::ReadWrite);
    tomato_f.seek(tomato_f.size()-(qint64)sizeof(int)*(3+3));//定位t_today的位置
    QDataStream out(&tomato_f);
    out << tomato.t_today;
}

void pomodoro::set_use_file() const{
    QFile tomato_f(tomato_filepath());
    if(!check_tomato(username)){
        throw fileEmpty_exception("番茄文件为空文件！",tomato_filepath());
    }
    tomato_f.open(QIODevice::WriteOnly);
    tomato_f.seek(tomato_f.size()-(qint64)sizeof(int)*(3+2));//定位t_use的位置
    QDataStream out(&tomato_f);
    out << tomato.t_use;
}

void pomodoro::set_punish_file() const{
    QFile tomato_f(tomato_filepath());
    if(!check_tomato(username)){
        throw fileEmpty_exception("番茄文件为空文件！",tomato_filepath());
    }
    tomato_f.open(QIODevice::WriteOnly);
    tomato_f.seek(tomato_f.size()-(qint64)sizeof(int)*(3+1));//定位t_punish的位置
    QDataStream out(&tomato_f);
    out << tomato.t_punish;
}

void pomodoro::tomato_gain(const int &tomato_gain){
    tomato.t_today += tomato_gain;
    tomato.t_continue += tomato_gain;
    set_today_file();//这里会抛出文件异常
}

}
