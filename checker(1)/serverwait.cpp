#include "serverwait.h"
#include "ui_serverwait.h"
#include"widget.h"

serverwait::serverwait(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serverwait)
{
    ui->setupUi(this);

    /*b=new QPushButton(this);
    b->move(450,50);
    b->setText("ok");*/
    ui->name1->setReadOnly(true);
    ui->name2->setReadOnly(true);
    ui->name3->setReadOnly(true);
    ui->name4->setReadOnly(true);
    ui->name5->setReadOnly(true);
    ui->name6->setReadOnly(true);
    setWindowTitle("server");
//    connect(b, &QPushButton::clicked, [&]()  //加入游戏并关闭开始窗口
//    {

//                    Widget *c=new Widget;
//                    c->setWindowTitle("server");
//                    c->show();
//                    hide();
//    });
}

serverwait::~serverwait()
{
    delete ui;
}

void serverwait::setPlayerName(int i, QString name){
    switch(i){
    case 0:
        ui->name1->setText(name);
        break;
    case 1:
        ui->name2->setText(name);
        break;
    case 2:
        ui->name3->setText(name);
        break;
    case 3:
        ui->name4->setText(name);
        break;
    case 4:
        ui->name5->setText(name);
        break;
    case 5:
        ui->name6->setText(name);
    }
    return;
}

void serverwait::setPlayerReady(int i,bool isReady){
    if(isReady){
        switch(i){
        case 0:
            ui->situ1->setText("Ready");
            break;
        case 1:
            ui->situ2->setText("Ready");
            break;
        case 2:
            ui->situ3->setText("Ready");
            break;
        case 3:
            ui->situ4->setText("Ready");
            break;
        case 4:
            ui->situ5->setText("Ready");
            break;
        case 5:
            ui->situ6->setText("Ready");
            break;
        }
    }
    else{
        switch(i){
        case 0:
            ui->situ1->setText("Waiting");
            break;
        case 1:
            ui->situ2->setText("Waiting");
            break;
        case 2:
            ui->situ3->setText("Waiting");
            break;
        case 3:
            ui->situ4->setText("Waiting");
            break;
        case 4:
            ui->situ5->setText("Waiting");
            break;
        case 5:
            ui->situ6->setText("Waiting");
            break;
        }
    }
}
