#ifndef BULLET_H
#define BULLET_H

#include <QList>
#include "constants.h"
#include "area.h"
#include "player.h"

const double STEP = 0.2;

struct Bullet {
	int id,type;
	double x,y,vx,vy;

	Bullet(int i, int t, double xx, double yy, double vxx, double vyy):
		id(i), type(t), x(xx), y(yy), vx(vxx), vy(vyy) {}

	bool update(QList<Player> plrs, const Area& a);
};

#endif
