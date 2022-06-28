#include "room.h"

void Room::addPl(QString _name,QTcpSocket* _client){
    playerNumber++;
    Player newpl(_name,_client);
    playerList.append(newpl);
    return;
}
