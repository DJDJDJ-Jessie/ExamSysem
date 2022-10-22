#include "logindialog.h"
#include <QApplication>
#include "examdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  //应用程序类，建立了一个对象a
    LoginDialog login;  //建立登录窗口的对象
    int ret = login.exec();  //让登录窗口以模态的方式运行，即能获取当前窗体用户选择的状态
    if(ret == QDialog::Accepted)
    {
        ExamDialog* exam;
        exam = new ExamDialog;
    }
    else
    {
        return 0;
    }
    return a.exec();  //应用程序的exec方法，使对象进入消息循环
}
