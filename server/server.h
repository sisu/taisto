#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTimer>
#include <QByteArray>
#include <QTime>
#include <QPointF>
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
	int curSpawn;

    void hitBot(Bot& b, int weapon);
	void addBullet(int w, double x, double y, double dx, double dy, double v, Player* pl);
	void hitLightning(Unit& u);

	Player* getPlayer(int id) {
		for(int i=0; i<players.size(); ++i) if (players[i].id==id) return &players[i];
		return 0;
	}

public slots:
	void update();

private:
	QTimer timer;
	int nextID;
	QTime curT;
	int prevSec, frames;
	int lastSpawn;
	int nextItem;

	void sendInitialInfo(QTcpSocket* sock, int id);
	void sendHit(const Bullet& b);
	void sendStats();
	void spawnPlayer(Unit& p, bool bot=0);
	void createBot(int place, int w);
	Item& createItem(int type);
	void updatePlayers();
	void updateBots();
	void updateBullets();
	void updateItems();
	void spawnStuff(bool next=0);
	void rocketDamage(Unit& u, const Bullet& b);
	void lightningDamage(Unit& shooter, Unit& pl, QList<QPointF>& pts, Player* player);
};

#endif
