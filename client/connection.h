#ifndef CONNECTION_H
#define CONNECTION_H
#include<QString>
#include<QObject>
#include <QTcpSocket>
#include <QDataStream>
#include "player.h"
#include "engine.h"
#include "area.h"

class Connection : public QTcpSocket {
	Q_OBJECT
public:
	Connection(Player* pl, Engine& e);
	void connect(QString,int);
	void update();
	void sendStatus();
	void sendShoot();

	int packetCount;

private:
	Player* player;
	Engine& engine;

	void readInitial(QDataStream& s);
	void readState(QDataStream& s);
	void readShoot(QDataStream& s);
	void readHit(QDataStream& s);
	void readEnemy(QDataStream& s);
	void readItem(QDataStream& s);
	int packetSize;
};
#endif
