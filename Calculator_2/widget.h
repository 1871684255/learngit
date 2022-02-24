#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QDebug>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onKeyPressed();

    QString calculate();


private:
    Ui::Widget *ui;

    QLineEdit *lineEdit;

    //屏幕显示内容
    QString tmp;

    //按钮数组
    QPushButton* buttons[16];

    double num1;
    double num2;
    QString flag;

    enum Type{
        None,Add,Sub,Mul,Div
    };
    Type m_type;

    bool m_onEqualPressed;
};
#endif // WIDGET_H
