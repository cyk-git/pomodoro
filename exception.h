#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QException>
#include <QMessageBox>

//异常管理：
//定义tmt::exception类作为所有错误的父类，内置show_message_box方法显示错误提示框
//定义filepath和path两个派生类异常对应文件不存在和路径不存在错误
//定义file异常对应文件无法打开错误
//TODO:定义setting,tomato,log三个派生类异常对应文件格式异常
//TODO:将三个file，filepath异常分开

namespace tmt
{

class exception : public QException
{
private:
    QString error;

public:
    exception(const QString &errorname="Unkonwn Error!");
    void raise() const { throw *this; }
    exception *clone() const { return new exception(*this); }
    virtual int show_massage_box(QMessageBox::Icon icon=QMessageBox::Warning,const QString &title="警告") const;
    virtual int show_massage_box(const QString &errorname,QMessageBox::Icon icon=QMessageBox::Warning,const QString &title="警告") const;
    virtual QString errorname() const;
};

class file_exception : public exception{
private:
    QString filepath;
public:
    file_exception(const QString &errorname="File Exception!",const QString &_filepath="");
    void raise() const { throw *this; }
    exception *clone() const { return new exception(*this); }
    QString filename() const { return filepath; }
};

class filepath_exception : public exception{
private:
    QString filepath;
public:
    filepath_exception(const QString &errorname="Filepath Exception!",const QString &_filepath="");
    void raise() const { throw *this; }
    exception *clone() const { return new exception(*this); }
    QString filepathname() const { return filepath; }
};

class path_exception : public exception{
private:
    QString path;
public:
    path_exception(const QString &errorname="Path Exception!",const QString &_path="");
    void raise() const { throw *this; }
    exception *clone() const { return new exception(*this); }
    QString pathname() const { return path; }
};


}
#endif // EXCEPTION_H
