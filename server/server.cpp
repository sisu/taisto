#include <QDataStream>
#include "server.h"
#include "messages.h"

Server::Server(int spawns): area(spawns), curSpawn(0), nextID(1)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(50);
}

void Server::update()
{
	while(hasPendingConnections()) {
		qDebug()<<"got connection";
		QTcpSocket* sock = nextPendingConnection();
		sendInitialInfo(sock);
		QPair<int,int> p = area.getSpawnPoint(curSpawn);
//		qDebug()<<"spawn"<<p.first<<p.second;
		Player pl(sock, p.first+.5, p.second+.5, nextID++);
		players.append(pl);
	}
	for(int i=0; i<players.size(); ) {
		if (players[i].socket->state() != QAbstractSocket::ConnectedState)
			players.erase(players.begin()+i);
		else ++i;
	}

	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);

	stream << MSG_STATE << players.size();
	for(int i=0; i<players.size(); ++i) {
		Player& pl = players[i];
		pl.update();
//		qDebug()<<"pl1"<<pl.x<<pl.y;
		stream<<pl.id<<pl.x<<pl.y<<pl.angle<<pl.moveForward<<pl.moveSide<<pl.turn;
	}

	sendToAll(stateMsg);
}

void Server::sendInitialInfo(QTcpSocket* sock)
{
	QDataStream s(sock);
	s<<MSG_INITIAL;
	int h=0;
	for(int i=0; i<area.parts.size(); ++i)
		h += area.parts[i].data.size()/area.w;
	s<<area.w<<h;
	for(int i=0; i<area.parts.size(); ++i)
		for(int j=0; j<area.parts[i].data.size(); ++j)
			s<<area.parts[i].data[j];
	sock->flush();
}

void Server::sendToAll(QByteArray msg)
{
	for(int i=0; i<players.size(); ++i)
		players[i].socket->write(msg);
	for(int i=0; i<players.size(); ++i)
		players[i].socket->flush();
}
