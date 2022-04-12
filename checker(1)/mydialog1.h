#ifndef MYDIALOG1_H
#define MYDIALOG1_H

#include<QDialog>

class QPushButton;
class QLabel;

class mydialog1:public QDialog
{
    Q_OBJECT;
public:
    explicit mydialog1(QWidget *parent=0);
    bool Quit() const
    {
        return pressquit;
    }
    ~mydialog1();
    mydialog1();
    void Quit();

private slots:
   void q();
private:

    QLabel *forsure;

    //QPushButton *quit;
    QPushButton *cont;  //继续游戏
    QPushButton *leav;   //退出

    bool pressquit;
};

#endif // MYDIALOG1_H
