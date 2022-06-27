//.h
#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include <QComboBox>
#include <QDebug>
#include<QValidator>
#include<QString>
#include<QRegularExpression>
#include<QRegularExpressionValidator>

class myDialog:public QDialog
{
    Q_OBJECT

public:
    myDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    bool Join() const
    {
        return joinSuccessed;
    }
    QLabel *pix;
    QPoint& operator=(QPointF& f){
       p.setX((int)f.x());
       p.setY((int)f.y());
       return p;
    };
    QComboBox *setplayer;
    QComboBox *settype;
    QString port;
    QString ip;
    QPushButton* getJoin(){
        return join;
    }

protected:
    void mpe(QMouseEvent *mo);

private:
    QLabel *ch_logo;
    QLabel *checker;
    QLabel *PORT;
    QPushButton *join;  //加入游戏
    QPushButton *an;
    QPushButton *quit;  //退出游戏
    QLabel *IP;

    QLabel *setp;

    bool joinSuccessed;

    QPoint p;
    QLineEdit *PORTS;
    QLineEdit *IPS;



//signals:
//    void plnum(QString);
};

#endif // MYDIALOG_H
