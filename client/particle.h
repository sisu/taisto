#ifndef PARTICLE_H
#define PARTICLE_H

#include "constants.h"
#include "area.h"
#include "physics.h"
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
	bool update(const Area& a) {
		double xx = x+vx*FRAME_TIME;
		double yy = y+vy*FRAME_TIME;
		QPointF p = getWallHitPoint(x,y,xx,yy,a);
		x=p.x(), y=p.y();
		dist += v*FRAME_TIME;
		time += FRAME_TIME;
		return dist>=ROCKET_RADIUS;
	}
};

#endif
