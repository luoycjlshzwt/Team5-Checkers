#ifndef WAIT_H
#define WAIT_H

#include <QWidget>
#include<QLineEdit>
#include<QLabel>
#include<QPushButton>

namespace Ui {
class Wait;
}

class Wait : public QWidget
{
    Q_OBJECT

public:
    explicit Wait(QWidget *parent = nullptr);
    ~Wait();
    bool Join()const
    {
        return ifsuccessed;
    }
    //bool isready[6];
    QLineEdit *ids[6];
    QLabel *sis[6];
    QPushButton* rea;
    QPushButton *exi;

private slots:
   void qqq();

public:
    Ui::Wait *ui;
    bool ifsuccessed;
};

#endif // WAIT_H
