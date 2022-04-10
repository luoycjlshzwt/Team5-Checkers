#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include "checkerbutton.h"
#include "mydialog.h"
#include"mydialog1.h"
#include <QMouseEvent>
#include <QLabel>
#include<QPropertyAnimation>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public Q_SLOTS:
    void cbuttonpress();
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void mousePressEvent(QMouseEvent* ev);
    bool islegal();
    void int2pixel();   //将loc坐标转换为像素坐标
    int pixel2int(QPointF& pixel);
    void CheckerMove(CheckerButton*btn,QPointF p);

protected:
    void paintEvent(QPaintEvent *);
     void Mpe(QMouseEvent *event);

private:
    Ui::Widget *ui;
    void DrawCheckerboard();
    void InitCheckerboard();
    QPointF loc[17][17];//棋盘每个位置的坐标
    bool isfill[17][17];//棋盘每个位置是否有棋子
//    CheckerButton *rbtn[10],*bbtn[10];//红蓝双方棋子
    CheckerButton* btn[6][10]; //六方棋子
    int playernum; //棋手个数
    int flag;  //判断下棋方
    QPointF chosen;//选中棋子位置
    int chosenloc[2];//选中棋子所在位置
    CheckerButton* chosenbtn;//选中棋子
    bool ischosen=false;//是否有棋子被选中
    QPointF obj;//目标位置？
    int objloc[2];//目标位置坐标？
    bool jumpmove;
    QLabel* test;
    QPushButton* end;
    mydialog1 *z;
};
#endif // WIDGET_H
