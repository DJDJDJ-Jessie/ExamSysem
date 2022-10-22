#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  //应用程序类，建立了一个对象a
    LoginDialog w;  //建立登录窗口的对象
    w.show();  //调用show方法

    return a.exec();  //应用程序的exec方法，使对象进入消息循环
}
