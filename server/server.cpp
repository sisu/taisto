#include <QDataStream>
#include <cmath>
#include "server.h"
#include "messages.h"
#include "constants.h"

Server::Server(int spawns): area(spawns), curSpawn(0), nextID(1)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000/FPS);
	bulletID=1;
}

void Server::update()
{
	while(hasPendingConnections()) {
		qDebug()<<"got connection";
		QTcpSocket* sock = nextPendingConnection();
		Player pl(sock,0,0,nextID++);
		spawnPlayer(pl);
		players.append(pl);
		sendInitialInfo(sock, pl.id);
	}
	for(int i=0; i<players.size(); ) {
		if (players[i].socket->state() != QAbstractSocket::ConnectedState)
			players.erase(players.begin()+i);
		else ++i;
	}

	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);

	stream << 1 + 4 + players.size()*(4+8+8+8+4+4+4+8);
	stream << MSG_STATE << players.size();
	for(int i=0; i<players.size(); ++i) {
		Player& pl = players[i];
		pl.update(*this);
//		qDebug()<<"pl1"<<pl.x<<pl.y;
		stream<<pl.id<<pl.x<<pl.y<<pl.angle<<pl.moveForward<<pl.moveSide<<pl.turn<<pl.health;
	}

    /*
    for(int i = 0; i < bots.size(); ++i) {
        
    }
    */

	sendToAll(stateMsg);


	for(int i=0; i<bullets.size(); ) {
		if (bullets[i].update(*this)) {
			qDebug()<<"removing bullet"<<bullets[i].x<<bullets[i].y;
			sendHit(bullets[i]);
			bullets[i] = bullets.back();
			bullets.pop_back();
		} else ++i;
	}
}

void Server::sendInitialInfo(QTcpSocket* sock, int id)
{
	QDataStream s(sock);
	s << 1 + 4+4 + area.w*area.h*4 + 4;
	s<<MSG_INITIAL;
	s<<area.w<<area.h;
	for(int i=0; i<area.parts.size(); ++i)
		for(int j=0; j<area.parts[i].data.size(); ++j)
			s<<area.parts[i].data[j];
	s<<id;
	qDebug()<<"sent player id"<<id;
	sock->flush();
}

void Server::sendToAll(QByteArray msg)
{
	for(int i=0; i<players.size(); ++i)
		players[i].socket->write(msg);
	for(int i=0; i<players.size(); ++i)
		players[i].socket->flush();
}

void Server::sendHit(const Bullet& b)
{
	QByteArray msg;
	QDataStream s(&msg, QIODevice::WriteOnly);

	s << 1+sizeof(b.id);
	s << MSG_HIT << b.id;

	sendToAll(msg);
}

double damages[] = {0,0.1};
void Server::hitPlayer(Player& p, int weapon)
{
	p.health -= damages[weapon];
	if (p.health < 0) {
		spawnPlayer(p);
	}
}
void Server::spawnPlayer(Player& p)
{
	QPair<int,int> spawn = area.getSpawnPoint(curSpawn);
	p.x = spawn.first+.5;
	p.y = spawn.second+.5;
	p.health = 1;
	p.angle = -M_PI/2;
}
