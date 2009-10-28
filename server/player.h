#ifndef PLAYER_H
#define PLAYER_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>

class Player {
public:
	Player(QTcpSocket* s, double xx, double yy, int i): socket(s), x(xx), y(yy), id(i) {
		moveForward=moveSide=turn=angle=0;
	}
	void update();

	QTcpSocket* socket;
	double x,y,angle;
	int moveForward, moveSide, turn;
	int id;

private:
	void readState(QDataStream& s);
};

#endif
