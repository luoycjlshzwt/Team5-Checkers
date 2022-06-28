#ifndef CHOOSECLIENT_H
#define CHOOSECLIENT_H

#include <QWidget>
#include<QTcpSocket>
#include<QString>
#include<QHostAddress>
#include<QNetworkInterface>
#include<QLineEdit>

#include<QValidator>
#include<QRegularExpression>
#include<QRegularExpressionValidator>


class QPushButton;
class QLabel;
class QLineEdit;
class QValidator;
class QRegExpValidator;
class QRegularExpression;

class chooseclient:public QWidget
{
public:
    chooseclient(QWidget *parent = nullptr);
    ~chooseclient();
    QHostAddress ips;
    QString ip;
    QString id;
    QString room;
    QString read_ip_address();
    QString getRoomID(){
        return this->ROOMS->text();
    }
    QString getName(){
        return IDS->text();
    }
    QPushButton* getYES(){
        return YES;
    }

private:
    //QTcpSocket* client;
    QLabel *IP;
    //QLabel *PORT;
    QLabel *ID;
    QLabel *ROOM;
    QPushButton *YES;
    QLineEdit *IPS;
    QLineEdit *ROOMS;
    //QLineEdit *PORTS;
    QLineEdit *IDS;
};


#endif // CHOOSECLIENT_H
