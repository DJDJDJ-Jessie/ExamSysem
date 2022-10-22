#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

//声明一个作用域（命名空间）
namespace Ui {
class LoginDialog; //子类，描述窗口界面的一个类
}

class LoginDialog : public QDialog  //自定义类
{
    Q_OBJECT  //宏，当前类支持信号与槽的机制，不是所有的类都有这个宏即支持此机制，只有继承自QObject的类才可以使用该机制（如果自定义的想使用则要手动加上）
//信号与槽：不同对象（类、窗口）进行通信

public:    //构造与析构
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_LoginButton_clicked();

    void on_registerButton_clicked();

private:   //私有数据
    Ui::LoginDialog *ui;  //ui指针指向命名空间Ui中的LoginDialog对象
};

#endif // LOGINDIALOG_H
