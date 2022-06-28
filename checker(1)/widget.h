#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include "checkerbutton.h"
#include "mydialog.h"
#include"mydialog1.h"
#include <QMouseEvent>
#include <QLabel>
#include <QDialog>
#include<QPropertyAnimation>
#include<QTimerEvent>
#include"networkserver.h"
#include"networksocket.h"
#include"networkdata.h"
#include"room.h"
#include"serverwait.h"
#include"chooseservice.h"

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
    int islegal();//判断移动是否合法

    int pixel2int(QPointF pixel);
    void CheckerMove(CheckerButton*btn,QPointF p);

    bool canJump(int x,int y);//排查是否可以进行下一次跳跃
    void shouldSwitcht2f();
    bool isfinish(int x);
    CheckerButton* int2btn(int x,int y);
    int place2num(char pln);
    char num2place(int);
    void setPort(QString p){
        Port = p;
        return;
    }
    NetworkServer* getServer(){
        return server;
    }
    void InitializeChecker();//画棋子
    void setPlayerTable();//列玩家列表
   // void initializeChecker(int x);

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);


private:
    Ui::Widget *ui;
    void DrawCheckerboard();
    QPointF loc[17][17];//棋盘每个位置的坐标
    int isfill[17][17];//棋盘每个位置是否有棋子
    CheckerButton* btn[6][10]; //六方棋子
    int playernum; //棋手个数
    int flag;  //判断下棋方
    bool isover[6];
    int overnum=0;

    QPointF chosen;//选中的棋子
    CheckerButton* checked;//被选中的棋子
    bool haveJumped;
    CheckerButton* jumped;

    QDialog *nobai;
   // QDialog *win;
   // QLabel *w;
    int chosenloc[2];//选中棋子所在位置
    bool ischosen=false;//是否有棋子被选中
    bool isobjset=false;
    QPointF obj;//目标位置
    int objloc[2];//目标位置坐标
    bool ischange=false;
    int btnx,btny;//记录选中棋子初始位置
    int step=0;
    QLabel* test;
    QLabel* nowplayer;
    QPushButton* end;
    bool shouldSwitch;
public:
    myDialog d;
    mydialog1 *z;
private:
    int totalstep=0;

    //stage2相关
public:
    NetworkServer* server;//监听,其中的clients队列应指向每一个客户端，进行通信；
private:
    QList<Room> roomList;//房间，每个room类里包括：房间号（roomID)
                         //游戏状态（gameOn）,玩家人数（playerNumber）,玩家列表（playerList):包含玩家类（Player）
                         //玩家类，内含玩家socket，玩家ID，玩家状态

    QString ranklist;
    QStringList overlist;//出局的人
    int id;//计时器id,负责倒计时
    QLabel* clock1;//显示倒计时提示
    QLabel* clock2;//显示时间
    QString Port;//端口号
public:
    serverwait* ServerWait;//房间等待界面
    chooseservice* ChooseServer;//服务端等待第一个玩家接入界面
    QList<QLabel*> PlayerTable;//局内玩家用户名表

signals:
    void shouldSwitchChanged();
    //终局判断，信号和槽函数未实现连接
    void someonewin(QString);//一方结束游戏信号
    void gameover();//游戏结束信号
public slots:
    void changeplayer();
  //  void someoneover(int i);
    void receiveData(QTcpSocket* client, NetworkData data);//服务端接受到客户端信号时的解析函数


};
#endif // WIDGET_H
