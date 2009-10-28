#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTimer>
#include <QByteArray>
#include "player.h"
#include "area.h"
#include "bullet.h"

class Server: public QTcpServer {
	Q_OBJECT
public:
	Server(int spawns);

	void sendToAll(QByteArray msg);

	Area area;
	QList<Bullet> bullets;

public slots:
	void update();

private:
	QList<Player> players;
	int curSpawn;
	QTimer timer;
	int nextID;

	void sendInitialInfo(QTcpSocket* sock, int id);
};

#endif
