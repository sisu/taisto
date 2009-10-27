#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTimer>
#include <QByteArray>
#include "player.h"
#include "area.h"

class Server: public QTcpServer {
	Q_OBJECT
public:
	Server(int spawns);

public slots:
	void update();

private:
	QList<Player> players;
	Area area;
	int curSpawn;
	QTimer timer;
	int nextID;

	void sendInitialInfo(QTcpSocket* sock);
	void sendToAll(QByteArray msg);
};

#endif
