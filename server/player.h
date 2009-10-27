#ifndef PLAYER_H
#define PLAYER_H

#include <QTcpSocket>

struct Player {

	Player(QTcpSocket* s, double xx, double yy): socket(s), x(xx), y(yy) {}
	void update();
	void handleMessage();

	QTcpSocket* socket;
	double x,y,angle;
	int moveForward, moveSide, turn;
};

#endif
