#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QMessageBox>
#include<QPainter>
#include<QPointF>
#include<QDebug>
#include<QLabel>
#include<QFont>
#include<QDateTime>
#include"networkdata.h"
#include"chooseclient.h"


#define I 40 //横向间距
#define JX 20
#define JY sqrt(3)*20 //行距
#define R 20
#define RR 34 //棋子半径
#define RRR 24

#define red 3
#define blue 2
#define green 1
#define pink 0
#define purple 5
#define orange 4

void ClientWindow::cbuttonpress()
{
    z = new mydialog1(this) ;//将类指针实例化
    z->show();
}

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);

    //passit1(dd.port);
    //passit2(cc.room,cc.id);

    //初始化socket
    socket = new NetworkSocket(new QTcpSocket(),this);
    connect(socket, &NetworkSocket::receive, this, &ClientWindow::receive);
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    });

    //页面2：输入roomid和用户名 发送加入房间信号
    connect(this->cc.getYES(),&QPushButton::clicked,this,[=](){
       this->RoomID = cc.getRoomID();
       this->PlName = cc.getName();
        socket->send(NetworkData(OPCODE::JOIN_ROOM_OP,RoomID,PlName));
        //test
        qDebug() << PlName << " send JOIN_ROOM_OP of " << RoomID;
        //test end
    });

    //页面3：等待界面，ready按钮功能实现
    connect(ww.rea,&QPushButton::clicked,this,[=](){
        if(cc.getRoomID()!=""&&cc.getRoomID()!="请输入房间号..."&&cc.getName()!=""&&cc.getName()!="请输入用户名...")
        {
        NetworkData sure(OPCODE::PLAYER_READY_OP,PlName,"");
        socket->send(sure);
        qDebug() << PlName << " says he is ready";
        }
    });

    connect(socket->base(), &QAbstractSocket::errorOccurred, this, &ClientWindow::displayError);
    connect(socket->base(), &QAbstractSocket::connected, this, &ClientWindow::connected);

    //test
    infoTest = new QPushButton(this);
    infoTest->setText("test");
    infoTest->setGeometry(70,600,50,50);
    connect(infoTest,&QPushButton::clicked,this,[=](){
        socket->send(NetworkData(OPCODE::MOVE_OP,QString(myPos),QString("-2 5 -2 3")));
    });
    infoTest->hide();
    //test end

    connect(ww.exi,&QPushButton::clicked,this,[=](){
        socket->send(NetworkData(OPCODE::LEAVE_ROOM_OP,RoomID,PlName));
        for(int i=0;i<6;i++)
        {
//            if(ww.ids[i]->text()==PlName)
//            {
//                ww.sis[i]->setText("Waiting");
//                break;
//            }
            ww.ids[i]->setText("");
            ww.sis[i]->setText("Waiting");
        }
        players.clear();
        playerState.clear();
        cc.show();
        ww.hide();
    });

    //开始界面 设置玩家人数
   /* myDialog *d = new myDialog;
    d->exec();
    int ifstart=d->Join();
    QString str=d->setplayer->currentText();
        if(str=="2")
            playernum=2;
        else if(str=="3")
            playernum=3;
        else
            playernum=6;
    if(!ifstart)
        exit(0);*/

    connect(ui->QUIT, SIGNAL(clicked(bool)), this, SLOT(cbuttonpress()));  //弹出退出窗口
    //设置禁止摆烂弹窗
        

        
    nobai = new QDialog(this);
    QLabel *lb = new QLabel("禁止摆烂",nobai);
    nobai->setFixedSize(200,100);
    lb->setFont(QFont("Microsoft YaHei",20,75));
    lb->setGeometry(45,25,200,50);
    nobai->hide();

    this->setFixedSize(700,700);

    DrawCheckerboard();
    //初始化部分
        //初始化秒表
        clock1 = new QLabel("剩余时间",this);
        clock1->setFont(QFont("Microsoft YaHei",20));
        clock1->setGeometry(70,500,300,50);
        clock1->setStyleSheet("color:brown;");
        clock1->hide();
        clock2 = new QLabel("15 s",this);
        clock2->setFont(QFont("Agency FB",20));
        clock2->setStyleSheet("color:brown;");
        clock2->setGeometry(70,530,300,50);
        clock2->hide();

        //初始化isfill
        for(int i1=0;i1<17;i1++){
            for(int i2=0;i2<17;i2++){
                isfill[i1][i2]=0;
            }
        }

        //初始化是否需要更换棋手
        shouldSwitch=false;
        //初始化是否胜利
        iswin=false;
        //记录上一步是否为跳子
        haveJumped=false;
        //初始化游戏结束排名表
        rank = new Rank(this);
        if(overnum==playernum)
        rank->show();
        rank->hide();

  //      initializeChecker(QString("data2"));//这一处最后应该是需要删掉的

        //初始化回合结束按钮
        end = new QPushButton(this);
        end->setText("回合结束");
        end->setGeometry(293,640,100,50);

        //实现更换执棋方功能
        connect(end,&QPushButton::clicked,this,[=](){
            if(ischosen==true&&ischange==false&&!(chosenloc[0]==btnx&&chosenloc[1]==btny)){//当没有换过且棋子不在初始位置时换player
//                shouldSwitch=true;
//                shouldSwitcht2f();
//                shouldSwitch=false;
//                ischange=true;
//                qDebug() << "player changed";
                NetworkData mv(OPCODE::MOVE_OP,QString(myPos),path);
                socket->send(mv);//发送move信号
                //test
                qDebug() << myPos << "send jump MOVE_OP ";
                qDebug() << "path is " << path;
                //test end
            }
            else if(chosenloc[0]==btnx&&chosenloc[1]==btny||ischosen==false){
                nobai->show();
            }
        });

        //connect(this,SIGNAL(shouldSwitchChanged()),this,SLOT(changeplayer()));

}
void ClientWindow::shouldSwitcht2f(){
    if(shouldSwitch==true){
        shouldSwitch=false;
        emit shouldSwitchChanged();
    }
}

void ClientWindow::changeplayer(){
    for(int i=0;i<10;i++){
        btn[flag][i]->setCheckable(false);
    }
//    Su:我觉得flag的修改可以不用客户端自己来，当客户端接收到服务端的行棋信号时，根据信号内容修改即可
    flag = (flag+1)%playernum;
    while(isover[flag]){
        qDebug()<<"isover";
        flag = (flag+1)%playernum;
    }
    if(place2num(myPos)==flag)
    for(int j=0;j<10;j++){
        btn[flag][j]->setCheckable(true);
    }
   /* switch(flag){
    case red:
        nowplayer->setText("Player:  RED");
        nowplayer->setStyleSheet("color:red;");
        break;
    case blue:
        nowplayer->setText("Player: BLUE");
        nowplayer->setStyleSheet("color:blue;");
        break;
    case green:
        nowplayer->setText("Player:GREEN");
        nowplayer->setStyleSheet("color:green;");
        break;
    case pink:
        nowplayer->setText("Player:PINK");
        nowplayer->setStyleSheet("color:#DB7093;");
        break;
    case purple:
        nowplayer->setText("Player:PURPLE");
        nowplayer->setStyleSheet("color:#800080;");
        break;
      case orange:
        nowplayer->setText("Player:ORANGE");
        nowplayer->setStyleSheet("color:#FF4500;");
        break;
    }*/
    haveJumped=false;
    ischosen=false;
    isobjset=false;
    checked=NULL;
    jumped=NULL;
    step=0;
    path = "";
}

void ClientWindow::paintEvent(QPaintEvent *)
{
    DrawCheckerboard();       //棋盘

    update();          //强制更新界面
}


void ClientWindow::DrawCheckerboard(void)
{
    QPainter plot(this);
    plot.setRenderHint(QPainter::Antialiasing, true);    //防止棋盘变形
    plot.setPen(QPen(QColor(Qt::gray), 3));                        //给边框绘制颜色，灰色，线宽3
    plot.setBrush(Qt::transparent);
    QPointF bp(330,330);                                    //set base point原点
    //绘制棋盘所有的格子
    for(int i=-4; i<5; i++)
    {
        for(int j=-4; j<5; j++)
        {
            plot.drawEllipse(bp.x()+i*I-j*JX,bp.y()+j*JY, R, R);
            loc[i+8][-j+8].setX(bp.x()+i*I-j*JX+R/2);
            loc[i+8][-j+8].setY(bp.y()+j*JY+R/2);
        }
    }//左下到右上的菱形
    for(int j=5;j<=8;j++){
        for(int i=j-4;i<=4;i++){
            plot.drawEllipse(bp.x()+i*I-j*JX,bp.y()+j*JY, R, R);
            loc[i+8][-j+8].setX(bp.x()+i*I-j*JX+R/2);
            loc[i+8][-j+8].setY(bp.y()+j*JY+R/2);
            plot.drawEllipse(bp.x()-i*I+j*JX,bp.y()-j*JY, R, R);
            loc[-i+8][j+8].setX(bp.x()-i*I+j*JX+R/2);
            loc[-i+8][j+8].setY(bp.y()-j*JY+R/2);
        }
    }//上下三角形
    for(int i=5;i<=8;i++){
        for(int j=i-4;j<=4;j++){
            plot.drawEllipse(bp.x()+i*I-j*JX,bp.y()+j*JY, R, R);
            loc[i+8][-j+8].setX(bp.x()+i*I-j*JX+R/2);
            loc[i+8][-j+8].setY(bp.y()+j*JY+R/2);
            plot.drawEllipse(bp.x()-i*I+j*JX,bp.y()-j*JY, R, R);
            loc[-i+8][j+8].setX(bp.x()-i*I+j*JX+R/2);
            loc[-i+8][j+8].setY(bp.y()-j*JY+R/2);
        }
    }//左上右下三角形


}

void ClientWindow::mousePressEvent(QMouseEvent *ev){
    //录入obj 并进行棋子移动
    qDebug()<<"clicked"<<ischosen;
    if(ischosen){
          //反映鼠标点击点坐标
        //       QString posi = QString("%1,%2").arg(ev->pos().rx()).arg(ev->pos().ry());
        //test->setText(posi);

        //在这里判断所点位置是否在圆圈内，若在圆圈内，则为合法，直接设置目标位置obj
        QPointF td=ev->pos();
        int l=pixel2int(td);
        if((td.rx()-loc[l/17][l%17].rx())*(td.rx()-loc[l/17][l%17].rx())+(td.ry()-loc[l/17][l%17].ry())*(td.ry()-loc[l/17][l%17].ry())>RR*RR/4){
            //test->setText("out range");//不合法
        }
        else{
            isobjset=true;//is obj set
            obj.setX(loc[l/17][l%17].rx()-RR/4);
            obj.setY(loc[l/17][l%17].ry()-RR/4);
            objloc[0] = l/17;
            objloc[1] = l%17;
            qDebug()<<objloc[0]<<' '<<objloc[1];
            int mv=islegal();
            if(mv&&isobjset){
                qDebug()<<"legal move";
                qDebug()<<"now the path is " << path;
                CheckerMove(checked,obj);
                isobjset=false;
                if(mv==1){
                    //shouldSwitcht2f();
                    NetworkData mv(OPCODE::MOVE_OP,QString(myPos),path);
                    socket->send(mv);//发送move信号
                    //test
                    qDebug() << myPos << " send flat MOVE_OP";
                    qDebug() << "path is " << path;
                    //test end
                }
                else if(mv==2){
                    jumped=checked;
                    chosenloc[0]=objloc[0];
                    chosenloc[1]=objloc[1];
                }
            }
        }
    }
}

int ClientWindow::islegal(){
    //判断是否已经有选中棋子
    if(!ischosen){
        //test->setText("no chosen checker");
        return 0;
    }
    //判断目标点是否为空位
    if(isfill[objloc[0]][objloc[1]]){
        //test->setText("object is filled");
        return 0;
    }
    //判断是否平动
    bool flatmove;
    flatmove=(objloc[0]==chosenloc[0]+1&&objloc[1]==chosenloc[1])
            ||(objloc[0]==chosenloc[0]-1&&objloc[1]==chosenloc[1])
            ||(objloc[0]==chosenloc[0]-1&&objloc[1]==chosenloc[1]+1)
            ||(objloc[0]==chosenloc[0]+1&&objloc[1]==chosenloc[1]-1)
            ||(objloc[0]==chosenloc[0]&&objloc[1]==chosenloc[1]+1)
            ||(objloc[0]==chosenloc[0]&&objloc[1]==chosenloc[1]-1);
    if(flatmove&&haveJumped==false){
        ischosen=false;
        shouldSwitch=true;//阻止下一步
        qDebug() << "flat move made";
        return 1;
    }

    //判断连跳
    if(haveJumped&&jumped!=checked){
        //test->setText("jump checker changed");
        return 0;
    }
    //判断是否为跳跃
    int midloc[2];
    midloc[0]=(objloc[0]+chosenloc[0])/2;
    midloc[1]=(objloc[1]+chosenloc[1])/2;
    if((objloc[0]==chosenloc[0]+2&&objloc[1]==chosenloc[1])
     ||(objloc[0]==chosenloc[0]-2&&objloc[1]==chosenloc[1])
     ||(objloc[0]==chosenloc[0]-2&&objloc[1]==chosenloc[1]+2)
     ||(objloc[0]==chosenloc[0]+2&&objloc[1]==chosenloc[1]-2)
     ||(objloc[0]==chosenloc[0]&&objloc[1]==chosenloc[1]+2)
     ||(objloc[0]==chosenloc[0]&&objloc[1]==chosenloc[1]-2)){
        if(isfill[midloc[0]][midloc[1]]){
            if(!canJump(chosenloc[0],chosenloc[1])){
               shouldSwitch=true;
            }
            else{
                shouldSwitch=false;
            }
            //test->setText("jump made");
            haveJumped=true;
            return 2;
        }
    }
    //排查是否可以进行下一次跳跃
    //test->setText("undefined move");
    return 0;
}


int ClientWindow::pixel2int(QPointF pixel){
    int x=0,y=0;
    double d=(loc[0][0].rx()-pixel.rx())*(loc[0][0].rx()-pixel.rx())+(loc[0][0].ry()-pixel.ry())*(loc[0][0].ry()-pixel.ry());
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            if(loc[i][j].isNull()==false){
                if(d>(loc[i][j].rx()-pixel.rx())*(loc[i][j].rx()-pixel.rx())+(loc[i][j].ry()-pixel.ry())*(loc[i][j].ry()-pixel.ry())){
                    x=i;
                    y=j;
                    d=(loc[i][j].rx()-pixel.rx())*(loc[i][j].rx()-pixel.rx())+(loc[i][j].ry()-pixel.ry())*(loc[i][j].ry()-pixel.ry());
                }
            }
        }
    }
    qDebug()<<x<<' '<<y;
    return x*17+y;//进行一个下标的转换
}


void ClientWindow::CheckerMove(CheckerButton*btn,QPointF p){
    if(iswin)
        return;//赢了就不让动 su
    qDebug()<<btn->x<<" "<<btn->y;
    qDebug()<<btn->player;
    if(!step){
        for(int i=0;i<20;i++){
            pointpath[btn->player][i]->hide();
        }
    }
    qDebug()<<"hide done";
    QPropertyAnimation *anim = new QPropertyAnimation(btn, "pos", this);
    anim->setDuration(500);
    QPointF obj;
    obj.setX(loc[btn->x][btn->y].rx()-RR/4-R/2+1);
    obj.setY(loc[btn->x][btn->y].ry()-RR/4-R/2+0.5);
    qDebug()<<obj;
    anim->setStartValue(obj);
    anim->setEndValue(QPointF(p.rx()-R/2+1,p.ry()-R/2+0.5));
  //  anim->start(QPropertyAnimation::DeleteWhenStopped);
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    group->addAnimation(anim);
    group->addPause(2000);
    group->start(QPropertyAnimation::DeleteWhenStopped);
   QString label_style;
    switch(btn->player){
    case pink:
        label_style="min-width:8px;min-height:8px;max-width:8px;max-height:8px;border-radius:4px;background:#DB7093";
        break;
    case green:
        label_style="min-width:8px;min-height:8px;max-width:8px;max-height:8px;border-radius:4px;background:green";
        break;
    case blue:
        label_style="min-width:8px;min-height:8px;max-width:8px;max-height:8px;border-radius:4px;background:blue";
        break;
    case red:
        label_style="min-width:8px;min-height:8px;max-width:8px;max-height:8px;border-radius:4px;background:red";
        break;
    case orange:
        label_style="min-width:8px;min-height:8px;max-width:8px;max-height:8px;border-radius:4px;background:#FF4500";
        break;
    case purple:
        label_style="min-width:8px;min-height:8px;max-width:8px;max-height:8px;border-radius:4px;background:#800080";
        break;
    }

    pointpath[btn->player][step]->setStyleSheet(label_style);
    pointpath[btn->player][step]->setGeometry(loc[btn->x][btn->y].rx()-R/4+1,loc[btn->x][btn->y].ry()-R/4+0.5,1,1);
    pointpath[btn->player][step]->show();
    btn->x=objloc[0];
    btn->y=objloc[1];
    isfill[objloc[0]][objloc[1]]=btn->player+1;
    isfill[chosenloc[0]][chosenloc[1]]=0;
    ischange=false;
    step++;

    //test
        qDebug() << "chosen is" << chosenloc[0]-8 << "," << chosenloc[1]-8;
        qDebug() << "test:obj is " << objloc[0]-8 << "," << objloc[1]-8;
    //test end
    if(step==1){
        path = "";
        path = QString(QString::number(chosenloc[0]-8).append(" ").append(QString::number(chosenloc[1]-8)));
    }
    path.append(" ").append(QString::number(objloc[0]-8).append(" ").append(QString::number(objloc[1]-8)));
    //test
    qDebug() << "path now is " << path;
    //test end
    /*totalstep++;
    if(totalstep>60*playernum){
        //isfinish();
    }*/
}

bool ClientWindow::canJump(int x,int y){//不能跳回原位
    int direction[6][2]={{2,2},{0,2},{-2,0},{-2,-2},{0,-2},{2,0}};
        for(int t=0;t<6;t++){
            int x1 = objloc[0]+direction[t][0];
            int y1 = objloc[1]+direction[t][1];
            if((x1==objloc[0]+2&&y1==objloc[1])
             ||(x1==objloc[0]-2&&y1==objloc[1]+2)
             ||(x1==objloc[0]&&y1==objloc[1]+2)
             ||(x1==objloc[0]-2&&y1==objloc[1])
             ||(x1==objloc[0]+2&&y1==objloc[1]-2)
             ||(x1==objloc[0]&&y1==objloc[1]-2))
                if(isfill[(x1+objloc[0])/2][(y1+objloc[1])/2]&&x1!=x&&y1!=y)
                    return true;

    }
    return false;
}
ClientWindow::~ClientWindow()
{
    delete ui;
}

int timeLeft=15;

void ClientWindow::receive(NetworkData data){
    qDebug() << "receive data" << data.encode();
    switch(data.op){
        case OPCODE::JOIN_ROOM_OP://有新玩家加入 判定了
        {
        //请求合法性判定
        bool isValid=true;
        if(data.data1.length()>20||data.data1.isEmpty())
            isValid=false;
        else{
            for(int i=0;i<data.data1.length();i++)
            {
                char ch = data.data1.toStdString()[i];
                if((ch>=48&&ch<=57)
                 ||(ch>=65&&ch<=90)
                 ||(ch>=97&&ch<=122)
                        ||ch==95){
                    continue;
                }
                else{
                    isValid = false;
                    break;
                }
            }
        }
        if(!isValid){
            QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
            break;
        }
        //test
        qDebug() << "client receive JOIN_ROOM_OP";
        //test end
        players.append(data.data1);
        playerState.append(0);
        ww.ids[players.length()-1]->setText(players.at(players.length()-1));
        playernum++;
        }
        break;
        case OPCODE::JOIN_ROOM_REPLY_OP://加入房间成功 已判定
        {
        //test
        qDebug() << "client receive JOIN_ROOM_REPLY_OP";
        //test end
        //设置已有玩家状态
        if(data.data2==""){
            players.clear();
            players.append(PlName);
            playerState.clear();
            playerState.append(0);
        }
        else{
            players = data.data1.split(" ");//载入已有玩家姓名
            if(players[players.length()-1]==""){
                players.removeLast();
            }
            if(players.length()!=data.data2.length()){
                //玩家数目与状态数目不一致
                break;
            }
            for(int i=0;i<data.data2.length();i++){
                playerState.append(QString(data.data2.at(i)).toInt());
            }
            players.append(PlName);
            playerState.append(0);
            playernum = players.length();
        }
//          qDebug() << "previous player name is " << players;
//          qDebug() << "now player number is " << players.length();
        for(int i=0;i<players.length();i++)
        {
            ww.ids[i]->setText(players.at(i));
            if(playerState.at(i))
                ww.sis[i]->setText("ready");
        }
            ww.ro->setText(RoomID);
        cc.hide();
        ww.show();
        }
        break;
        case OPCODE::LEAVE_ROOM_OP://有其他玩家离开了房间 已判定
        {
            //请求合法性判定
            bool isValid=true;
            if(data.data1.length()>20||data.data1.isEmpty())
                isValid=false;
            else{
                for(int i=0;i<data.data1.length();i++)
                {
                    char ch = data.data1.toStdString()[i];
                    if((ch>=48&&ch<=57)
                     ||(ch>=65&&ch<=90)
                     ||(ch>=97&&ch<=122)
                            ||ch==95){
                        continue;
                    }
                    else{
                        isValid = false;
                        break;
                    }
                }
            }
            if(!isValid){
                QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                break;
            }
            //test
            qDebug() << "client receive LEAVE_ROOM_OP";
            //test end

            int Index = players.indexOf(data.data1);
            if(Index>=0&&Index<=players.length()){
                players.removeAt(Index);
                playerState.removeAt(Index);
                for(int m=Index;m<playernum-1;m++){
                    ww.ids[m]->setText(ww.ids[m+1]->text());
                    ww.sis[m]->setText(ww.sis[m+1]->text());
                }
                ww.ids[playernum-1]->setText("");
                ww.sis[playernum-1]->setText("Waiting");
            }
        }
        break;
        case OPCODE::CLOSE_ROOM_OP://关闭房间 应该是回到进房界面 task里好像没有设计
            cc.show();
            ww.hide();
        break;
        case OPCODE::PLAYER_READY_OP://有玩家准备就绪 已判定
        {
        //请求合法性判定
        bool isValid=true;
        if(data.data1.length()>20||data.data1.isEmpty())
            isValid=false;
        else{
            for(int i=0;i<data.data1.length();i++)
            {
                char ch = data.data1.toStdString()[i];
                if((ch>=48&&ch<=57)
                 ||(ch>=65&&ch<=90)
                 ||(ch>=97&&ch<=122)
                        ||ch==95){
                    continue;
                }
                else{
                    isValid = false;
                    break;
                }
            }
        }
        if(!isValid){
            QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
            break;
        }
        //test
        qDebug() << "client receive PLAYER_READY_OP";
        //test end
        if(players.indexOf(data.data1)>=0){
            playerState[players.indexOf(data.data1)] = 1;
            ww.sis[players.indexOf(data.data1)]->setText("Ready");
        }
        }
        break;
        case OPCODE::START_GAME_OP://开始游戏
           {
            if((!data.data1.isEmpty())&&(!data.data2.isEmpty())){
                //test
                qDebug() << "client receive START_GAME_OP";
                //test end
                QStringList pls = data.data1.split(" ");
                qDebug() << "test playerList:" << pls;
                if(pls[pls.length()-1]==""){
                    pls.removeLast();
                }

                QStringList seq = data.data2.split(" ");
                if(seq[seq.length()-1]==""){
                    seq.removeLast();
                }

               /* qDebug() << "test seqList:" << seq;
                if(pls.length()!=seq.length()||(!(pls.length()==2||pls.length()==3||pls.length()==6))){

                    //玩家人数与序列人数不符 或玩家人数不对
                    QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                    break;
                }*/
                playernum=seq.length();
                for(int i=0;i<playernum;i++){
                    players.replace(place2num(seq[i].toUtf8().at(0)),pls[i]);
                }
                myPos = seq.at(pls.indexOf(PlName)).toLatin1()[0];
                //接下来需要设定 只有己方棋子可动,以及根据data2画棋盘
                initializeChecker(data.data2);//画棋子
                setPlayerTable();
                ww.hide();
                this->setWindowTitle(QString("Client %1").arg(PlName));
                this->show();
                qDebug()<<myPos<<' '<<place2num(myPos);
            }
            else{
                QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                break;
            }
        }
        break;
        case OPCODE::START_TURN_OP://新玩家回合开始
        {
            if((!data.data1.isEmpty())&&(!data.data2.isEmpty()))
            {
                //在考虑做通过发过来的时间差确定延迟时间
                if(data.data1.toLatin1()[0]==myPos)//本人下棋
                {
                    //test
                    qDebug() << PlName <<" receive START_TURN_OP";
                    //test end
                  
                    timeLeft=15;//暂时不考虑延迟
                    id=startTimer(1000);
                    clock2->setText("15 s");
                    clock1->show();
                    clock2->show();
                    haveJumped=false;
                    ischosen=false;
                    isobjset=false;
                    checked=NULL;
                    jumped=NULL;
                    path = "";
                    flag =place2num(myPos);
                    step=0;
                    isaimove=false;

                    if(aiflag&&!isaimove){
                           ai();
                           isaimove=true;
                    }
                    else{
                        for(int i=0;i<10;i++){
                            btn[place2num(myPos)][i]->setCheckable(true);
                        }
                    }

                    flag = place2num(myPos);
                    qDebug()<<"now flag"<<myPos<<' '<<flag;                 
                    nowplayer->setText("Your Turn");
                    switch(flag){
                    case red:
                        nowplayer->setStyleSheet("color:red;");
                        break;
                    case blue:
                        nowplayer->setStyleSheet("color:blue;");
                        break;
                    case green:
                        nowplayer->setStyleSheet("color:green;");
                        break;
                    case pink:
                        nowplayer->setStyleSheet("color:#DB7093;");
                        break;
                    case purple:
                        nowplayer->setStyleSheet("color:#800080;");
                        break;
                    case orange:
                        nowplayer->setStyleSheet("color:#FF4500;");
                        break;
                    }
                    //nowplayer->setText(QString("Player:%1").arg(PlName));
                }
                else{
                    int tmp = flag;
                    flag = place2num(data.data1.toUtf8()[0]);
                    if(flag>=playernum){
                        QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                        flag = tmp;
                        break;
                    }
                    step=0;
                    nowplayer->show();
                    nowplayer->setText(QString("Now:%1").arg(players.at(flag)));
                    switch(flag){
                    case red:
                        nowplayer->setStyleSheet("color:red;");
                        break;
                    case blue:
                        nowplayer->setStyleSheet("color:blue;");
                        break;
                    case green:
                        nowplayer->setStyleSheet("color:green;");
                        break;
                    case pink:
                        nowplayer->setStyleSheet("color:#DB7093;");
                        break;
                    case purple:
                        nowplayer->setStyleSheet("color:#800080;");
                        break;
                    case orange:
                        nowplayer->setStyleSheet("color:#FF4500;");
                        break;
                    }
                }
            }
        }
        break;
        case OPCODE::MOVE_OP://移动棋子
            {
                if(data.data1.isEmpty()||data.data2.isEmpty()){
                    //data1 data2不能为空
                    QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                    break;
                }
                //test
                qDebug() << PlName<< "receive MOVE_OP";
                qDebug() << "player is " << data.data1;
                qDebug() << "path is " << data.data2;
                //test end
                //nowplayer->setText(QString("Player:%1").arg(players.at(data.data1.toLatin1()[0]-65)));
                int nowPlpos;//该玩家的ABCDEF对应在btn里的序号
                nowPlpos = place2num(data.data1.toLatin1()[0]);
                if(data.data2=="-1"){
                    if(nowPlpos==place2num(myPos)){
                       killTimer(id);
                       nowplayer->setText("OUT!");
                       nowplayer->setStyleSheet("color:grey");
                    }
                    //移除该玩家所有棋子
                    for(int i=0;i<10;i++){
                        isfill[btn[nowPlpos][i]->x][btn[nowPlpos][i]->y]=0;
                        btn[nowPlpos][i]->close();
                     delete btn[nowPlpos][i];
                    }
                    PlayerTable[place2num(data.data1.toUtf8()[0])+1]->setStyleSheet("color:grey");
                }
                else{
                if(data.data1[0].toLatin1()==myPos&&path==data.data2){//自己的移动合法 服务端发来反馈
                    this->killTimer(id);
                    changeplayer();
                    nowplayer->setText("");
                    nowplayer->setStyleSheet("color:grey;");
                    clock1->hide();
                    clock2->hide();
                    break;
                }
                else {
                    flag = place2num(data.data1.toLatin1()[0]);
                    QStringList checkerpath = data.data2.split(" ");
                    if(checkerpath.at(checkerpath.length()-1)==""){
                        checkerpath.removeLast();
                    }
                    if(checkerpath.length()%2==1){
                        //path格式不合法，应为偶数
                        QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                        break;
                    }
                    int stepnum = checkerpath.length()/2;
                    chosenloc[0]= checkerpath.at(0).toInt()+8;
                    chosenloc[1]= checkerpath.at(1).toInt()+8;
                    for(int i=0;i<10;i++){
                        if(btn[nowPlpos][i]->x==chosenloc[0]
                           &&btn[nowPlpos][i]->y==chosenloc[1]){
                            for(int j=1;j<stepnum;j++){
                                objloc[0]= checkerpath.at(2*j).toInt()+8;
                                objloc[1]=checkerpath.at(2*j+1).toInt()+8;
                                qDebug()<<"otherchecker move"<<nowPlpos<<' '<<i<<' '<<objloc[0]<<' '<<objloc[1];
                                QPointF obj;
                                obj.setX(loc[objloc[0]][objloc[1]].rx()-RR/4);
                                obj.setY(loc[objloc[0]][objloc[1]].ry()-RR/4);
                                CheckerMove(btn[nowPlpos][i],obj);
                                chosenloc[0]=objloc[0];
                                chosenloc[1]=objloc[1];
                            }
                        }
                    }
                }
            }
        }
        break;
    case OPCODE::END_TURN_OP://胜利反馈
    {
        iswin=true;
        QWidget* win=new QWidget(this);
        QLabel* wlb=new QLabel("Congratulations!",win);
        win->setFixedSize(200,100);
        wlb->setFont(QFont("Microsoft YaHei",20,75));
        wlb->setGeometry(45,25,200,50);
        win->show();
    }
    break;
    case OPCODE::END_GAME_OP://游戏结束
    {
           if(data.data1.isEmpty()){
               QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
                break;
           }
           //弹排名界面
           QStringList pls = data.data1.split(" ");
           if(pls.at(pls.length()-1)==""){
               pls.removeLast();
           }
           rank->ranktable->setRowCount(pls.length());
           rank->ranktable->setHorizontalHeaderLabels(QStringList("玩家ID"));
           QLabel* conlb=new QLabel(rank);
           conlb->setGeometry(220,400,150,50);
           conlb->setFont(QFont("Microsoft YaHei",20));
           conlb->setStyleSheet("color:brown;");
           QStringList header;
           for(int i=0;i<pls.length();i++){
               if(pls[i]==PlName){
                   conlb->setText("Your rank is "+QString((char)(i+'1')));
               }
               if(i==0)
                   header << "1st";
               else if(i==1)
                   header << "2nd";
               else if(i==2)
                   header << "3rd";
               else{
                   header << QString("%1th").arg(i+1);
               }
           }        rank->ranktable->setVerticalHeaderLabels(header);
           for(int i=0;i<pls.length();i++){
                   rank->ranktable->setItem(i,0,new QTableWidgetItem(pls[i]));
           }
           QPushButton* con=new QPushButton(rank);
           con->setGeometry(225,450,100,40);
           con->setText("Confirm");
           connect(con,&QPushButton::clicked,this,[=](){
               for(int i=0;i<6;i++)
               {
                   ww.ids[i]->setText("");
                   ww.sis[i]->setText("Waiting");
               }
               players.clear();
               playerState.clear();
               cc.show();
               this->close();
               ww.hide();
               this->nowplayer->setText("");
               this->PlayerTable.clear();
               rank->hide();
           });
           rank->show();
    }
    break;
        case OPCODE::ERROR_OP://错误
        {
            if(data.data1.toInt()==400003)
                QMessageBox::information(this,QString("error"),QString("用户名已存在"),"OK");
            else if(data.data1.toInt()==400005){
                //把棋子移回去
                if(checked!=NULL){
                    CheckerMove(checked,loc[btnx][btny]);
                    isfill[btnx][btny] = 1;
                    isfill[objloc[0]][objloc[1]]=0;
                }
                QMessageBox::information(this,QString("error"),QString("移动不合法"),"OK");}
            else if(data.data1.toInt()==400006)
                QMessageBox::information(this,QString("error"),QString("无法解析该请求"),"OK");
            else if(data.data1.toInt()==400000)
                QMessageBox::information(this,QString("error"),QString("您不在该房间内"),"OK");
            else if(data.data1.toInt()==400004){
                //把棋子移回去
                if(checked!=NULL){
                    CheckerMove(checked,loc[btnx][btny]);
                    isfill[btnx][btny] = 1;
                    isfill[objloc[0]][objloc[1]]=0;
                }
                QMessageBox::information(this,QString("error"),QString("现在不是您的回合"),"OK");
                }
            else if(data.data1.toInt()==400001)
                QMessageBox::information(this,QString("error"),QString("该房间正在游戏中"),"OK");
            else if(data.data1.toInt()==400002){
                //把棋子移回去
                if(checked!=NULL)
                {
                    CheckerMove(checked,loc[btnx][btny]);
                    isfill[btnx][btny] = 1;
                    isfill[objloc[0]][objloc[1]]=0;
                }
                QMessageBox::information(this,QString("error"),QString("房间内无游戏进行"),"OK");}
            else{
                if(data.data2.isEmpty()){
                    QMessageBox::information(this,QString("error"),QString("未知错误"),"OK");
                }
                else{
                    QMessageBox::information(this,QString("error"),data.data2,"OK");
                }
            }
        }
        default:
        QMessageBox::information(this,QString("error"),QString("错误请求:\n%1").arg(data.encode()),"OK");
        break;
    }
}

void ClientWindow::timerEvent(QTimerEvent *event){
    timeLeft--;
    if(timeLeft<0){
        timeLeft = 0;
        //this->killTimer(id);//停止计时
//        for(int i=0;i<10;i++){
//            btn[place2num(myPos)][i]->close();
//        }
    }
    else{
       clock2->setText(QString("%1 s").arg(timeLeft));
    }
}
int ClientWindow::place2num(char pln){
    int k=0;
    if(playernum==2){
        if(pln=='A') k=0;
        else if(pln=='D') k=1;
    }
    else if(playernum==3){
        if(pln=='A') k=0;
        else if(pln=='C') k=1;
        else if(pln=='E') k=2;
    }
    else if(playernum==6){
        k=pln-'A';
    }
    return k;
}

void ClientWindow::initializeChecker(QString data){
    //初始化棋子 2player
    //wzr：我觉得不用根据data改棋子位置，把玩家和区域对应起来应该就行
    qDebug() << "enter initialize";
    playernum = players.length();
    int k=0;
    flag=pink;
    nowplayer = new QLabel(this);
    nowplayer->setFont(QFont("Agency FB",24));
    nowplayer->setGeometry(275,0,300,50);
    //nowplayer->setText("Player: PINK");
    nowplayer->setStyleSheet("color:#DB7093;");
    if(playernum==2){
    for(int j=5;j<=8;j++){
        for(int i=j-4;i<=4;i++){
            btn[pink][k]=new CheckerButton(this);
           // qDebug() << "debug: btn[" << pink << "][" << k << "] is constructed";
            btn[pink][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
            btn[pink][k]->setIcon(QPixmap(":/image/pink.png"));
            btn[pink][k]->setIconSize(QSize(RRR,RRR));
            btn[pink][k]->setFlat(true);
            btn[pink][k]->player=pink;//set player
            btn[pink][k]->x=-i+8;
            btn[pink][k]->y=j+8;
            isfill[-i+8][j+8]=pink+1;
            btn[green][k]=new CheckerButton(this);
           // qDebug() << "debug: btn[" << green << "][" << k << "] is constructed";
            btn[green][k]->setGeometry(loc[i+8][-j+8].rx()-RR/2,loc[i+8][-j+8].ry()-RR/2,RR,RR);
            btn[green][k]->setIcon(QPixmap(":/image/green.png"));
            btn[green][k]->setIconSize(QSize(RRR,RRR));
            btn[green][k]->setFlat(true);
            btn[green][k]->player=green; //set player
            btn[green][k]->x=i+8;
            btn[green][k]->y=-j+8;
            isfill[i+8][-j+8]=green+1;
            k++;
        }
    }
}
    if(playernum==3){
        k=0;
        for(int j=5;j<=8;j++){
            for(int i=j-4;i<=4;i++){
                btn[pink][k]=new CheckerButton(this);
                btn[pink][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
                btn[pink][k]->setIcon(QPixmap(":/image/pink.png"));
                btn[pink][k]->setIconSize(QSize(RRR,RRR));
                btn[pink][k]->setFlat(true);
                btn[pink][k]->player=pink;//set player
                btn[pink][k]->x=-i+8;
                btn[pink][k]->y=j+8;
                isfill[-i+8][j+8]=pink+1;
                k++;
            }
        }
        k=0;
        for(int i=1; i<5; i++){
            for(int j=-4; j<i-4; j++){
                btn[blue][k]=new CheckerButton(this);
                btn[blue][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
                btn[blue][k]->setIcon(QPixmap(":/image/blue.png"));
                btn[blue][k]->setIconSize(QSize(RR,RR));
                btn[blue][k]->setFlat(true);
                btn[blue][k]->player=blue; //set player
                btn[blue][k]->x=-i+8;
                btn[blue][k]->y=j+8;
                isfill[-i+8][j+8]=blue+1;
                k++;
            }
        }

        k=0;
        for(int i=5;i<=8;i++){
            for(int j=i-4;j<=4;j++){
                btn[green][k]=new CheckerButton(this);
                btn[green][k]->setGeometry(loc[i+8][-j+8].rx()-RR/2,loc[i+8][-j+8].ry()-RR/2,RR,RR);
                btn[green][k]->setIcon(QPixmap(":/image/green.png"));
                btn[green][k]->setIconSize(QSize(RRR,RRR));
                btn[green][k]->setFlat(true);
                btn[green][k]->player=green; //set player
                btn[green][k]->x=i+8;
                btn[green][k]->y=-j+8;
                isfill[i+8][-j+8]=green+1;
                k++;
            }
        }
    }
    if(playernum==6){
        k=0;
        for(int j=5;j<=8;j++){
            for(int i=j-4;i<=4;i++){
                btn[red][k]=new CheckerButton(this);
                btn[red][k]->setGeometry(loc[i+8][-j+8].rx()-RR/2,loc[i+8][-j+8].ry()-RR/2,RR,RR);
                btn[red][k]->setIcon(QPixmap(":/image/red.png"));
                btn[red][k]->setIconSize(QSize(RR,RR));
                btn[red][k]->setFlat(true);
                btn[red][k]->player=red; //set player
                btn[red][k]->x=i+8;
                btn[red][k]->y=-j+8;
                isfill[i+8][-j+8]=red+1;
                btn[pink][k]=new CheckerButton(this);
                btn[pink][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
                btn[pink][k]->setIcon(QPixmap(":/image/pink.png"));
                btn[pink][k]->setIconSize(QSize(RRR,RRR));
                btn[pink][k]->setFlat(true);
                btn[pink][k]->player=pink;//set player
                btn[pink][k]->x=-i+8;
                btn[pink][k]->y=j+8;
                isfill[-i+8][j+8]=pink+1;
                k++;
            }
        }
        k=0;
        for(int i=1; i<5; i++){
            for(int j=-4; j<i-4; j++){
                btn[green][k]=new CheckerButton(this);
                btn[green][k]->setGeometry(loc[i+8][-j+8].rx()-RR/2,loc[i+8][-j+8].ry()-RR/2,RR,RR);
                btn[green][k]->setIcon(QPixmap(":/image/green.png"));
                btn[green][k]->setIconSize(QSize(RRR,RRR));
                btn[green][k]->setFlat(true);
                btn[green][k]->player=green; //set player
                btn[green][k]->x=i+8;
                btn[green][k]->y=-j+8;
                isfill[i+8][-j+8]=green+1;
                btn[orange][k]=new CheckerButton(this);
                btn[orange][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
                btn[orange][k]->setIcon(QPixmap(":/image/orange.png"));
                btn[orange][k]->setIconSize(QSize(RRR,RRR));
                btn[orange][k]->setFlat(true);
                btn[orange][k]->player=orange; //set player
                btn[orange][k]->x=-i+8;
                btn[orange][k]->y=j+8;
                isfill[-i+8][j+8]=orange+1;
                k++;
            }
        }
        k=0;
        for(int i=5;i<=8;i++){
            for(int j=i-4;j<=4;j++){
                btn[purple][k]=new CheckerButton(this);
                btn[purple][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
                btn[purple][k]->setIcon(QPixmap(":/image/purple.png"));
                btn[purple][k]->setIconSize(QSize(RRR,RRR));
                btn[purple][k]->setFlat(true);
                btn[purple][k]->player=purple; //set player
                btn[purple][k]->x=-i+8;
                btn[purple][k]->y=j+8;
                isfill[-i+8][j+8]=purple+1;
                btn[blue][k]=new CheckerButton(this);
                btn[blue][k]->setGeometry(loc[i+8][-j+8].rx()-RR/2,loc[i+8][-j+8].ry()-RR/2,RR,RR);
                btn[blue][k]->setIcon(QPixmap(":/image/blue.png"));
                btn[blue][k]->setIconSize(QSize(RR,RR));
                btn[blue][k]->setFlat(true);
                btn[blue][k]->player=blue; //set player
                btn[blue][k]->x=i+8;
                btn[blue][k]->y=-j+8;
                isfill[i+8][-j+8]=blue+1;
                k++;
            }
        }
    }

   for(int i=0;i<10;i++){
        for(int j=0;j<playernum;j++){
            btn[j][i]->setCheckable(false);
        }
    }
    for(int i=0;i<playernum;i++){
        isover[i]=false;
    }
    //建立连接：按下棋子后记录被选中者
    for(int t=0;t<10;t++){
        for(int j=0;j<playernum;j++){
            connect(btn[j][t],&CheckerButton::is_chosen,this,[=](CheckerButton& but){
                if(flag==but.player&&step==0){
                   chosen.setX(but.pos().rx());
                   chosen.setY(but.pos().ry());
                   chosenloc[0]=but.x;
                   chosenloc[1]=but.y;
                   btnx=but.x;
                   btny=but.y;
                   ischosen=true;
                   checked=&but;
                 //  qDebug()<<but.x<<' '<<but.y;
                 //  qDebug() << "choose a check";
               }
               else{
                   //返回警告
               }
            });
        }
    }

    aiflag=false;
    isaimove=false;
    dep=new QPushButton(this);
    dep->setText("deposit");
    dep->setGeometry(600,100,80,40);
    connect(dep,&QPushButton::clicked,[&](){
        if(!aiflag){
            dep->setText("cancel");
            aiflag=true;
            if(flag==place2num(myPos)){
                for(int i=0;i<10;i++){
                    btn[flag][i]->setCheckable(false);
                }
                if(!isaimove){
                    ai();
                    isaimove=true;
                }
            }
        }
        else{
            dep->setText("deposit");
            aiflag=false;
            for(int i=0;i<10;i++){
                btn[flag][i]->setCheckable(true);
            }
        }
    });

    memset(fillnum,0,sizeof(fillnum));

    for(int i=0;i<playernum;i++){
        for(int j=0;j<20;j++){
            pointpath[i][j]=new QLabel(this);
        }
    }

}

void ClientWindow::setPlayerTable(){
    QLabel* Title = new QLabel(this);
    Title->setFont(QFont("Agency FB",20));
    Title->setGeometry(15,5,200,50);
    Title->setText("Players:");
    Title->setStyleSheet("color:black;");
    PlayerTable.append(Title);

    int plnum = players.length();
    int cir=20;
    for(int i=0;i<plnum;i++){
        QLabel* pl = new QLabel(this);
        pl->setFont(QFont("Agency FB",15));
        pl->setGeometry(15,10+cir,300,50);
        pl->setText(players[i]);
        switch(i){
        case 0:
            pl->setStyleSheet("color:#DB7093;");
            break;
        case 1:
            pl->setStyleSheet("color:green;");
            break;
        case 2:
            pl->setStyleSheet("color:blue;");
            break;
        case 3:
            pl->setStyleSheet("color:red;");
            break;
        case 4:
            pl->setStyleSheet("color:#FF4500;");
            break;
        case 5:
            pl->setStyleSheet("coloc:#800080");
            break;
        }
        PlayerTable.append(pl);
        cir += 20;
    }
}

void ClientWindow::displayError(){
    QMessageBox::information(this,QString("error"),QString("连接失败"),"OK");
    return;
}

void ClientWindow::connected(){
    QMessageBox::information(this,QString(""),QString("连接成功"),"OK");
    return;
}

int flatm[6][2]={{1,0},{0,1},{-1,0},{0,-1},{1,-1},{-1,1}};
int jumpm[6][2]={{2,0},{0,2},{-2,0},{0,-2},{2,-2},{-2,2}};

int qzbA[17][17]={/*0*/{0,0,0,0,0,0,0,0,0,0,0,0,-2,0,0,0,0},/*1*/{0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0},/*2*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*3*/{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
                  /*4*/{0,0,0,0,0,1,3,4,3,3,3,2,3,3,2,1,0},/*5*/{0,0,0,0,1,3,5,5,4,4,5,5,4,3,2,1,0},/*6*/{0,0,0,0,3,6,7,7,6,6,6,5,4,3,2,0,0},/*7*/{0,0,0,0,7,9,9,8,7,7,6,5,4,3,0,0,0},
                  /*8*/{0,0,0,0,11,11,10,9,8,7,6,5,3,0,0,0,0},/*9*/{0,0,0,13,12,11,10,9,7,6,5,2,1,0,0,0,0},/*10*/{0,0,14,13,12,11,10,8,6,4,3,1,0,0,0,0,0},/*11*/{0,15,14,13,12,11,9,7,4,3,1,0,-1,0,0,0,0},
                  /*12*/{16,15,14,13,11,9,7,5,3,1,0,-1,-2,0,0,0,0},/*13*/{0,0,0,0,7,6,5,4,0,0,0,0,0,0,0,0,0},/*14*/{0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0},/*15*/{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},/*16*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
int qzbB[17][17]={/*0*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*1*/{0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},/*2*/{0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},/*3*/{0,0,0,0,0,0,0,0,0,7,6,5,4,0,0,0,0},
                  /*4*/{0,0,0,0,16,15,14,13,11,9,7,5,3,1,0,-1,-2},/*5*/{0,0,0,0,15,14,13,12,11,9,7,4,3,1,0,-1,0},/*6*/{0,0,0,0,14,13,12,11,10,8,6,4,3,1,0,0,0},/*7*/{0,0,0,0,13,12,11,10,9,7,6,5,2,1,0,0,0},/*8*/{0,0,0,0,11,11,10,9,8,7,6,5,3,0,0,0,0},
                   /*9*/{0,0,0,7,9,9,8,7,7,6,5,4,3,0,0,0,0},/*10*/{0,0,3,6,7,7,6,6,6,5,4,3,2,0,0,0,0},/*11*/{0,1,3,5,5,4,4,5,5,4,3,2,1,0,0,0,0},/*12*/{0,1,3,4,3,3,3,2,3,3,2,1,0,0,0,0,0},
                   /*13*/{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},/*14*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*15*/{0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0},/*16*/{0,0,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,0}};
int qzbC[17][17]={/*0*/{0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0},/*1*/{0,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0},/*2*/{0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0},/*3*/{0,0,0,0,0,0,0,0,0,13,13,13,13,0,0,0,0},
                  /*4*/{0,0,0,0,0,1,3,7,11,12,12,12,11,7,3,1,0},/*5*/{0,0,0,0,1,3,6,9,11,11,11,11,9,6,3,1,0},/*6*/{0,0,0,0,3,5,7,9,10,10,10,9,7,5,3,0,0},/*7*/{0,0,0,0,4,5,7,8,9,9,8,7,5,4,0,0,0},
                  /*8*/{0,0,0,0,3,4,6,7,8,7,6,4,3,0,0,0,0},/*9*/{0,0,0,1,3,4,6,7,7,6,4,3,1,0,0,0,0},/*10*/{0,0,0,1,3,5,6,6,6,5,3,1,0,0,0,0,0},/*11*/{0,-1,0,1,2,5,5,5,5,2,1,0,-1,0,0,0,0},/*12*/{-2,-1,0,1,3,4,4,4,3,1,0,-1,-2,0,0,0,0},
                  /*13*/{0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,0,0},/*14*/{0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0},/*15*/{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},/*16*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
int qzbD[17][17]={/*0*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*1*/{0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},/*2*/{0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0},/*3*/{0,0,0,0,0,0,0,0,0,4,5,6,7,0,0,0,0},
                  /*4*/{0,0,0,0,-2,-1,0,1,3,5,7,9,11,13,14,15,16},/*5*/{0,0,0,0,-1,0,1,3,4,7,9,11,12,13,14,15,0},/*6*/{0,0,0,0,0,1,3,4,6,8,10,11,12,13,14,0,0},/*7*/{0,0,0,0,1,2,5,6,7,9,10,11,12,13,0,0,0},/*8*/{0,0,0,0,3,5,6,7,8,9,10,11,11,0,0,0,0},
                  /*9*/{0,0,0,3,4,5,6,7,7,8,9,9,7,0,0,0,0},/*10*/{0,0,2,3,4,5,6,6,6,7,7,6,3,0,0,0,0},/*11*/{0,1,2,3,4,5,5,4,4,5,5,3,1,0,0,0,0},/*12*/{0,1,2,3,3,2,3,3,3,4,3,1,0,0,0,0,0},
                  /*13*/{0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},/*14*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*15*/{0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0},/*16*/{0,0,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,0}};
int qzbE[17][17]={/*0*/{0,0,0,0,0,0,0,0,0,0,0,0,-2,0,0,0,0},/*1*/{0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0},/*2*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*3*/{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
                  /*4*/{0,0,0,0,0,1,2,3,3,2,3,3,3,4,3,1,0},/*5*/{0,0,0,0,1,2,3,4,5,5,4,4,5,5,3,1,0},/*6*/{0,0,0,0,2,3,4,5,6,6,6,7,7,6,3,0,0},/*7*/{0,0,0,0,3,4,5,6,7,7,8,9,9,7,0,0,0},/*8*/{0,0,0,0,3,5,6,7,8,9,10,11,11,0,0,0,0},
                  /*9*/{0,0,0,1,2,5,6,7,9,10,11,12,13,0,0,0,0},/*10*/{0,0,0,1,3,4,6,8,10,11,12,13,14,0,0,0,0},/*11*/{0,-1,0,1,3,4,7,9,11,12,13,14,15,0,0,0,0},/*12*/{-2,-1,0,1,3,5,7,9,11,13,14,15,16,0,0,0,0},
                  /*13*/{0,0,0,0,4,5,6,7,0,0,0,0,0,0,0,0,0},/*14*/{0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0},/*15*/{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},/*16*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
int qzbF[17][17]={/*0*/{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},/*1*/{0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},/*2*/{0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0},/*3*/{0,0,0,0,0,0,0,0,0,3,3,3,3,0,0,0,0},
                  /*4*/{0,0,0,0,-2,-1,0,1,3,4,4,4,3,1,0,-1,-2},/*5*/{0,0,0,0,-1,0,1,2,5,5,5,5,2,1,0,-1,0},/*6*/{0,0,0,0,0,1,3,5,6,6,6,5,3,1,0,0,0},/*7*/{0,0,0,0,1,3,4,6,7,7,6,4,3,1,0,0,0},/*8*/{0,0,0,0,3,4,6,7,8,7,6,4,3,0,0,0,0},
                  /*9*/{0,0,0,4,5,7,8,9,9,8,7,5,4,0,0,0,0},/*10*/{0,0,3,5,7,9,10,10,10,9,7,5,3,0,0,0,0},/*11*/{0,1,3,6,9,11,11,11,11,9,6,3,1,0,0,0,0},/*12*/{0,1,3,7,11,12,12,12,11,7,3,1,0,0,0,0,0},
                  /*13*/{0,0,0,0,13,13,13,13,0,0,0,0,0,0,0,0,0},/*14*/{0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0},/*15*/{0,0,0,0,15,15,0,0,0,0,0,0,0,0,0,0,0},/*16*/{0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0}};

void ClientWindow::ai(){
    int t=place2num(myPos);
    val1=0;
    val2=0;
    val3=0;
    bool ispass[17][17];
    for(int i=0;i<10;i++){
        way.clear();
        memset(ispass,false,sizeof(ispass));
        qDebug()<<"button:"<<t<<' '<<i;
        dfs(0,btn[t][i]->x,btn[t][i]->y,btn[t][i]->x,btn[t][i]->y);
        qDebug()<<"aipath now:"<<aipath;
    }
    for(int i=0;i<10;i++){
        if(btn[t][i]->x==aipath.at(0)&&btn[t][i]->y==aipath.at(1)){
            checked=btn[t][i];
            break;
        }
    }
    chosenloc[0]=aipath.at(0);
    chosenloc[1]=aipath.at(1);
    for(int i=0;i<aipath.length();i++){
        if(i>1) objloc[i%2]=aipath.at(i);
        if(i%2&&i>1) {
            qDebug()<<"objloc:"<<objloc[0]<<" "<<objloc[1];
            obj.setX(loc[objloc[0]][objloc[1]].rx()-RR/4);
            obj.setY(loc[objloc[0]][objloc[1]].ry()-RR/4);
            CheckerMove(checked,obj);
            chosenloc[0]=objloc[0];
            chosenloc[1]=objloc[1];
        }
    }
    socket->send(NetworkData(OPCODE::MOVE_OP,QString(myPos),path));
   // changeplayer();
}

bool ClientWindow::isPlaceLegal(int x,int y){
    int a=x-8,b=y-8;
    if(-4<=a&&a<=4&&-4<=b&&b<=4) return true;
    if((5<=a&&a<=8&&-4<=b&&b<=4-a)||((5<=-a&&-a<=8&&-4<=-b&&-b<=4+a))) return true;
    if((5<=b&&b<=8&&-4<=a&&a<=4-b)||((5<=-b&&-b<=8&&-4<=-a&&-a<=4+b))) return true;
    return false;
}


void ClientWindow::dfs(int k,int x,int y,int bx,int by){
    qDebug()<<"k:"<<k<<" x:"<<x<<" y:"<<y;
    ispass[x][y]=true;
    int flagg=0;
    if(!k){
        way.append(x);
        way.append(y);
    }
    if(!k){
    for(int i=0;i<6;i++){
        int a=x+flatm[i][0],b=y+flatm[i][1];
        bool pl=isPlaceLegal(a,b);
       // qDebug()<<a<<' '<<b<<" ispl:"<<pl;
        if(pl){
            if(!isfill[a][b]){
                isfill[a][b]=flag+1;
                isfill[x][y]=0;
                int pv1=PossibleValue1(0,x,y,a,b,bx,by);
               // qDebug()<<"pv1:"<<pv1<<" pv2:"<<pv2;
                if(val1<pv1) {
                //    val2=pv2;||(val1==pv1&&val2<pv2)
                    val1=pv1;
                    //记录轨迹
                    if(flagg)
                    {
                        way.removeLast();
                        way.removeLast();

                    }
                    valueback3(x,y,a,b);
                    isfill[a][b]=0;
                    isfill[x][y]=flag+1;
                }
                isfill[a][b]=0;
                isfill[x][y]=flag+1;

            }
        }
    }
    for(int i=0;i<6;i++){
        int a=x+jumpm[i][0],b=y+jumpm[i][1];
        bool pl=isPlaceLegal(a,b);
        qDebug()<<a<<' '<<b<<" ispl:"<<pl;
        if(pl){
            qDebug()<<"isfill:"<<isfill[a][b]<<" ispass:"<<ispass[a][b];
            if(!isfill[a][b]&&!ispass[a][b]&&isfill[(a+x)/2][(b+y)/2]){
                isfill[a][b]=flag+1;
                isfill[x][y]=0;
                if(flagg==1)
                {
                    way.removeLast();
                    way.removeLast();
                }
                way.append(a);
                way.append(b);
                int vv1=PossibleValue1(0,x,y,a,b,bx,by);
               // int vv2=PossibleValue2(0,x,y,a,b,bx,by);
                qDebug()<<"value after jump: "<<vv1;
                if(vv1>val1) {
                 //   val2=vv2;||(vv1==val1&&vv2>val2)
                    val1=vv1;
                    aipath.clear();
                aipath=way;
                qDebug()<<"way now:"<<way;
                if(vv1==1000000) return;}
                dfs(k+1,a,b,bx,by);
                way.removeLast();
                way.removeLast();
                isfill[a][b]=0;
                isfill[x][y]=flag+1;
                ispass[a][b]=false;
                flagg=2;
            }
        }
    }
    ispass[x][y]=false;
    return;
}

int valueA[17][17]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,150,-1,-1,-1,-1},
                     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,145,145,-1,-1,-1,-1},
                      {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,140,135,140,-1,-1,-1,-1},
                       {-1,-1,-1,-1,-1,-1,-1,-1,-1,130,125,125,130,-1,-1,-1,-1},
                         {-1,-1,-1,-1,0,10,30,70,110,115,120,115,110,70,30,10,0},
                          {-1,-1,-1,-1,15,25,40,80,105,110,110,105,80,40,25,15,-1},
                            {-1,-1,-1,-1,20,30,60,90,95,100,95,90,60,30,20,-1,-1},
                              {-1,-1,-1,-1,25,50,80,85,90,90,85,80,50,25,-1,-1,-1},
                                {-1,-1,-1,-1,30,65,70,75,80,75,70,65,30,-1,-1,-1,-1},
                                  {-1,-1,-1,15,35,60,65,70,70,65,60,35,15,-1,-1,-1,-1},
                                    {-1,-1,10,20,45,50,55,60,55,50,45,20,10,-1,-1,-1,-1},
                                       {-1,5,15,20,40,45,50,50,45,40,20,15,5,-1,-1,-1,-1},
                                         {0,10,15,20,30,35,40,35,30,20,15,10,0,-1,-1,-1,-1},
                                          {-1,-1,-1,-1,20,25,25,20,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                                           {-1,-1,-1,-1,10,15,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                                             {-1,-1,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                                              {-1,-1,-1,-1,-5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

int valueD[17][17]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-5,-1,-1,-1,-1},
                     {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1},
                      {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,15,10,-1,-1,-1,-1},
                       {-1,-1,-1,-1,-1,-1,-1,-1,-1,20,25,25,20,-1,-1,-1,-1},
                            {-1,-1,-1,-1,0,10,15,20,25,30,35,30,25,20,15,5,0},
                             {-1,-1,-1,-1,5,15,25,30,35,40,40,35,30,25,15,5,-1},
                               {-1,-1,-1,-1,10,25,35,40,45,50,45,40,35,25,10,-1,-1},
                                {-1,-1,-1,-1,15,35,45,50,55,55,50,45,35,15,-1,-1,-1},
                                 {-1,-1,-1,-1,30,50,55,60,65,60,55,50,30,-1,-1,-1,-1},
                                   {-1,-1,-1,25,35,55,65,70,70,65,55,35,25,-1,-1,-1,-1},
                                    {-1,-1,20,30,50,70,75,80,75,70,50,30,20,-1,-1,-1,-1},
                                       {-1,15,25,40,80,85,90,90,85,80,40,25,15,-1,-1,-1,-1},
                                         {0,10,30,70,90,95,100,95,90,70,30,10,0-1,-1,-1,-1},
                                          {-1,-1,-1,-1,110,105,105,110,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                                           {-1,-1,-1,-1,115,110,115,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                                            {-1,-1,-1,-1,120,120,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                                            {-1,-1,-1,-1,125,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

//估值   待实现
//x,y跳前位置，a,b跳后位置，bx,by原始位置
int ClientWindow::PossibleValue1(int t,int x,int y,int a,int b,int bx,int by){

    oa=a;ob=b;obx=bx;oby=by;

    int msv=morestepvalue(t);

    int lpv=lonelypointvalue(myPos);

    if(myPos=='A')

    return valueA[b][a]-valueA[by][bx]+100+msv-lpv*5;
    else if(myPos=='B')
        return qzbB[b][a]*5-qzbB[by][bx]*5+100+msv-lpv*5;
    else if(myPos=='C')
        return qzbC[b][a]*5-qzbC[by][bx]*5+100+msv-lpv*5;
    else if(myPos=='D')
        return valueD[b][a]-valueD[by][bx]+100+msv-lpv*5;
    else if(myPos=='E')
        return qzbE[b][a]*5-qzbE[by][bx]*5+100+msv-lpv*5;
    else if(myPos=='F')
        return qzbF[b][a]*5-qzbF[by][bx]*5+100+msv-lpv*5;

}



float ClientWindow::longdistancefirst(int bx,int by,int pos){
    float d=0;
    switch(pos){
    case'A':
        d=distance(loc[bx][by],loc[12][0]);
        break;
    case'C':
        d=distance(loc[bx][by],loc[0][12]);
        break;
    case'D':
        d=distance(loc[bx][by],loc[4][16]);
        break;
    case'E':
        d=distance(loc[bx][by],loc[12][12]);
        break;
    }
    return d;
}

int ClientWindow::stepvalue(int x,int y,int pos){
    int d=0;
    switch(pos){
    case'A':
        d=abs(x-12)+abs(y-0);
        break;
    case'C':
        d=abs(x-0)+abs(y-12);
        break;
    case'D':
        d=abs(x-4)+abs(y-16);
        break;
    case'E':
        d=abs(x-12)+abs(y-12);
        break;
    }
    return d;
}
float ddd;
//目标位置到终点平均位置距离估值
float ClientWindow::guessvalue1(int x,int y,int z)//这里传入的参数是目标要移动的位置的x,y坐标和该棋子的初始区域如‘A’，计算到目标区域的平均距离
 {
     ddd=0;
     int k=10;
     int tt=place2num(z);
     float x1=0,y1=0;
     if(z=='A')
     {
         for(int i=1;i<=4;i++)
         {
             for(int j=-5;j>=-i-4;j--)
             {
                 if(isfill[i+8][j+8]==tt+1){
                     k--;
                     continue;
                 }
                 x1=(loc[x][y].rx()-loc[i+8][j+8].rx())*(loc[x][y].rx()-loc[i+8][j+8].rx());
                 y1=(loc[x][y].ry()-loc[i+8][j+8].ry())*(loc[x][y].ry()-loc[i+8][j+8].ry());
                 ddd+=sqrt(x1+y1);
             }
         }
     }
     else if(z=='B')
     {
         for(int j=-4;j<=-1;j++)
         {
             for(int i=-4;i<=-j-5;i++)
             {
                 if(isfill[i+8][j+8]==tt+1){
                     k--;
                     continue;
                 }
                 x1=(loc[x][y].rx()-loc[i+8][j+8].rx())*(loc[x][y].rx()-loc[i+8][j+8].rx());
                 y1=(loc[x][y].ry()-loc[i+8][j+8].ry())*(loc[x][y].ry()-loc[i+8][j+8].ry());
                 ddd+=sqrt(x1+y1);
             }
         }
     }
     else if(z=='C')
     {
         for(int i=-8;i<=-5;i++)
         {
             for(int j=4;j>=-i-4;j--)
             {
                 if(isfill[i+8][j+8]==tt+1){
                     k--;
                     continue;
                 }
                 x1=(loc[x][y].rx()-loc[i+8][j+8].rx())*(loc[x][y].rx()-loc[i+8][j+8].rx());
                 y1=(loc[x][y].ry()-loc[i+8][j+8].ry())*(loc[x][y].ry()-loc[i+8][j+8].ry());
                 ddd+=sqrt(x1+y1);
             }
         }
     }
     else if(z=='D')
     {
         for(int i=-4;i<=-1;i++)
         {
             for(int j=-i+4;j>=5;j--)
             {
                 if(isfill[i+8][j+8]==tt+1){
                     k--;
                     continue;
                 }
                 x1=(loc[x][y].rx()-loc[i+8][j+8].rx())*(loc[x][y].rx()-loc[i+8][j+8].rx());
                 y1=(loc[x][y].ry()-loc[i+8][j+8].ry())*(loc[x][y].ry()-loc[i+8][j+8].ry());
                 ddd+=sqrt(x1+y1);
             }
         }
     }
     else if(z=='E')
     {
         for(int j=1;j<=4;j++)
         {
             for(int i=4;i>=-j+5;i--)
             {
                 if(isfill[i+8][j+8]==tt+1){
                     k--;
                     continue;
                 }
                 x1=(loc[x][y].rx()-loc[i+8][j+8].rx())*(loc[x][y].rx()-loc[i+8][j+8].rx());
                 y1=(loc[x][y].ry()-loc[i+8][j+8].ry())*(loc[x][y].ry()-loc[i+8][j+8].ry());
                 ddd+=sqrt(x1+y1);
             }
         }
     }
     else if(z=='F')
     {
         for(int i=5;i<=8;i++)
         {
             for(int j=-4;j<=-i+4;j++)
             {
                 if(isfill[i+8][j+8]==tt+1){
                     k--;
                     continue;
                 }
                 x1=(loc[x][y].rx()-loc[i+8][j+8].rx())*(loc[x][y].rx()-loc[i+8][j+8].rx());
                 y1=(loc[x][y].ry()-loc[i+8][j+8].ry())*(loc[x][y].ry()-loc[i+8][j+8].ry());
                 ddd+=sqrt(x1+y1);
             }
         }
     }
     return ddd/k;
 }

//目标位置和初始位置的距离估值
 float ClientWindow::guessvalue2(int x1,int y1,int x2,int y2)//这里传入的参数是目标位置和初始位置的x,y坐标，计算移动的距离大小,目前想的是，如果到中心区域一样，就选走到距离最长的
 {
     //return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
     return abs(x1-x2)+abs(y1-y2);
 }

 int ClientWindow::originvalue3(){
     int d=0;
     int t=place2num(myPos);
     switch(myPos){
     case'A':{
       for(int i=0;i<10;i++){
           d+=abs(btn[t][i]->x-12)+abs(btn[t][i]->y);
       }
       break;
     }
     case'B':{
       for(int i=0;i<10;i++){
           d+=abs(btn[t][i]->x-4)+abs(btn[t][i]->y-4);
       }
       break;
     }
     case'C':{
       for(int i=0;i<10;i++){
           d+=abs(btn[t][i]->x)+abs(btn[t][i]->y-12);
       }
       break;
     }
     case'D':{
       for(int i=0;i<10;i++){
           d+=abs(btn[t][i]->x-4)+abs(btn[t][i]->y-16);
       }
       break;
     }
     case'E':{
       for(int i=0;i<10;i++){
           d+=abs(btn[t][i]->x-12)+abs(btn[t][i]->y-12);
       }
       break;
     }
     case'F':{
       for(int i=0;i<10;i++){
           d+=abs(btn[t][i]->x-16)+abs(btn[t][i]->y-4);
       }
       break;
     }
     }
     qDebug()<<"guessvalue3: "<<d;
     return d;
 }

 //button到目标位置三角中心点坐标距离（移动步数）估值
/* int ClientWindow::guessvalue3(int x,int y,int a,int b){
     switch(myPos){
     case'A':{
         ov-=abs(x-12)+abs(y);
         ov+=abs(a-12)+abs(b);
         break;
     }
     case'B':{
         ov-=abs(x-4)+abs(y-4);
         ov+=abs(a-4)+abs(b-4);
         break;
     }
     case'C':{
         ov-=abs(x)+abs(y-12);
         ov+=abs(a)+abs(b-12);
         break;
     }
     case'D':{
         ov-=abs(x-4)+abs(y-16);
         ov+=abs(a-4)+abs(b-16);
         break;
     }
     case'E':{
         ov-=abs(x-12)+abs(y-12);
         ov+=abs(a-12)+abs(b-12);
         break;
     }
     case'F':{
         ov-=abs(x-16)+abs(y-4);
         ov+=abs(a-16)+abs(b-4);
         break;
     }
     }
     return ov;
 }

 void ClientWindow::valueback3(int x,int y,int a,int b){
     switch(myPos){
     case'A':{
         ov+=abs(x-11)+abs(y-2);
         ov-=abs(a-11)+abs(b-2);
         break;
     }
     case'B':{
         ov+=abs(x-5)+abs(y-5);
         ov-=abs(a-5)+abs(b-5);
         break;
     }
     case'C':{
         ov+=abs(x-2)+abs(y-11);
         ov-=abs(a-2)+abs(b-11);
         break;
     }
     case'D':{
         ov+=abs(x-5)+abs(y-14);
         ov-=abs(a-5)+abs(b-14);
         break;
     }
     case'E':{
         ov+=abs(x-11)+abs(y-11);
         ov-=abs(a-11)+abs(b-11);
         break;
     }
     case'F':{
         ov+=abs(x-14)+abs(y-5);
         ov-=abs(a-14)+abs(b-5);
         break;
     }
     }
     return;
 }
 */

 int ClientWindow::distance(QPointF p,QPointF q){
     return sqrt((p.rx()-q.rx())*(p.rx()-q.rx())+(p.ry()-q.ry())*(p.ry()-q.ry()));
 }


 bool ClientWindow::guessvalue4(int x,int y)//检查移动处是否可以跳跃
 {
     int direction[6][2]={{1,0},{-1,0},{0,1},{-1,1},{0,-1},{1,1}};
     for(int i=0;i<6;i++)
     {
         int x1=direction[i][0]+x,x2=direction[i][0]+x1;
         int y1=direction[i][1]+y,y2=direction[i][1]+y1;
         if(isfill[x1][y1]&&!isfill[x2][y2])
             return true;
     }
     return false;
 }


 //到达终点的估值
 int ClientWindow::fillvalue(int x){
     int flg=0;
         if(x=='A'){
             for(int j=5;j<=8;j++){
                 for(int i=j-4;i<=4;i++){
                     if(isfill[i+8][-j+8]==pink+1) flg++;
                 }
         }
     }
         else if(x=='D'){
             for(int j=5;j<=8;j++){
                 for(int i=j-4;i<=4;i++){
                     if(isfill[-i+8][j+8]==green+1) flg++;
                 }
             }
         }
         else if(x=='C'){
             for(int i=5;i<=8;i++){
                 for(int j=i-4;j<=4;j++){
                     if(isfill[-i+8][j+8]==green+1) flg++;
                 }
             }
         }
         else if(x=='E'){
             for(int i=1; i<5; i++){
                 for(int j=-4; j<i-4; j++){
                     if(isfill[i+8][-j+8]==blue+1) flg++;
                 }
             }
         }
         fillnum[place2num(myPos)]=flg;
         if(flg>=5) {
             flg+=fillvalueplus(x);
         }
     return flg;
}

 int ClientWindow::fillvalueplus(int x){
     int flg=0;
         if(x=='A'){
             for(int j=5;j<=8;j++){
                 for(int i=j-4;i<=4;i++){
                     if(isfill[i+8][-j+8]==pink+1){
                         if(j==8) flg+=10;
                         else if(j==7) flg+=5;
                         else if(j==6) flg+=3;
                         if(i==4||i-j==-4) flg+=2;
                     }
                 }
         }
     }
         else if(x=='D'){
             for(int j=5;j<=8;j++){
                 for(int i=j-4;i<=4;i++){
                     if(isfill[-i+8][j+8]==green+1){
                         if(j==8) flg+=10;
                         else if(j==7) flg+=5;
                         else if(j==6) flg+=3;
                         if(i==4||j-i==4) flg+=2;
                     }
                 }
             }
         }
         else if(x=='C'){
             for(int i=5;i<=8;i++){
                 for(int j=i-4;j<=4;j++){
                     if(isfill[-i+8][j+8]==green+1){
                         if(i==8) flg+=10;
                         else if(i==7) flg+=5;
                         else if(i==6) flg+=3;
                         if(j==4||i-j==4) flg+=2;
                     }
                 }
             }
         }
         else if(x=='E'){
             for(int i=1; i<5; i++){
                 for(int j=-4; j<i-4; j++){
                     if(isfill[i+8][-j+8]==blue+1){
                         if(i-j==8) flg+=10;
                         else if(i-j==7) flg+=5;
                         else if(i-j==6) flg+=3;
                         if(i==4||j==-4) flg+=2;
                     }
                 }
             }
         }
         return flg;
 }

 int ClientWindow::lonelypointvalue(int x){
     int t=place2num(x);
     int k=0;
     for(int i=0;i<17;i++){
         for(int j=0;j<17;j++){
             if(isfill[i][j]==t+1){
                 bool ff=false;
                 for(int p=0;p<6;p++){
                     if(isPlaceLegal(i+flatm[p][0],j+flatm[p][1])){
                         if(isfill[i+flatm[p][0]][j+flatm[p][1]]){
                             ff=true;
                             break;
                         }
                     }
                     if(isPlaceLegal(i+jumpm[p][0],j+jumpm[p][1])){
                         if(isfill[i+jumpm[p][0]][j+jumpm[p][1]]){
                             ff=true;
                             break;
                         }
                     }
                 }
                 if(!ff) k++;
             }
         }
     }
     return k;
 }

 bool ClientWindow::islonely(int bx,int by){
     int k=0;
     int tt=place2num(myPos);
     for(int i=0;i<6;i++){
         if(isPlaceLegal(bx+flatm[i][0],by+flatm[i][1])){
             if(isfill[bx+flatm[i][0]][by+flatm[i][1]]==tt+1) {
                 k++;
             }
         }
         if(isPlaceLegal(bx+jumpm[i][0],by+jumpm[i][1])){
             if(isfill[bx+jumpm[i][0]][by+jumpm[i][1]]==tt+1) {
                 k++;
             }
         }
     }
     if(k<2) return true;
     return false;
 }

 int ClientWindow::morestepvalue(int t){
     if(t) return 0;
     int mpos=place2num(myPos);
     mval=0;
     memset(ispass2,false,sizeof(ispass2));
     for(int i=0;i<17;i++){
         for(int j=0;j<17;j++){
             if(isfill[i][j]==mpos-'A'+1){
                 dfsplus(t+1,0,i,j,i,j);
             }
         }
     }
     return mval;

 }
void ClientWindow::dfsplus(int t,int k,int x,int y,int bx,int by){
    if(bx==oa&&by==ob&&ispass[x][y]==true) return;
     qDebug()<<"k:"<<k<<" x:"<<x<<" y:"<<y;
     ispass2[x][y]=true;
     if(!k){
     for(int i=0;i<6;i++){
         int a=x+flatm[i][0],b=y+flatm[i][1];
         bool pl=isPlaceLegal(a,b);
        // qDebug()<<a<<' '<<b<<" ispl:"<<pl;
         if(pl){
             if(!isfill[a][b]){
                 isfill[a][b]=flag+1;
                 isfill[x][y]=0;
                 int pv=PossibleValue1(k,x,y,a,b,bx,by);
                 qDebug()<<"pv:"<<pv;
                 if(mval<pv) {
                     mval=pv;
                 }
                 isfill[a][b]=0;
                 isfill[x][y]=flag+1;
             }
         }
     }
     }
     for(int i=0;i<6;i++){
         int a=x+jumpm[i][0],b=y+jumpm[i][1];
         bool pl=isPlaceLegal(a,b);
      //   qDebug()<<a<<' '<<b<<" ispl:"<<pl;
         if(pl){
            // qDebug()<<"isfill:"<<isfill[a][b]<<" ispass:"<<ispass[a][b];
             if(!isfill[a][b]&&!ispass2[a][b]&&isfill[(a+x)/2][(b+y)/2]){
                 isfill[a][b]=flag+1;
                 isfill[x][y]=0;
                 int vv=PossibleValue1(k+1,x,y,a,b,bx,by);
                 qDebug()<<"value after jump: "<<vv;
                 if(vv>mval) {
                     mval=vv;
                 if(vv==1000000) return;}
                 dfsplus(t,k+1,a,b,bx,by);
                 isfill[a][b]=0;
                 isfill[x][y]=flag+1;
                 ispass2[a][b]=false;
             }
         }
     }
     ispass2[x][y]=false;
     return;
 }

