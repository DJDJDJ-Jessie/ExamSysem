#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QString>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)   //ui在构造函数中进行初始化 LoginDialog登录窗口界面信息的对象，所以所有界面信息都可以ui指针进行访问
{
    ui->setupUi(this);   //方法：初始化界面
    ui->imageLabel->setScaledContents(true);
    this->resize(ui->imageLabel->width(),ui->imageLabel->height());
    setFixedSize(width(),height());
    this->setWindowTitle("科目一考试登录");
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;  //析构：进行自然回收ui指针
}

void LoginDialog::on_LoginButton_clicked()
{
    //正则验证邮箱地址 用户名@域名 比如 66688800@qq.com  或666889992@cn
    //元字符解释：^表示规则字符串的开始 $表示规则字符串的结束
    //+表示匹配次数≥1次  *表示匹配次数为任意次数（可为0次） {n,m}表示匹配次数至少n次，至多m次
    //编写正则表达式：正则表达式的初始化
    //用户名可以是[大小写字母和数字]至少出现一次,加上[指定符号和大小写字母]可以出现任意次数 @ 域名可以是[..]至少出现一次加上.加上[..]长度在2到6之间
    QRegExp rx("^[A-Za-z0-9]+([_\.][A-Za-z0-9]+)*@([A-Za-z0-9\-]+\.)+[A-Za-z]{2,6}$");   //注意不要随意加空格
    bool ret = rx.exactMatch(ui->UserName->text());    //准确的匹配
    if(!ret)
    {
        QMessageBox::information(this,"提示","非法邮箱地址，请重新输入");
        ui->UserName->clear();
        ui->Password->clear();
        ui->UserName->setFocus();
        return;     //直接返回，不需要进行后续操作
    }
    else
    {
        QString filename;   //要读取文件的路径
        QString strAccount; //用户输入的账号
        QString strPassword;//用户输入的密码
        QString strLine;    //保存每一行读取的字符串
        QStringList strList; //字符串链表

        //初始化
        filename = "../account.txt";    //相对路径是基于debug目录来的
        strAccount = ui->UserName->text();//保存用户账号输入
        strPassword = ui->Password->text(); //保存用户密码输入

        //对文件进行操作
        QFile file(filename);        //QFile要加头文件
        QTextStream stream(&file);   //文本流，要加头文件
        bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);   //用只读方式打开文件，该文件是一个文本文件
        if(!ret)
        {
            QMessageBox::information(this,"提示","读取账号数据文件失败");
            return;
        }
        else
        {
            //循环读直到读完文件
            while(!stream.atEnd())
            {
                //读取文本内容
                strLine = stream.readLine();    //读取一行
                strList = strLine.split(",");             //QString方法有split方法将整行字符串以逗号进行分割，返回多个字符串
                if(strAccount == strList.at(0))         //账号匹配成功
                {
                    if(strPassword == strList.at(1))      //密码匹配成功
                    {
                        QMessageBox::information(this,"提示","欢迎进入科目一考试系统");
                        file.close();
                        return;
                    }
                    else
                    {
                        QMessageBox::information(this,"提示","密码错误，请重新输入");
                        ui->Password->clear();
                        ui->Password->setFocus();
                        file.close();
                        return;
                    }
                }
            }
            //循环完毕，整个数据文件都没有匹配的
            QMessageBox::information(this,"提示","用户账户不存在，请重新输入");
            ui->UserName->clear();
            ui->Password->clear();
            ui->UserName->setFocus();
            file.close();
            return;
        }

    }
}

void LoginDialog::on_registerButton_clicked()
{
    QRegExp rx("^[A-Za-z0-9]+([_\.][A-Za-z0-9]+)*@([A-Za-z0-9\-]+\.)+[A-Za-z]{2,6}$");   //注意不要随意加空格
    bool ret = rx.exactMatch(ui->UserName->text());    //准确的匹配
    if(!ret)
    {
        QMessageBox::information(this,"提示","非法邮箱地址，请重新输入");
        ui->UserName->clear();
        ui->Password->clear();
        ui->UserName->setFocus();
        return;     //直接返回，不需要进行后续操作
    }
    else
    {
        QString filename;   //要读取文件的路径
        QString strAccount; //用户输入的账号
        QString strPassword;//用户输入的密码
        QString strLine;    //保存每一行读取的字符串

        //初始化
        filename = "../account.txt";    //相对路径是基于debug目录来的
        strAccount = ui->UserName->text();//保存用户账号输入
        strPassword = ui->Password->text(); //保存用户密码输入

        //对文件进行操作
        QFile file(filename);        //QFile要加头文件
        QTextStream out(&file);   //文本流，要加头文件
        bool ret = file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);   //用只写方式打开文件，该文件是一个文本文件，追加到文件末尾
        if(!ret)
        {
            QMessageBox::information(this,"提示","读取账号数据文件失败");
            file.close();
            return;
        }
        else
        {
           out << strAccount << "," << strPassword << "\n";
           QMessageBox::information(this,"提示","注册成功");
           ui->UserName->clear();
           ui->Password->clear();
           ui->UserName->setFocus();
           file.close();
           return;
        }
    }
}
