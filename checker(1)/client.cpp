#include "client.h"
#include "ui_client.h"
#include <QMessageBox>
#in

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    //初始化socket
    this->socket = new NetworkSocket(new QTcpSocket(), this);

    connect(socket, &NetworkSocket::receive, this, &Client::receive);
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    });

}

Client::~Client()
{
    delete ui;
}
