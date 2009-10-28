#ifndef PLAYER_H
#define PLAYER_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include "area.h"
#include "constants.h"

class Player {
public:
	Player(QTcpSocket* s, double xx, double yy, int i): socket(s), x(xx), y(yy), id(i) {
		moveForward=moveSide=turn=angle=0;
		packetSize=-1;
	}
	void update(const Area& a);

	QTcpSocket* socket;
	double x,y,angle;
	int moveForward, moveSide, turn;
	int id;

private:
	void readState(QDataStream& s);
	void fix(double px, double py, double d=PLAYER_RADIUS);

	int packetSize;
};

#endif
