#ifndef BOT_H
#define BOT_H

#include <QTime>
#include "area.h"
#include "constants.h"
#include "unit.h"

class Server;

class Bot : public Unit {
public:
	Bot(double xx, double yy, int w): Unit(xx,yy) {
		lastShoot=0;
		shooting=0;
		nextT=0;
		angle=M_PI/2;
		armor=0.15;
		weapon = w;
	}

	void runAI(Server& s, int t);

	int weapon;

	bool shooting;
	int lastShoot;
	int nextT;
};

#endif
