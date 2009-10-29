#ifndef BOT_H
#define BOT_H

#include <QTime>
#include "area.h"
#include "constants.h"
#include "unit.h"

class Server;

const double botArmors[1+WEAPONS] = {0,0.15,0.2,0.3,0.8,0.9};

class Bot : public Unit {
public:
	Bot(double xx, double yy, int w): Unit(xx,yy) {
		lastShoot=0;
		shooting=0;
		nextT=0;
		angle=M_PI/2;
		armor=botArmors[w];
		weapon = w;
	}

	void runAI(Server& s, int t);

	int weapon;

	bool shooting;
	int lastShoot;
	int nextT;
};

#endif
