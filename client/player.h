#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

struct Player: public Object {
	Player(): Object(0,0,0,0,0) {}
	bool shooting;
	int shootTime;
	int weapon;
};

#endif
