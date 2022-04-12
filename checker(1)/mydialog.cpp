#include"mydialog.h"
#include<QPushButton>

myDialog::myDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    setMinimumSize(300, 300);
    setMaximumSize(500, 500);
    /*pix = new QLabel;
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/map.png"));
    pix->setFixedSize(600, 600);*/
    an = new QPushButton("");
    an->setMinimumSize(450, 500);
    an->setFlat(true);
    an->setIconSize(QSize(600, 600));
    an->setIcon(QIcon(":/image/background.png"));
    //setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    QPalette palette ;
    palette.setColor(QPalette::Window,QColor("#b0e0e6"));  //窗口背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    this->setWindowTitle("跳棋");

    joinSuccessed = false;

    checker = new QLabel("跳棋");

    ch_logo = new QLabel("START");

    checker->setAlignment(Qt::AlignCenter);
    ch_logo->setAlignment(Qt::AlignCenter);

    join = new QPushButton("START");
    quit = new QPushButton("EXIT");

    /*login->setStyleSheet("background-color:rgb(255,255,255");   设计窗口外观
    login->setStyleSheet("color:green");
    login->setStyleSheet("font:bold 20px");*/
    join->setStyleSheet("QPushButton {border: 0px solid #dadbde;border-radius: 5px;background-color: rgb(255,255,255);color:green;font:bold 25px;}");
    /*quit->setStyleSheet("background-color:rgb(255,255,255");
    quit->setStyleSheet("color:black");
    quit->setStyleSheet("font:bold 20px");*/
    quit->setStyleSheet("QPushButton {border: 0px solid #dadbde;border-radius: 5px;background-color:  rgb(255,255,255);color:black;font:bold 25px;}");

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(an);
    vbox->addWidget(join);
    vbox->addWidget(quit);
    setLayout(vbox);

    connect(quit, SIGNAL(clicked(bool)), this, SLOT(close()));  //不加入游戏
    connect(join, &QPushButton::clicked, [&]()  //加入游戏并关闭开始窗口
    {
            joinSuccessed = true;
            close();
    });
}

