#include "chooseservice.h"

#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QMessageBox>
#include<QLineEdit>
#include"serverwait.h"

chooseservice::chooseservice(QWidget *parent) :
    QWidget(parent)
{
    this->resize( QSize( 400, 252 ));

    //setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //quit=new QPushButton(this);
    QPalette palette ;
    palette.setColor(QPalette::Window,QColor("#f8f8ff"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

        loading=new QLabel(this);
        loading->move(50,60);
        loading->setText("等待玩家进入房间……");
        loading->setStyleSheet("QLabel {color:black;font:bold 11px;}");
//    IP=new QLabel(this);
//    IP->move(50,60);
//    IP->setText("IP");
//    IP->setStyleSheet("QLabel {color:black;font:bold 11px;}");

//    /*PORT=new QLabel(this);
//    PORT->move(50,110);
//    PORT->setText("Port");
//    PORT->setStyleSheet("color:black;font:bold 11px;}");*/

//    IPS=new QLineEdit(this);
//    IPS->move(110,60);
//    IPS->setText("10.46.156.60");
//    IPS->setStyleSheet("QLineEdit{color:black;font:11px}");

//    /*PORTS=new QLineEdit(this);
//    PORTS->move(110,110);
//    PORTS->setText("9999");
//    PORTS->setStyleSheet("QLineEdit{color:black;font:11px}");*/

//    YES=new QPushButton(this);
//    YES->move(160,180);
//    YES->setText(tr("确 认"));
//    YES->setStyleSheet("QPushButton {background-color: rgb(255,255,255);color:blue;font:bold 12px;}");

//    connect(YES,&QPushButton::clicked,[&]()
//            {
//        serverwait *w=new serverwait;
//        w->setWindowTitle("server");
//        w->show();
//                    close();
//            });
//    show();
}

chooseservice::~chooseservice()
{
}
