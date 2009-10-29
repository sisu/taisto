#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTimer>
#include <QByteArray>
#include <QTime>
#include "unit.h"
#include "player.h"
#include "area.h"
#include "bullet.h"
#include "bot.h"
#include "item.h"

class Server: public QTcpServer {
	Q_OBJECT
public:
	Server(int spawns);

	void sendToAll(QByteArray msg);
//	void hitPlayer(Unit& pl, int weapon);
	void bulletHit(Unit* pl, const Bullet& b);

	Area area;
	QList<Bullet> bullets;
	QList<Player> players;
	QList<Item> items;
	QList<Bot> bots;
	int bulletID;
    void hitBot(Bot& b, int weapon);
	void addBullet(int w, double x, double y, double dx, double dy, double v);

public slots:
	void update();

private:
	int curSpawn;
	QTimer timer;
	int nextID;
	QTime curT;
	int prevSec, frames;
	int lastSpawn;

	void sendInitialInfo(QTcpSocket* sock, int id);
	void sendHit(const Bullet& b);
	void spawnPlayer(Unit& p, bool bot=0);
	void createBot(int place);
	void createItem();
	void updatePlayers();
	void updateBots();
	void updateBullets();
	void updateItems();
	void spawnInitial();
	void rocketDamage(Unit& u, const Bullet& b);
};

#endif
