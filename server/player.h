#ifndef PLAYER_H
#define PLAYER_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <cmath>
#include "area.h"
#include "constants.h"

class Server;

class Player {
public:
	Player(QTcpSocket* s, double xx, double yy, int i): socket(s), x(xx), y(yy), id(i) {
		moveForward=moveSide=turn=0;
		angle=-M_PI/2;
		packetSize=-1;
	}
	void update(Server& s);

	QTcpSocket* socket;
	double x,y,angle;
	int moveForward, moveSide, turn;
	int id;

private:
	void readState(QDataStream& s);
	void readShoot(QDataStream& s, Server& serv);
	void fix(double px, double py, double d=PLAYER_RADIUS);

	int packetSize;
};

#endif
