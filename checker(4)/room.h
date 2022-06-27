#ifndef ROOM_H
#define ROOM_H

#include<QStringList>
#include<QString>
#include<QTcpSocket>
#include<QDebug>

class Player{
private:
    QString ID;
    bool isReady;
    QTcpSocket* pSocket;
    char place;
public:
    Player(QString _ID,QTcpSocket* _client):ID(_ID),pSocket(_client),isReady(false){
    }
//    Player(Player& p){
//        ID=p.ID;
//        isReady=p.isReady;
//        pSocket = p.pSocket;
//        place = p.place;
//    }
    QString getID() const{
        return ID;
    }
    bool ifReady() const{
        qDebug() << "class player ifready is :" << isReady;
        return isReady;
    }
    QTcpSocket* getSocket() const{
        return pSocket;
    }
    void setReady(){
        isReady = true;
        qDebug() << "class player:set ready";
        qDebug() << "now status is " << isReady;
        return;
    }
    char getPlace(){
        return place;
    }
    void setPlace(char p){
        place = p;
    }

};

class Room
{
private:
    int roomID;
public:
    QList<Player> playerList;
private:
    bool gameOn;
    int playerNumber;
    int readynum;
public:
    Room(int _id):roomID(_id){
        gameOn = false;
        playerNumber=0;
        readynum=0;
    }
    void plLeave(int index){
        playerNumber--;
        if(playerList[index].ifReady())
            readynum--;
        playerList.removeAt(index);
    }
    void addPl(QString _name,QTcpSocket* _client);
    QList<Player> getPl() const{
        return playerList;
    }
    int getID() const{
        return roomID;
    }
    bool ifON() const{
        return gameOn;
    }
    int getPlnum() const{
        return playerNumber;
    }
    int getReadynum()const{
        return readynum;
    }
    void addReady(){
        readynum++;
    }
    void gameBegin(){
        gameOn=true;
    }
};

#endif // ROOM_H
