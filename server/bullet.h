#ifndef BULLET_H
#define BULLET_H

#include <QList>
#include "constants.h"

const double STEP = 0.2;

class Server;

struct Bullet {
	int id,type;
	double x,y,vx,vy;

	Bullet(int i, int t, double xx, double yy, double vxx, double vyy):
		id(i), type(t), x(xx), y(yy), vx(vxx), vy(vyy) {}

	bool update(Server& s);
};

#endif
