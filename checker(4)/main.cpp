#include "widget.h"
#include "clientwindow.h"
#include "mydialog.h"
#include <QApplication>
#include"chooseclient.h"
#include"chooseservice.h"
#include<QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myDialog h;
    h.exec();
    int ifsta=h.Join();
    if(!ifsta)
        return a.exec();
    QString str=h.settype->currentText();
    QString ips = h.ip;
    QString ports=h.port;
    qDebug() << "debug: port is";
    //qDebug() << port.toInt();
        if(str=="Server")
        {
            Widget* w = new Widget;
            w->hide();
            //w->setPort(port);
            //w->initializeChecker(h.setplayer->currentText().toInt());
            //开始监听
            w->server->listen(QHostAddress::Any,9999);
            //test
            qDebug() << "start to listen" ;
            //test end
            //过渡界面显示
            w->ChooseServer->show();
            w->ServerWait->hide();
        }
        else if(str=="Client")
        {
            ClientWindow *e = new ClientWindow;
            qDebug() << "point 1";
            e->hide();
            //e->setPort(port);
            //建立连接
            e->getSocket()->hello(ips,ports.toInt());
            qDebug() << "say hello to" << ips;
            e->cc.show();
        }
//             e->getSocket()->hello("127.0.0.1",9999);
//             qDebug() << "say hello to" << ip;
//             //test
//             //qDebug() << "client send hello:" << port;
//             //test end
//             e->cc.show();

             /*ClientWindow *e2 = new ClientWindow;
             qDebug() << "point 1";
             e2->hide();
//             //e2->setPort(port);
//             //建立连接
             e2->getSocket()->hello(ips,ports.toInt());
//             //test
             //qDebug() << "client send hello:" << port;
//             //test end
             e2->cc.show();*/
//         //}
    return a.exec();
}
