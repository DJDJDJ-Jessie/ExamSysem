#ifndef EXAMDIALOG_H
#define EXAMDIALOG_H
#include <QDialog>
#include <QTimer>
#include <QTextEdit>        //文件编辑控件
#include <QLabel>           //标签控件
#include <QRadioButton>     //单选控件
#include <QCheckBox>        //多项控件
#include <QGridLayout>      //布局管理器
#include <QButtonGroup>

class ExamDialog : public QDialog
{
    Q_OBJECT
public:
    ExamDialog(QWidget* parent = 0);
    void initTimer();   //初始化计时器
    bool initTextEdit();//初始化文本编辑器
    void initLayout();  //初始化布局管理器
    void initButtons(); //初始化按钮布局

    bool hasNoSelect(); //判断题目是否有未完成的
private:
    QTimer *m_timer;    //计时器
    int m_timeGo;       //考试用时

    QTextEdit* m_textEdit;          //考试题显示
    QLabel* m_titleLabels[10];      //题目标签
    QRadioButton* m_radioBtns[32];   //单选题按钮
    QCheckBox* m_checkBtns[4];      //多选题按钮
    QRadioButton* m_radioA;         //判断题A选项
    QRadioButton* m_radioB;         //判断题B选项
    QGridLayout* m_layouts;         //布局管理器
    QStringList m_answer;           //答案链表

    QButtonGroup* m_btnGroup[9];    //单选按钮分组
private slots:
    void freshTime();   //槽方法：刷新时间
    void getScore();    //获取考试成绩
};

#endif // EXAMDIALOG_H
