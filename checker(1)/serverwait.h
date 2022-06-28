#ifndef SERVERWAIT_H
#define SERVERWAIT_H

#include <QWidget>
#include"room.h"
#include<QPushButton>
#include<QLabel>

namespace Ui {
class serverwait;
}

class serverwait : public QWidget
{
    Q_OBJECT

public:
    explicit serverwait(QWidget *parent = nullptr);
    ~serverwait();
    void setPlayerName(int i,QString name);
    void setPlayerReady(int ,bool);


private:
    Ui::serverwait *ui;
    QPushButton *b;
};

#endif // SERVERWAIT_H
