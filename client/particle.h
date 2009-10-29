#ifndef PARTICLE_H
#define PARTICLE_H

#include "constants.h"
#include <cmath>

struct Particle {
	double x,y,vx,vy;
	double v,dist;
	double time;
	QColor color;

	Particle(double x_, double y_, double vx_, double vy_, QColor c, double dst):
		x(x_),y(y_),vx(vx_),vy(vy_),color(c)
	{
		v = sqrt(vx*vx + vy*vy);
		dist=dst;
		time=0;
	}
	bool update() {
		x += vx*FRAME_TIME;
		y += vy*FRAME_TIME;
		dist += v*FRAME_TIME;
		time += FRAME_TIME;
		return dist>=ROCKET_RADIUS;
	}
};

#endif
