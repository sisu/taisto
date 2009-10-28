#include <QDataStream>
#include <cmath>
#include "server.h"
#include "messages.h"
#include "constants.h"
#include "bot.h"

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
	sendToAll(stateMsg);

	// Bot stuff

    if(bots.size() == 0) {
        QPair<int,int> pt = this->area.getSpawnPoint(this->curSpawn + 1);
		qDebug()<<"spawning bot to"<<pt.first<<pt.second;
        Bot b(pt.first+.5,pt.second+.5);
        bots.append(b);
    }

	stateMsg.clear();
    stream << 1 + 4 + bots.size()*(8+8+8+4+4+4);
    stream << MSG_ENEMY << bots.size();

    for(int i = 0; i < bots.size(); ++i) {
        Bot& bot = bots[i];
        bot.updatePhysics(*this);
        stream << bot.x << bot.y << bot.angle << bot.moveForward << bot.moveSide << bot.turn;
    }
	sendToAll(stateMsg);


	for(int i=0; i<bullets.size(); ) {
		if (bullets[i].update(*this)) {
			qDebug()<<"removing bullet"<<bullets[i].x<<bullets[i].y;
			sendHit(bullets[i]);
			bullets[i] = bullets.back();
			bullets.pop_back();
		} else ++i;
	}


	// Handle spawn changes
	for(int i=0; i<players.size(); ++i) {
		if (players[i].y >= area.startPlaces[curSpawn+1])
			++curSpawn;
	}
}

void Server::sendInitialInfo(QTcpSocket* sock, int id)
{
	QDataStream s(sock);
	s << 1 + 4+4 + area.w*area.h*4 + 4+4 + 4;
	s<<MSG_INITIAL;
	s<<area.w<<area.h;
	for(int i=0; i<area.parts.size(); ++i)
		for(int j=0; j<area.parts[i].data.size(); ++j)
			s<<area.parts[i].data[j];
	s<<area.parts[0].data.size()/area.w<<area.parts[0].spawnH;
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
	qDebug()<<"hit"<<p.health;
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
