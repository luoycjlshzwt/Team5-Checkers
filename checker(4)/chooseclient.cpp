#include "chooseclient.h"

#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QMessageBox>
#include<QLineEdit>
#include"wait.h"

QString chooseclient::read_ip_address()
{
    QString ip_address;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&  ipAddressesList.at(i).toIPv4Address())
        {
            ip_address = ipAddressesList.at(i).toString();
            qDebug()<<ip_address;  //debug
            //break;
        }
    }
    if (ip_address.isEmpty())
        ip_address = QHostAddress(QHostAddress::LocalHost).toString();
    return ip_address;
}

chooseclient::chooseclient(QWidget *parent) :
    QWidget(parent)
{
    //初始化
    /*client = new QTcpSocket(this);
    ui->IPS->setText("127.0.0.1");
    ui->PORTS->setText("9999");
    //连接服务器
    client->connectToHost(QHostAddress(ui->IPS->text()),ui->PORTS->text().toInt());*/

    this->resize( QSize( 400, 252 ));
    //setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //quit=new QPushButton(this);
    ip=read_ip_address();

    QPalette palette ;
    palette.setColor(QPalette::Window,QColor("#f8f8ff"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    /*IP=new QLabel(this);
    IP->move(66,60);
    IP->setText("IP");
    IP->setStyleSheet("QLabel {color:black;font:bold 11px;}");*/

    ID=new QLabel(this);
    ID->move(66,76);
    ID->setText("ID");
    ID->setStyleSheet("QLabel{color:black;font:bold 11px;}");

    ROOM=new QLabel(this);
    ROOM->move(66,125);
    ROOM->setText("Room");
    ROOM->setStyleSheet("QLable{color:black;font:bold 11px;}");

    IPS=new QLineEdit(this);
    IPS->move(110,60);
    IPS->setText(ip);
    IPS->setStyleSheet("QLineEdit{color:black;font:11px}");
    IPS->setReadOnly(true);
    IPS->hide();

    IDS=new QLineEdit(this);
    IDS->move(130,76);
    IDS->setPlaceholderText("请输入用户名...");
    IDS->setStyleSheet("QLineEdit{color:black;font:11px}");
    QRegularExpression rx1;
    rx1.setPattern("[A-Za-z0-9_]{1,20}");
    QValidator *validator0 = new QRegularExpressionValidator(rx1,this);
    IDS->setValidator(validator0);

    ROOMS=new QLineEdit(this);
    ROOMS->move(130,125);
    ROOMS->setPlaceholderText("请输入房间号...");
    ROOMS->setStyleSheet("QLineEdit{color:black;font:11px}");
    QRegularExpression rx2;
    rx2.setPattern("^([1-9]{1}[0-9]{0,4})$");
    QValidator *validator1 = new QRegularExpressionValidator(rx2,this);
    ROOMS->setValidator(validator1);

    YES=new QPushButton(this);
    YES->move(160,175);
    YES->setText(tr("确 认"));
    YES->setStyleSheet("QPushButton {background-color: rgb(255,255,255);color:blue;font:bold 12px;}");

    connect(YES,&QPushButton::clicked,[&]()
            {
//        ip=IPS->text();
//        ip=ip.mid(0,6);
//        room=ROOMS->text();
//        id=IDS->text();
//        //if(ip=="10.46."||ip=="10.47.")
//        //{
//            id=IDS->text();
//            room=ROOMS->text();
//        Wait *d=new Wait;
//        d->setWindowTitle("waiting");
//        d->show();
//        int ifs=d->Join();
//        if(!ifs)
//           exit(0);
//                    hide();
//        }

    });

}
chooseclient::~chooseclient()
{

}
