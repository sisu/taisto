#include "server.h"
#include "messages.h"

Server::Server(int spawns): area(spawns), curSpawn(0)
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
		Player pl(sock, p.first+.5, p.second+.5);
		players.append(pl);
	}
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
