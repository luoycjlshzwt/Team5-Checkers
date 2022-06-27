#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "networksocket.h"

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

private:
    Ui::Client *ui;
    NetworkSocket* socket; //负责通信的socket
};

#endif // CLIENT_H
