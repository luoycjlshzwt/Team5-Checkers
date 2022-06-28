#ifndef CHOOSESERVICE_H
#define CHOOSESERVICE_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
class QPushButton;
class QLabel;
class QLineEdit;

class chooseservice:public QWidget
{
public:
    chooseservice(QWidget *parent = nullptr);
    ~chooseservice();

private:
//    QTcpServer* service; //监听
//    QTcpSocket* commun;   //通信
//    QLabel *IP;
//    QLabel *PORT;
//    QPushButton *YES;
//    QLineEdit *IPS;
//    QLineEdit *PORTS;
    QLabel* loading;//等待玩家接入。这个窗口主要作为一个过渡作用
};

#endif // CHOOSESERVICE_H
