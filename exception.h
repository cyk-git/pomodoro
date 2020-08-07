#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QException>
#include <QMessageBox>

//异常管理：
//定义tmt::exception类作为所有错误的父类，内置show_message_box方法显示错误提示框
//定义filePath和path两个派生类异常对应文件不存在和路径不存在错误
//定义fileOpen异常对应文件无法打开错误
//定义fileEmpty异常对应文件空错误
//TODO:定义fileDamaged异常对应文件损坏错误
//TODO:相应check函数的编写，每种check函数使用异常抛出filePath,fileOpen,fileDamaged错误，使用返回值false返回文件空异常（文件空在某些情况下是异常，某些情况下是首次使用导致的结果）
//TODO:将三个文件对应的各类文件异常分开

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

class fileOpen_exception : public exception{
private:
    QString filepath;
public:
    fileOpen_exception(const QString &errorname="FileOpen Exception!",const QString &_filepath="");
    void raise() const { throw *this; }
    exception *clone() const { return new exception(*this); }
    QString filename() const { return filepath; }
};

class filePath_exception : public exception{
private:
    QString filepath;
public:
    filePath_exception(const QString &errorname="Filepath Exception!",const QString &_filepath="");
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

class fileEmpty_exception : public exception{
private:
    QString filepath;
public:
    fileEmpty_exception(const QString &errorname="FileEmpty Exception!",const QString &_filepath="");
    void raise() const { throw *this; }
    exception *clone() const { return new exception(*this); }
    QString filename() const { return filepath; }
};

}
#endif // EXCEPTION_H
