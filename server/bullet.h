#ifndef BULLET_H
#define BULLET_H

#include <QList>
#include "constants.h"
#include "unit.h"

const double STEP = 0.2;

class Server;
class Player;

struct Bullet {
	int id,type;
	double x,y,vx,vy;
	Player* shooter;

	Bullet(int i, int t, double xx, double yy, double vxx, double vyy, Player* sht):
		id(i), type(t), x(xx), y(yy), vx(vxx), vy(vyy), shooter(sht) {}

	bool update(Server& s, QList<Unit*>& obj);
};

#endif
