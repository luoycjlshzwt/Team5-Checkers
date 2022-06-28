#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<QPointF>
#include<QDebug>
#include<QLabel>
#include<QFont>
#include<QDateTime>
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

void Widget::cbuttonpress()
{
    z = new mydialog1(this) ;//将类指针实例化
    z->show();
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::Widget)
{
    ui->setupUi(this);
    //初始化等待界面和过渡界面
    ChooseServer = new chooseservice;
    ServerWait = new serverwait;
    //初始化server
    server = new NetworkServer(this);
    connect(server, &NetworkServer::receive, this, &Widget::receiveData);
//    connect(this->d.getJoin(),&QPushButton::clicked,this,[=](){
//       server->listen(QHostAddress::Any,d.getPort().toInt());
//       //test
//       qDebug() << "begin to listen";
//       //test end
//    });
    //初始化秒表
    clock1 = new QLabel("剩余时间",this);
    clock1->setFont(QFont("Microsoft YaHei",20));
    clock1->setGeometry(70,500,300,50);
    clock1->setStyleSheet("color:brown;");
    clock2 = new QLabel("15 s",this);
    clock2->setFont(QFont("Agency FB",20));
    clock2->setStyleSheet("color:brown;");
    clock2->setGeometry(70,530,300,50);

    //开始界面 设置玩家人数
   /* myDialog *d=new myDialog;
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
    //playernum=6;
    connect(ui->QUIT, SIGNAL(clicked(bool)), this, SLOT(cbuttonpress()));  //弹出退出窗口
    this->setWindowTitle("Server");

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
        //初始化isfill
        for(int i1=0;i1<17;i1++){
            for(int i2=0;i2<17;i2++){
                isfill[i1][i2]=0;
            }
        }
        /*win=new QDialog(this);
        win->setFixedSize(200,100);
        QLabel *w=new QLabel(win);
        w->setFont(QFont("Microsoft YaHei",20,75));
        w->setGeometry(45,25,200,50);
        win->hide();
       connect(this,SIGNAL(finish(int)),this,SLOT(someoneover(int)));*/
        //想做弹窗提示但好像不太成功

        //初始化是否需要更换棋手
        shouldSwitch=false;
        flag=pink;
        //记录棋手

        //记录上一步是否为跳子
        haveJumped=false;

        //初始化回合结束按钮
        end = new QPushButton(this);
        end->setText("回合结束");
        end->setGeometry(293,640,100,50);

        nowplayer = new QLabel(this);
        nowplayer->setFont(QFont("Agency FB",24));
        nowplayer->setGeometry(275,0,300,50);
        nowplayer->setText("Player: PINK");
        nowplayer->setStyleSheet("color:#DB7093;");
        //实现更换执棋方功能
        /*connect(end,&QPushButton::clicked,this,[=](){
            if(ischange==false&&!(chosenloc[0]==btnx&&chosenloc[1]==btny)){//当没有换过且棋子不在初始位置时换player
                shouldSwitch=true;
                shouldSwitcht2f();
                shouldSwitch=false;
                ischange=true;
                qDebug() << "player changed";
            }
            else if(chosenloc[0]==btnx&&chosenloc[1]==btny){
                nobai->show();
            }
        });*/

        //connect(this,SIGNAL(shouldSwitchChanged()),this,SLOT(changeplayer()));

    }

    void Widget::shouldSwitcht2f(){
        if(shouldSwitch==true){
            shouldSwitch=false;
            emit shouldSwitchChanged();
        }
    }

    void Widget::changeplayer(){

        do{
            flag = (flag+1)%playernum;
        }while(isover[flag]);
        switch(flag){
        case red:
            nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[red].getID()));
            nowplayer->setStyleSheet("color:red;");
            break;
        case blue:
            nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[blue].getID()));
            nowplayer->setStyleSheet("color:blue;");
            break;
        case green:
            nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[green].getID()));
            nowplayer->setStyleSheet("color:green;");
            break;
        case pink:
            nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[pink].getID()));
            nowplayer->setStyleSheet("color:#DB7093;");
            break;
        case purple:
            nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[purple].getID()));
            nowplayer->setStyleSheet("color:#800080;");
            break;
          case orange:
            nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[orange].getID()));
            nowplayer->setStyleSheet("color:#FF4500;");
            break;
        }
        haveJumped=false;
        ischosen=false;
        isobjset=false;
        checked=NULL;
        jumped=NULL;
        step=0;
    }


    Widget::~Widget()
    {
        delete ui;
    }
    void Widget::paintEvent(QPaintEvent *)
    {
        DrawCheckerboard();       //棋盘
        update();          //强制更新界面
    }


    void Widget::DrawCheckerboard(void)
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

    void Widget::mousePressEvent(QMouseEvent *ev){
        //录入obj 并进行棋子移动
        if(ischosen){
              //反映鼠标点击点坐标
    //        QString posi = QString("%1,%2").arg(ev->pos().rx()).arg(ev->pos().ry());
    //        test->setText(posi);

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
                int mv=islegal();
                if(mv&&isobjset){
                    CheckerMove(checked,obj);
                    isobjset=false;
                    if(mv==1){
                        shouldSwitcht2f();
                    }
                    else if(mv==2){
                        jumped=checked;
                        chosenloc[0]=objloc[0];
                        chosenloc[1]=objloc[1];
                    }
    //                if(shouldSwitch){
    //                   shouldSwitcht2f();
    //                }
    //                else{
    //                   chosenloc[0]=objloc[0];
    //                   chosenloc[1]=objloc[1];
    //                }
                }
            }
        }
    }

    int Widget::islegal(){
        //判断是否已经有选中棋子
        if(!ischosen){
            qDebug() << "no chosen checker";
            return 0;
        }
        //判断目标点是否为空位
        if(isfill[objloc[0]][objloc[1]]){
            qDebug() << "object is filled";
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
            //test->setText("flatmove made");
            return 1;
        }

        //判断连跳
        if(haveJumped&&jumped!=checked){
            qDebug() << "连跳换子";
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
        qDebug() << "undefined move";
        return 0;
    }


    int Widget::pixel2int(QPointF pixel){
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

    void Widget::CheckerMove(CheckerButton*btn,QPointF p){
        QPropertyAnimation *anim = new QPropertyAnimation(btn, "pos", this);
        anim->setDuration(300);
        QPointF obj;
        obj.setX(loc[btn->x][btn->y].rx()-R+3);
        obj.setY(loc[btn->x][btn->y].ry()-R+2);
        anim->setStartValue(obj);
        anim->setEndValue(QPointF(p.rx()-R+3,p.ry()-R+2.5));
        anim->start(QPropertyAnimation::KeepWhenStopped);
        btn->x=objloc[0];
        btn->y=objloc[1];
        isfill[objloc[0]][objloc[1]]=btn->player+1;
        isfill[chosenloc[0]][chosenloc[1]]=0;
        ischange=false;
        step++;
        totalstep++;
    }

    bool Widget::canJump(int x,int y){//不能跳回原位
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
    bool Widget::isfinish(int x){
        bool flg=true;
        if(playernum==2){
            if(x=='A'){
                for(int j=5;j<=8;j++){
                    for(int i=j-4;i<=4;i++){
                        if(!flg) break;
                        if(isfill[i+8][-j+8]!=pink+1) flg=false;
                    }
                    if(!flg) break;
            }
        }
            else if(x=='D'){
                for(int j=5;j<=8;j++){
                    for(int i=j-4;i<=4;i++){
                        if(!flg) break;
                        if(isfill[-i+8][j+8]!=green+1) flg=false;
                    }
                    if(!flg) break;
                }
            }
        }
        if(playernum==3){
            if(x=='A'){
                for(int j=5;j<=8;j++){
                    for(int i=j-4;i<=4;i++){
                        if(!flg) break;
                        if(isfill[i+8][-j+8]!=pink+1) flg=false;
                    }
                    if(!flg) break;
            }
            }
            else if(x=='C'){
                for(int i=5;i<=8;i++){
                    for(int j=i-4;j<=4;j++){
                        if(!flg) break;
                        if(isfill[-i+8][j+8]!=green+1) flg=false;
                    }
                    if(!flg) break;
                }
            }
            else if(x=='E'){
                for(int i=1; i<5; i++){
                    for(int j=-4; j<i-4; j++){
                        if(!flg)break;
                        if(isfill[i+8][-j+8]!=blue+1) flg=false;
                    }
                    if(!flg) break;
                }
            }
        }
        if(playernum==6){
        switch(x){
        case 'D':{
            for(int j=5;j<=8;j++){
                for(int i=j-4;i<=4;i++){
                    if(!flg) break;
                    if(isfill[-i+8][j+8]!=red+1) flg=false;
                }
                if(!flg) break;
            }
            break;
        }
        case 'C':{
            for(int i=5;i<=8;i++){
                for(int j=i-4;j<=4;j++){
                    if(!flg) break;
                    if(isfill[-i+8][j+8]!=blue+1) flg=false;
                }
                if(!flg) break;
            }
            break;
        }
        case 'B':{
            for(int i=1; i<5; i++){
                for(int j=-4; j<i-4; j++){
                    if(!flg)break;
                    if(isfill[-i+8][j+8]!=green+1) flg=false;
                }
                if(!flg)break;
            }
            break;
        }
        case 'A':{
            for(int j=5;j<=8;j++){
                for(int i=j-4;i<=4;i++){
                    if(!flg) break;
                    if(isfill[i+8][-j+8]!=pink+1) flg=false;
                }
                if(!flg) break;
            }
            break;
        }
        case 'F':{
            for(int i=5;i<=8;i++){
                for(int j=i-4;j<=4;j++){
                    if(!flg) break;
                    if(isfill[i+8][-j+8]!=purple+1) flg=false;
                }
                if(!flg) break;
            }
            break;
        }
        case 'E':{
            for(int i=1; i<5; i++){
                for(int j=-4; j<i-4; j++){
                    if(!flg)break;
                    if(isfill[i+8][-j+8]!=orange+1) flg=false;
                }
                if(!flg)break;
            }
            break;
        }}
        }
        if(flg){//未完成
            isover[place2num(x)]=true;
            int index =(place2num(x)+1)%playernum;
            this->PlayerTable[index]->setText(QString("%1 %2").arg(roomList[0].playerList[place2num(x)].getID()).arg(overnum++));
        }
        return flg;
}
CheckerButton* Widget::int2btn(int btnx,int btny){
    int flag=0;
    for(int i=0;i<playernum;i++){
        for(int j=0;j<10;j++){
            if(btn[i][j]->x==btnx&&btn[i][j]->y==btny){
                return btn[i][j];
                flag=1;
                break;
            }
        }
        if(flag) break;
    }
}
int Widget::place2num(char pln){
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
static int timeleft = 15;
void Widget::receiveData(QTcpSocket *client, NetworkData data){
    switch(data.op){
    case OPCODE::JOIN_ROOM_OP:{
        //请求合法性判定
        bool isValid=true;
        for(int i=0;i<data.data1.length();i++){
            if(data.data1.toStdString()[i]<'0'||data.data1.toStdString()[i]>'9'){
                isValid = false;
                break;
            }
        }
        if(data.data2.length()>20||data.data2.isEmpty())
            isValid=false;
        else{
            for(int i=0;i<data.data2.length();i++)
            {
                char ch = data.data2.toStdString()[i];
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
            server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::INVALID_REQ)),QString()));
            break;
        }
        //test
        qDebug() << "server receive JOIN_ROOM_OP";
        //test end
        bool newRoom=false;
        bool nameConflict=false;
        bool canEnter=false;
        int objRoom = -1;
        if(roomList.empty()){
            newRoom = true;
        }
        else{
            for(int i=0;i<roomList.length();i++){
                if(roomList[i].getID()==data.data1.toInt()){
                    newRoom = false;
                    for(int j=0;j<roomList.at(i).getPlnum();j++){
                        if(roomList[i].playerList[j].getID()==data.data2){
                           nameConflict = true;
                           canEnter=false;
                           break;
                        }
                    }
                    if(!nameConflict){
                        canEnter = true;
                        objRoom = i;
                    }
                    break;
                }
            }
            if(!(nameConflict||canEnter))
                newRoom=true;
        }
        if(newRoom)
        {
            Room newRoom(data.data1.toInt());
            newRoom.addPl(data.data2,client);
            roomList.append(newRoom);
            //发送开房信号
            server->send(client,NetworkData(OPCODE::JOIN_ROOM_REPLY_OP,QString(""),QString("")));
            //test
            qDebug() << "server send JOIN_ROOM_REPLY_OP";
            //test end
            ChooseServer->hide();//关闭等待页面
            ServerWait->setPlayerName(0,data.data2);//添加玩家
            ServerWait->setWindowTitle(QString("ROOM %1 Server").arg(data.data1));
            ServerWait->show();
        }
        if(nameConflict){
            server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::INVALID_JOIN)),QString("")));
            //test
            qDebug() << "server send INVALID_JOIN";
            //test end
        }
        if(canEnter&&objRoom>=0){
            if(roomList[objRoom].ifON()){
                //如果目标房间游戏开始，则不允许加入
                server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::ROOM_IS_RUNNING)),QString("")));
            //test
            qDebug() << "server send ROOM_IS_RUNNING";
            //test end
            }
            else{
                roomList[objRoom].addPl(data.data2,client);
                //发送加入成功信号
                QString prevPl;
                QString prevState;
                for(int t=0;t<roomList[objRoom].getPlnum()-1;t++){
                    prevPl.append(roomList[objRoom].playerList[t].getID()).append(" ");
                    if(roomList[objRoom].playerList[t].ifReady())
                        prevState.append("1");
                    else prevState.append("0");
                    //qDebug()<<prevPl;
                    server->send(roomList[objRoom].playerList[t].getSocket(),NetworkData(OPCODE::JOIN_ROOM_OP,data.data2,QString("")));//向其他玩家发送新玩家信息
                    //test
                    qDebug() << "server send JOIN_ROOM_OP";
                    //test end
                    ChooseServer->hide();//关闭等待页面
                    ServerWait->setPlayerName(roomList[objRoom].getPlnum()-1,data.data2);//添加玩家
                    ServerWait->setWindowTitle(QString("ROOM %1 Server").arg(data.data1));
                    ServerWait->show();
                }
                server->send(client,NetworkData(OPCODE::JOIN_ROOM_REPLY_OP,prevPl,prevState));//向新加入玩家发送其他玩家信息

                //test
                qDebug() << "server send JOIN_ROOM_REPLY_OP";
                      qDebug() << "to new pl:previous player is " << prevPl;
                      qDebug() << "to new pl:previous player status is " << prevState;
                    //test end
                }
            }
        }
    break;
    case OPCODE::LEAVE_ROOM_OP:{
        //请求合法性判定
        bool isValid=true;
        if(data.data1.isEmpty()){
            break;
        }
        for(int i=0;i<data.data1.length();i++){
            if(data.data1.toStdString()[i]<'0'||data.data1.toStdString()[i]>'9'){
                isValid = false;
                break;
            }
        }

        if(data.data2.length()>20||data.data2.isEmpty())
            isValid=false;
        else{
            for(int i=0;i<data.data2.length();i++)
            {
                char ch = data.data2.toStdString()[i];
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
            server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::INVALID_REQ)),QString("")));
            break;
        }
        //test
        qDebug() << "server receive LEAVE_ROOM_OP";
        //test end
        int objRoom = data.data1.toInt();
        bool inRoom = false;
        bool roomFound = false;
        QString outPl = data.data2;
        for(int i=0;i<roomList.length();i++){
            if(roomList[i].getID()==objRoom){
                roomFound = true;
                int plnum=roomList[i].getPlnum();
                int plPos = -1;
                for(int j=0;j<plnum;j++){
                    if(roomList[i].playerList[j].getID()==outPl){
                        roomList[i].plLeave(j);
                        inRoom = true;
                        plPos = j;
                        break;
                    }
                }
                if(inRoom){
                    for(int t=0;t<roomList[i].getPlnum();t++){
                        server->send(roomList[i].playerList[t].getSocket(),NetworkData(OPCODE::LEAVE_ROOM_OP,data.data2,QString("")));
                        //test
                        qDebug() << "server send LEAVE_ROOM_OP";
                        //test end
                              ServerWait->setPlayerReady(plPos,false);//清空该玩家姓名
                              ServerWait->setPlayerName(plPos,"");
                            qDebug() << "plPos is " << plPos;
                        for(int m=plPos;m<roomList[i].getPlnum();m++){
                            ServerWait->setPlayerReady(m,roomList[i].playerList[m].ifReady());
                            qDebug() << m << " is set with " << roomList[i].playerList[m].getID();
                            ServerWait->setPlayerName(m,roomList[i].playerList[m].getID());
                        }
                        ServerWait->setPlayerName(roomList[i].getPlnum(),"");
                        ServerWait->setPlayerReady(roomList[i].getPlnum(),false);
                    }
                    break;
                }
                else
                    server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::NOT_IN_ROOM)),QString("")));
                    //test
                    qDebug() << "server send NOT_IN_ROOM";
                    //test end
            }
        }
        if(!roomFound){
            server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::OTHER_ERROR)),QString("RoomID Not Found")));
            //test
            qDebug() << "server send OTHER_ERROR:RoomID Not Found";
            //test end
        }
    }
    break;
    case OPCODE::MOVE_OP:{
        //test
        qDebug() << "server receive MOVE_OP";
        //test end
        qDebug()<<"receive success";
        qDebug() << "path is " << data.data2;
        if(place2num(data.data1.toUtf8()[0])!=flag){
            server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::OUTTURN_MOVE)),QString("")));
            break;
        }
        QStringList step = data.data2.split(" ");
        char pln=data.data1[0].toLatin1();
       int stepNum = step.length();
        //设置初始点
        btnx = step[0].toInt()+8;
        btny = step[1].toInt()+8;
        CheckerButton*b=int2btn(btnx,btny);
        bool belonged = false;
        for(int s=0;s<10;s++){
            if(btn[flag][s]==b){
                belonged = true;
                break;
            }
        }
        if(!belonged){
            server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::OTHER_ERROR)),QString("not your checker")));
            break;
        }
        chosenloc[0]=btnx;
        chosenloc[1]=btny;
        bool legalmove = true;
       for(int i=2;i<stepNum;i++){
           if(i%2==0){
               objloc[0]=step[i].toInt()+8;
           }
           else{
               objloc[1]=step[i].toInt()+8;
           }
           if(i%2){
               if(isfill[chosenloc[0]][chosenloc[1]]){
                   ischosen=true;
                   if(islegal()){
                        CheckerMove(b,loc[objloc[0]][objloc[1]]);
                        //test
                        qDebug() << "make a move from " << chosenloc[0]-8 << "," << chosenloc[1]-8 << " to " << objloc[0]-8 << "," << objloc[1]-8;
                        //test end
                        chosenloc[0]=objloc[0];
                        chosenloc[1]=objloc[1];
                   }
                   else{
                       legalmove = false;
                       //发送错误信号
                       server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::INVALID_MOVE)),QString("move is illegal")));
                       //test
                       qDebug() << "server send INVALID_MOVE";
                       //test end
                       //棋子移回去
                       CheckerMove(b,loc[btnx][btny]);
                       break;
                   }
               }
           }
        }
       if(legalmove){
           for(int i=0;i<playernum;i++){
               //转发move op
               server->send(roomList[0].playerList[i].getSocket(),data);
               //test
               qDebug() << "server send MOVE_OP to "<<roomList[0].playerList[i].getID();
               //test end
              }
           this->killTimer(id);
           timeleft = 15;
           clock2->setText("15 s");

           //if(totalstep>70*playernum){
               if(isfinish(pln)){
                   server->send(client,NetworkData(OPCODE::END_TURN_OP,QString(),QString()));
                   //test
                   qDebug() << "server send END_TURN_OP";
                   //test end
                   for(int i=0;i<playernum;i++){
                       if(pln==roomList[0].playerList[i].getPlace()){
                           ranklist.append(roomList[0].playerList[i].getID()).append(" ");//roomList设置为玩家姓名表
                           break;
                       }
                   }
           }//}
           if(overnum==playernum-1) {
                for(int i=0;i<playernum;i++){
                    if(!isover[i]){
                           overlist.append(roomList[0].playerList[i].getID());
                          break;
                    }
                }
                if(overlist.length()){
                   for(int g=0;g<overlist.length();g++)
                       ranklist.append(overlist[overlist.length()-g-1]).append(" ");
                }
                for(int i=0;i<playernum;i++){
                   server->send(roomList[0].playerList[i].getSocket(),NetworkData(OPCODE::END_GAME_OP,ranklist,QString(" ")));
                   //test
                   qDebug() << "server send END_GAME_OP";
                   //test end
                   }
                 ranklist = "";
                 roomList.clear();
                 overlist.clear();
                 overnum=0;
                 for(int i=0;i<6;i++){
                     ServerWait->setPlayerName(i,"");
                     ServerWait->setPlayerReady(i,false);
                 }
                 ChooseServer->show();
                 this->hide();
                }
           else{
           changeplayer();
           qint64 epoch = QDateTime::currentMSecsSinceEpoch();
           QString str =  QString("%1").arg(epoch);
           for(int i=0;i<roomList[0].getPlnum();i++){
                server->send(roomList[0].playerList[i].getSocket(),NetworkData(OPCODE::START_TURN_OP,QString(num2place(flag)),str));
           }
           //test
           qDebug() << "server send START_TURN_OP";
           //test end
           timeleft = 15;
           id = startTimer(1000);}
       }

    }
    break;
    case OPCODE::PLAYER_READY_OP:{
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
                server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::INVALID_REQ)),QString("")));
                break;
            }
            //test
            qDebug() << "server receive PLAYER_READY_OP";
            //test end
            int roomNum = roomList.length();
            bool found=false;
            int PlayerIndex;
            for(int i=0;i<roomNum;i++){
                for(int j=0;j<roomList[i].getPlnum();j++){
                    if(roomList[i].playerList[j].getID()==data.data1){
                        roomList[i].playerList[j].setReady();
                        found=true;
                        PlayerIndex=j;
                        break;
                    }
                }
                if(found){
                    roomList[i].addReady();
                    //转发某玩家就绪信息
                    for(int j=0;j<roomList[i].getPlnum();j++){
                        server->send(roomList[i].playerList[j].getSocket(),data);
                        //test
                        qDebug() << "server send PLAYER_READY_OP";
                        //test end
                    }
                    //更新页面信息
                    ServerWait->setPlayerReady(PlayerIndex,true);
                    //检查是否可以开始游戏
                    if((roomList[i].getPlnum()==roomList[i].getReadynum())&&(roomList[i].getReadynum()==2||roomList[i].getReadynum()==3||roomList[i].getReadynum()==6)){
                        QString plName;
                        QString seq;
                        if(roomList[i].getReadynum()==2){
                            seq = "A D";
                            plName = roomList[i].playerList[0].getID().append(" ").append(roomList[i].playerList[1].getID());
                            roomList[i].playerList[0].setPlace('A');
                            roomList[i].playerList[1].setPlace('D');
                        }
                        else if(roomList[i].getReadynum()==3){
                            seq = "A C E";
                            plName = roomList[i].playerList[0].getID();
                            roomList[i].playerList[0].setPlace('A');
                            for(int t=1;t<=2;t++){
                                plName.append(" ").append(roomList[i].playerList[t].getID());
                                QChar place(65+2*t);
                                roomList[i].playerList[t].setPlace(place.toLatin1());
                            }
                        }
                        else{
                            seq = "A B C D E F";
                            plName = roomList[i].playerList[0].getID();
                            roomList[i].playerList[0].setPlace('A');
                            for(int t=1;t<6;t++){
                                plName.append(" ").append(roomList[i].playerList[t].getID());
                                QChar place(65+t);
                                roomList[i].playerList[t].setPlace(place.toLatin1());
                            }
                        }
                        qDebug() << "game will start";
                        qDebug() << "players are " << plName;
                        qDebug() << "the sequence is " << seq;
                        roomList[i].gameBegin();//游戏开始
                        flag = 0;
                        nowplayer->setText(QString("Player:%1").arg(roomList[0].playerList[0].getID()));
                        for(int j=0;j<roomList[i].getPlnum();j++)
                            server->send(roomList[i].playerList[j].getSocket(),NetworkData(OPCODE::START_GAME_OP,plName,seq));
                        qint64 epoch = QDateTime::currentMSecsSinceEpoch();
                        QString str =  QString("%1").arg(epoch);
                        for(int i=0;i<roomList[0].getPlnum();i++){
                             server->send(roomList[0].playerList[i].getSocket(),NetworkData(OPCODE::START_TURN_OP,QString(num2place(flag)),str));
                        }
                        //test
                        qDebug() << "server send START_GAME_OP";
                        //test end
                        ServerWait->hide();//关闭等待界面
                        this->InitializeChecker();
                        this->setPlayerTable();
                        this->show();//打开棋盘
                        timeleft=15;
                        id = startTimer(1000);
                    }
                    break;
                    }
                }
            if(!found)
                server->send(client,NetworkData(OPCODE::ERROR_OP,QString::number(static_cast<int>(ERRCODE::NOT_IN_ROOM)),QString("")));
        }
    default:
    break;
    }
}
void Widget::timerEvent(QTimerEvent *event){
    timeleft--;
    if(timeleft<0){
        this->killTimer(id);//停止计时
        for(int i=0;i<10;i++){
            btn[flag][i]->close();
            isfill[btn[flag][i]->x][btn[flag][i]->y] = 0;
        }
        //test
        qDebug() << "out player is " << flag;
        //test end
        isover[flag] = true;
        overnum++;
        overlist.append(roomList[0].playerList[flag].getID());
        //向其他玩家发送超时判负信号
        for(int i=0;i<roomList[0].getPlnum();i++){
            //if(i!=flag)
                server->send(roomList[0].playerList[i].getSocket(),NetworkData(OPCODE::MOVE_OP,QString(roomList[0].playerList[flag].getPlace()),QString("-1")));
        }
        if(overnum==playernum-1) {
            for(int i=0;i<playernum;i++){
                if(!isover[i]){
                    overlist.append(roomList[0].playerList[i].getID());
                    break;
                }
            }
            if(overlist.length()){
                for(int g=0;g<overlist.length();g++)
                    ranklist.append(overlist[overlist.length()-g-1]).append(" ");
             }
            for(int i=0;i<playernum;i++){
                server->send(roomList[0].playerList[i].getSocket(),NetworkData(OPCODE::END_GAME_OP,ranklist,QString(" ")));
                //test
                qDebug() << "server send END_GAME_OP";
                //test end
                }
              ranklist = "";
              roomList.removeAt(0);
              overlist.clear();
              overnum=0;
              for(int i=0;i<6;i++){
                  ServerWait->setPlayerName(i,"");
                  ServerWait->setPlayerReady(i,false);
              }
              ChooseServer->show();
              this->hide();
        }
        else{
            changeplayer();
            timeleft=15;
            id = startTimer(1000);
            qint64 epoch = QDateTime::currentMSecsSinceEpoch();
            QString str =  QString("%1").arg(epoch);
            for(int i=0;i<roomList[0].getPlnum();i++){
                 server->send(roomList[0].playerList[i].getSocket(),NetworkData(OPCODE::START_TURN_OP,QString(num2place(flag)),str));
            }
        }
    }
    else{
       clock2->setText(QString("%1 s").arg(timeleft));
    }
}

void Widget::setPlayerTable(){
    QLabel* Title = new QLabel(this);
    Title->setFont(QFont("Agency FB",20));
    Title->setGeometry(15,5,300,50);
    Title->setText("Players:");
    Title->setStyleSheet("color:black;");
    PlayerTable.append(Title);

    int plnum = roomList[0].getPlnum();
    int cir=30;
    for(int i=0;i<plnum;i++){
        QLabel* pl = new QLabel(this);
        pl->setFont(QFont("Agency FB",15));
        pl->setGeometry(15,10+cir,300,50);
        pl->setText(roomList[0].playerList[i].getID());
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
        cir += 25;
    }
}

void Widget::InitializeChecker(){
   qDebug() << "enter initialize";
   playernum= roomList[0].getPlnum();
   int k=0;
   flag=pink;
   if(playernum==2){
   for(int j=5;j<=8;j++){
       for(int i=j-4;i<=4;i++){
           btn[pink][k]=new CheckerButton(this);
           btn[pink][k]->setGeometry(loc[-i+8][j+8].rx()-RR/2,loc[-i+8][j+8].ry()-RR/2,RR,RR);
           btn[pink][k]->setIcon(QPixmap(":/image/pink.png"));
           btn[pink][k]->setIconSize(QSize(RRR+0.5,RRR+0.5));
           btn[pink][k]->setFlat(true);
           btn[pink][k]->player=pink;//set player
           btn[pink][k]->x=-i+8;
           btn[pink][k]->y=j+8;
           isfill[-i+8][j+8]=pink+1;
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

   for(int t=0;t<10;t++)
       for(int j=0;j<playernum;j++)
               btn[j][t]->setCheckable(false);

   for(int i=0;i<playernum;i++){
       isover[i]=false;
   }
}

char Widget::num2place(int num){
    int playernum = roomList[0].getPlnum();
    if(playernum==2){
        if(num==0)
            return 'A';
        else
            return 'D';
    }
    if(playernum==3){
        if(num==0)
            return 'A';
        else if(num==1)
            return 'C';
        else
            return 'E';
    }
    if(playernum==6)
        return num+'A';
}
