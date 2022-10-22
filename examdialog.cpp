#include "examdialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>

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

    initButtons();
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
                m_answer.append(strList.at(1)); //保存答案的选项
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

void ExamDialog::initButtons()
{
    QStringList strList = {"A","B","C","D"};    //选择题的选项
    for(int i=0;i<10;i++)
    {
        //题目标签
        m_titleLabels[i] = new QLabel(this);
        m_titleLabels[i]->setText("第" + QString::number(i+1) + "题");
        m_layouts->addWidget(m_titleLabels[i],1,i);//将按钮加入到布局管理器中，从第1行i列开始默认大小是占据1行1列

        //判断题的处理
        if(i==9)
        {
            m_radioA = new QRadioButton(this);
            m_radioB = new QRadioButton(this);

            m_radioA->setText("正确");
            m_radioB->setText("错误");

            m_layouts->addWidget(m_radioA,2,9);
            m_layouts->addWidget(m_radioB,3,9);

            //给判断题进行分组
            m_btnGroup[8] = new QButtonGroup(this);
            m_btnGroup[8]->addButton(m_radioA);
            m_btnGroup[8]->addButton(m_radioB);
            break;
        }

        if(i<8) m_btnGroup[i] = new QButtonGroup(this);
        //选择题的处理
        for(int j=0;j<4;j++)
        {
            if(i==8)//多选题
            {
                m_checkBtns[j] = new QCheckBox(this);
                m_checkBtns[j]->setText(strList.at(j));
                m_layouts->addWidget(m_checkBtns[j],2+j,8);
            }
            else    //单选题
            {
                m_radioBtns[4*i+j] = new QRadioButton(this);
                m_radioBtns[4*i+j]->setText(strList.at(j));
                m_layouts->addWidget(m_radioBtns[4*i+j],2+j,i);
                m_btnGroup[i]->addButton(m_radioBtns[4*i+j]);
            }
        }
        //提交按钮的处理
        QPushButton *combtn = new QPushButton(this);
        combtn->setText("提交");
        combtn->setFixedSize(100,35);   //给按钮设置大小
        //用连接方法实现信号与槽机制
        connect(combtn,SIGNAL(clicked(bool)),this,SLOT(getScore()));//点击这个按钮当前窗口响应,用槽方法响应
        m_layouts->addWidget(combtn,6,9);

    }
}


void ExamDialog::getScore()
{
    if(hasNoSelect())
    {
        QMessageBox::information(this,"提示","您有未完成的题目，请完成考试");
        return;
    }

    int scores = 0;
    //循环十道题和答案链表中的答案进行一个比对
    for(int i=0;i<10;i++)
    {
        if(i<8)
        {
            if(m_btnGroup[i]->checkedButton()->text() == m_answer.at(i))
                scores +=10;
        }
        if(i == 8)
        {
            QString answer = m_answer.at(i);    //提取多项选择题答案
            bool hasA = false;
            bool hasB = false;
            bool hasC = false;
            bool hasD = false;

            if(answer.contains("A"))    hasA = true;
            if(answer.contains("B"))    hasB = true;
            if(answer.contains("C"))    hasC = true;
            if(answer.contains("D"))    hasD = true;

            bool checkA = m_checkBtns[i]->checkState(); //得到选中的状态，选中就是true
            bool checkB = m_checkBtns[i]->checkState();
            bool checkC = m_checkBtns[i]->checkState();
            bool checkD = m_checkBtns[i]->checkState();

            if(hasA!=checkA || hasB!=checkB || hasC!=checkC || hasD!=checkD)    continue;
            scores+=10;
        }
        //判断题计分
        if(i==9)
        {
            if(m_btnGroup[8]->checkedButton()->text() == m_answer.at(i))
                scores+=10;
        }
    }

    QString str = "您的分数是" + QString::number(scores) + "分,是否重新考试";
    int ret = QMessageBox::information(this,"提示",str,QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        return;
    }
    else
    {
        close();
    }
}

void ExamDialog::freshTime()
{
    //刷新考试用时
    m_timeGo++;
    QString min = QString::number(m_timeGo/60); //分钟
    QString sec = QString::number(m_timeGo%60); //秒钟
    setWindowTitle("考试已用时："+ min + "分" + sec + "秒");
}

bool ExamDialog::hasNoSelect()
{
    //单选按钮计数，8个分组
    int radioSelects = 0;
    for(int i=0;i<8;i++)
    {
        if(m_btnGroup[i]->checkedButton())
            radioSelects++;
    }

    if(radioSelects != 8)
    {
        return true;
    }

    //多选计数
    int checkSelects = 0;
    for(int i=0;i<4;i++)
    {
        if(m_checkBtns[i]->isChecked())
            checkSelects++;
    }
    if(checkSelects < 1)
    {
        return true;
    }

    //判断题计数
    if(!m_radioA->isChecked() && !m_radioB->isChecked())
    {
        return true;
    }

    return false;
}
