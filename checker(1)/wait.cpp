#include "wait.h"
#include "ui_wait.h"
//#include"clientwindow.h"

void Wait::qqq()
{
    QApplication::exit();
}

Wait::Wait(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wait)
{
    ui->setupUi(this);
    setWindowTitle("client");

    rea = ui->YES;
    exi=ui->EXIT;
    ro=new QLabel(this);
    ro->setGeometry(125,40,35,20);

    for(int i=0;i<6;i++)
    {
        ids[i]=new QLineEdit(this);
        sis[i]=new QLabel(this);
    }
    ids[0]=ui->name1;
    ids[1]=ui->name2;
    ids[2]=ui->name3;
    ids[3]=ui->name4;
    ids[4]=ui->name5;
    ids[5]=ui->name6;
    sis[0]=ui->situ1;
    sis[1]=ui->situ2;
    sis[2]=ui->situ3;
    sis[3]=ui->situ4;
    sis[4]=ui->situ5;
    sis[5]=ui->situ6;

    for(int i=0;i<6;i++){
        ids[i]->setReadOnly(true);
    }

    //connect(ui->EXIT,SIGNAL(clicked(bool)), this, SLOT(qqq()));
    connect(ui->YES, &QPushButton::clicked, [&]()  //加入游戏并关闭开始窗口
    {

                    /*ClientWindow *c=new ClientWindow;
                    c->setWindowTitle("client");
                    c->show();
                    ifsuccessed = true;
                    hide();*/

    });
}

Wait::~Wait()
{
    delete ui;
}
