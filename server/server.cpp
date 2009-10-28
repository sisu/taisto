#include <QDataStream>
#include "server.h"
#include "messages.h"
#include "constants.h"

Server::Server(int spawns): area(spawns), curSpawn(0), nextID(1)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000/FPS);
}

void Server::update()
{
	while(hasPendingConnections()) {
		qDebug()<<"got connection";
		QTcpSocket* sock = nextPendingConnection();
		QPair<int,int> p = area.getSpawnPoint(curSpawn);
//		qDebug()<<"spawn"<<p.first<<p.second;
		Player pl(sock, p.first+.5, p.second+.5, nextID++);
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

	stream << 1 + 4 + players.size()*(4+8+8+8+4+4+4);
	stream << MSG_STATE << players.size();
	for(int i=0; i<players.size(); ++i) {
		Player& pl = players[i];
		pl.update(*this);
//		qDebug()<<"pl1"<<pl.x<<pl.y;
		stream<<pl.id<<pl.x<<pl.y<<pl.angle<<pl.moveForward<<pl.moveSide<<pl.turn;
	}

	sendToAll(stateMsg);


	for(int i=0; i<bullets.size(); ) {
		if (bullets[i].update(players, area)) {
			qDebug()<<"removing bullet"<<bullets[i].x<<bullets[i].y;
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
