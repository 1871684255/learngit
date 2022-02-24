#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    int row = 4;
    int col = 4;

    this->m_onEqualPressed = false;
    this->tmp = "0";
    //创建输入框
    lineEdit = new QLineEdit(this);
    //设置输入框大小可扩展
    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lineEdit->setMaximumHeight(60);
    lineEdit->setFont(QFont("Timers", 40));
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setText("0");

    //创建清空按钮
    QPushButton *btn_clear = new QPushButton("C", this);
    btn_clear->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn_clear->setFont(QFont("宋体", 20));
    connect(btn_clear, &QPushButton::clicked, this, [=]()
    {
        this->tmp = "0";
        this->lineEdit->setText(this->tmp);
    });

    //保存键盘值
    QString keys = "789/456*123-0.=+";

    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(lineEdit, 0, 1, 1, 4);
    grid->addWidget(btn_clear, 0, 0, 1, 1);

    //grid->addWidget(lineEdit);
    for(int j=0; j<row; j++)
    {
        for(int k=0; k<col; k++)
        {
            buttons[j*row+k] = new QPushButton(this);
            //设置间距
            grid->setSpacing(10);
            grid->addWidget(buttons[j*row+k], j+1, k);
            //设置文本
            buttons[j*row+k]->setText(keys.mid((j*row+k), 1));
            //设置按钮字体大小
            buttons[j*row+k]->setFont(QFont("宋体", 20));
            //设置按钮大小可扩展
            buttons[j*row+k]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            connect(buttons[j*4+k], &QPushButton::clicked, this, &Widget::onKeyPressed);
        }
    }
}


void Widget::onKeyPressed()
{
    //1.得到是谁发送的过来的信号
    //2.数字 符号
    //3.判断输入的合法性(正则表达式)
    QPushButton* button = dynamic_cast<QPushButton*>(sender());
    //button->text();
    //QString text = button->property("id").toString();

    bool isDigital = button->text().contains(QRegExp("[0-9]"));
    //判断输入的是否为数字
    if(isDigital)
    {
        int index = button->text().toInt();
        if(!this->m_onEqualPressed)
        {
            if(this->tmp == "0")
            {
                this->tmp = "";
            }
            this->tmp += QString::number(index);
            lineEdit->setText(this->tmp);
        }
        else
        {
            this->tmp = "";
            this->tmp += QString::number(index);
            lineEdit->setText(this->tmp);
            this->m_onEqualPressed = false;
        }
    }
    else
    {

        bool isFlag = lineEdit->text().contains(QRegExp("[+-*/]"));
        //如输入框中未输入数字时直接return
        bool isNumber = this->tmp.contains(QRegExp("[0-9]"));
        if(!isNumber)
        {
            return;
        }

        if(button->text() != "=")
        {
            if(button->text() == ".")
            {
                bool isPoint = this->tmp.contains(QRegExp("[.]"));
                if(!isPoint)
                {
                    this->tmp += ".";

                }
                lineEdit->setText(this->tmp);
                return;
            }
            else if(button->text() == "+")
            {
                if(!isFlag)
                {
                    this->num1 = this->tmp.toDouble();
                    this->tmp += " + ";
                    m_type = Add;
                }
            }
            else if(button->text() == "-")
            {
                if(!isFlag)
                {
                    this->num1 = this->tmp.toDouble();
                    this->tmp += " - ";
                    m_type = Sub;
                }
            }
            else if(button->text() == "*")
            {
                if(!isFlag)
                {
                    this->num1 = this->tmp.toDouble();
                    this->tmp += " * ";
                    m_type = Mul;
                }
            }
            else if(button->text() == "/")
            {
                if(!isFlag)
                {
                    this->num1 = this->tmp.toDouble();
                    this->tmp += " / ";
                    m_type = Div;
                }
            }
            lineEdit->setText(this->tmp);
            this->tmp = "";
        }
        else
        {
            this->m_onEqualPressed = true;
            this->num2 = this->tmp.toDouble();
            this->tmp = this->calculate();
            lineEdit->setText(this->tmp);
            m_type = None;
            //this->tmp = "";
        }

    }

}

QString Widget::calculate()
{
    double result = 0.0;
    switch(m_type){
    case Add:
        result = this->num1 + this->num2;
        break;
    case Sub:
        result = this->num1 - this->num2;
        break;
    case Mul:
        result = this->num1 * this->num2;
        break;
    case Div:{
        //被除数不能为零
        if(this->num2 == 0)
        {
            return "-1";
        }
        result = this->num1 / this->num2;
    }
        break;
    default:
        return QString::number(this->num2);
        break;
    }


    return QString::number(result);
}

Widget::~Widget()
{
    delete ui;
}

