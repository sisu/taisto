#ifndef PLAYER_H
#define PLAYER_H

#include <QTcpSocket>
#include <QByteArray>

struct Player {

	Player(QTcpSocket* s, double xx, double yy, int i): socket(s), x(xx), y(yy), id(i) {}
	void update();
	void handleMessage(QByteArray msg);

	QTcpSocket* socket;
	double x,y,angle;
	int moveForward, moveSide, turn;
	int id;
};

#endif
