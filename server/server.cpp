#include "server.h"

Server::Server(int spawns): area(spawns), curSpawn(0)
{
}

void Server::update()
{
	while(hasPendingConnections()) {
		QTcpSocket* sock = nextPendingConnection();
		sendInitialInfo(sock);
		QPair<int,int> p = area.getSpawnPoint(curSpawn);
		Player pl(sock, p.first+.5, p.second+.5);
	}
}

void Server::sendInitialInfo(QTcpSocket* sock)
{
}
