#include "examdialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

ExamDialog::ExamDialog(QWidget* parent):QDialog(parent)
{
    setWindowTitle("考试已用时：0分0秒");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint); //设置窗体风格
    QFont font;
    font.setPointSize(12);//设置字大小
    setFont(font);

    setPalette(QPalette(QColor(209,215,255)));  //设置窗体背景颜色

    resize(800,900);    //设置窗体大小
    initTimer();
    initLayout();
    if(!initTextEdit())
    {
        QMessageBox::information(this,"提示","初始化题库数据文件失败");
        QTimer::singleShot(0,qApp,SLOT(quit()));    //立即退出，qApp是应用程序的全局对象
    }
}

void ExamDialog::initTimer()
{
    m_timeGo = 0;
    m_timer = new QTimer(this);
    m_timer->setInterval(1000); //设置计时器每隔1000毫秒就发送一次信号
    m_timer->start();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(freshTime()));//将信号与槽联系起来，计时器每次1秒钟就要刷新窗口

}

bool ExamDialog::initTextEdit()
{
    QString strLine;        //保存文件中读取到的一行数据
    QStringList strList;    //保存读取到的答案行
    QString fileName("../exam.txt");
    QFile file(fileName);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(ret)
    {
        //打开成功就要进行读取
        m_textEdit = new QTextEdit(this);
        m_textEdit->setReadOnly(true);  //将文本编辑器控件设置为只读
        QString strText;        //将读取到的每一行都放在strText中
        int nLines = 0;
        QString strLine;
        QStringList strList;
        while(!stream.atEnd())
        {
            //过滤首行
            if(nLines == 0)
            {
                stream.readLine();
                nLines++;
                continue;
            }
            //过滤选择题的答案行和判断题的答案行
            if( (nLines>=6 && nLines <= 6*9 && (nLines%6==0)) || (nLines == 6*9+4))
            {
                strLine = stream.readLine();
                strList = strLine.split(" ");
                m_answer.append(strList.at(1));
                strText += "\n";
                nLines++;
                continue;
            }
            strText += stream.readLine();
            strText += "\n";
            nLines++;
        }
        m_textEdit->setText(strText);   //设置文本显示
        m_layouts->addWidget(m_textEdit,0,0,1,10);
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void ExamDialog::initLayout()
{
    m_layouts = new QGridLayout(this);
    m_layouts->setSpacing(10);    //设置控件与控件间的距离
    m_layouts->setMargin(10);     //设置窗体与控件间的间隙
}

void ExamDialog::freshTime()
{
    //刷新考试用时
    m_timeGo++;
    QString min = QString::number(m_timeGo/60); //分钟
    QString sec = QString::number(m_timeGo%60); //秒钟
    setWindowTitle("考试已用时："+ min + "分" + sec + "秒");
}
