#include "mydialog1.h"

#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QMessageBox>

void mydialog1::q()
{
    QApplication::exit();
}

mydialog1::mydialog1(QWidget *parent):QDialog(parent)
{
    this->resize( QSize( 500, 260 ));

    QPalette palette ;
    palette.setColor(QPalette::Window,QColor("#b0e0e6"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    //setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //quit=new QPushButton(this);

    forsure=new QLabel(this);
    forsure->move(50,100);
    forsure->setText("Are you sure you want to leave?");
    forsure->setStyleSheet("QLabel{color:blue;font:bold 25px;}");

    leav=new QPushButton(this);
    leav->move(130,180);
    leav->setText(tr("YES"));
    leav->setStyleSheet("QPushButton {background-color: rgb(255,255,255);color:black;font:bold 20px;}");

    cont=new QPushButton(this);
    cont->move(270,180);
    cont->setText(tr("NO"));
    cont->setStyleSheet("QPushButton {background-color: rgb(255,255,255);color:blue;font:bold 20px;}");

    connect(cont, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(leav,SIGNAL(clicked(bool)), this, SLOT(q()));

}
mydialog1::~mydialog1()
{

}
