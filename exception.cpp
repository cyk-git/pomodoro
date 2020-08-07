#include "exception.h"

namespace tmt
{

exception::exception(const QString &errorname):error(errorname){}

int exception::show_massage_box(QMessageBox::Icon icon,const QString &title) const{
    int choose;
    QMessageBox msg;
    msg.setWindowTitle(title);
    msg.setText(error);
    msg.setIcon(icon);
    msg.addButton("确定",QMessageBox::AcceptRole);
    choose=msg.exec();
    return choose;
}

int exception::show_massage_box(const QString &errorname,QMessageBox::Icon icon,const QString &title) const{
    int choose;
    QMessageBox msg;
    msg.setWindowTitle(title);
    msg.setText(errorname);
    msg.setIcon(icon);
    msg.addButton("确定",QMessageBox::AcceptRole);
    choose=msg.exec();
    return choose;
}

QString exception::errorname() const{
    return error;
}

fileOpen_exception::fileOpen_exception(const QString &errorname,const QString &_filepath):exception(errorname),filepath(_filepath){}

filePath_exception::filePath_exception(const QString &errorname,const QString &_filepath):exception(errorname),filepath(_filepath){}

fileEmpty_exception::fileEmpty_exception(const QString &errorname,const QString &_filepath):exception(errorname),filepath(_filepath){}

path_exception::path_exception(const QString &errorname,const QString &_path):exception(errorname),path(_path){}

}
