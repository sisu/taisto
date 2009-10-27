#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTimer>
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

	void sendInitialInfo(QTcpSocket* sock);
};

#endif
