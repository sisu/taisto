#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

struct Player: public Object {
	Player(): Object(0,0,0,0,0) {
		shootTime=0;
		weapon=1;
		shooting=0;
		health=1;
	}
	bool shooting;
	int shootTime;
	int weapon;
	double health;
};

#endif
