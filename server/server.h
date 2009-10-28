#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTimer>
#include <QByteArray>
#include "unit.h"
#include "player.h"
#include "area.h"
#include "bullet.h"
#include "bot.h"

class Server: public QTcpServer {
	Q_OBJECT
public:
	Server(int spawns);

	void sendToAll(QByteArray msg);
	void hitPlayer(Player& pl, int weapon);

	Area area;
	QList<Bullet> bullets;
	QList<Player> players;
	QList<Bot> bots;
	int bulletID;
    void hitBot(Bot& b, int weapon);

public slots:
	void update();

private:
	int curSpawn;
	QTimer timer;
	int nextID;

	void sendInitialInfo(QTcpSocket* sock, int id);
	void sendHit(const Bullet& b);
	void spawnPlayer(Player& p);
};

#endif
