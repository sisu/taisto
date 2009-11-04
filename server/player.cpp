#include <cmath>
#include <cstdlib>
#include "player.h"
#include "constants.h"
#include "messages.h"
#include "server.h"
#include "utils.h"
#include "unit.h"

void Player::update(Server& s)
{
	while(socket->bytesAvailable()) {
		QDataStream is(socket);
		if (packetSize<0) {
			if (socket->bytesAvailable()<4) break;
			is>>packetSize;
		}
		if (socket->bytesAvailable()<packetSize) break;
		packetSize=-1;
		quint8 type;
		is>>type;
		switch(type) {
			case MSG_STATE:
				readState(is);
				break;
			case MSG_SHOOT:
				readShoot(is, s);
				break;
			case MSG_INFO:
				readInfo(is);
				break;
		    case MSG_CHAT:
		        readChat(is);
		        break;
			default:
				qDebug()<<type;
				abort();
		}
	}

	updatePhysics(s);
}

void Player::readState(QDataStream& s)
{
	s>>moveForward>>moveSide>>turn;
//	qDebug()<<"got"<<x<<y<<moveForward<<moveSide<<turn;
}
void Player::readShoot(QDataStream& s, Server& serv)
{
//	qDebug()<<"got shoot";
	int weapon;
	s>>weapon;

	shoot(weapon, serv);
}
void Player::readInfo(QDataStream& s)
{
	int len;
	s>>len;
	char* buf = new char[2*len+1];
	s.readRawData(buf, 2*len);
	buf[2*len]=0;
	name = QString::fromRawData((QChar*)buf, len);
	for(int i=0; i<len; ++i) qDebug()<<int(buf[i]);
	qDebug()<<"got name"<<name<<len<<name.size();

	qDebug()<<name;
}
void Player::readChat(QDataStream& s)
{
	int len;
	s>>len;
	char* buf = new char[2*len+1];
	s.readRawData(buf, 2*len);
	buf[2*len]=0;
	chatMessage = QString::fromRawData((QChar*)buf, len);
	for(int i=0; i<len; ++i) qDebug()<<int(buf[i]);
	qDebug()<<"got chat message"<<chatMessage<<len<<chatMessage.size();
    
	//qDebug()<<msg;
}
