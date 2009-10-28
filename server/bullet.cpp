#include "bullet.h"
#include <QList>
#include <cmath>

inline double dist(double ax, double ay, double bx, double by)
{
	double dx=ax-bx, dy=ay-by;
	return sqrt(dx*dx + dy*dy);
}
inline double dist(double ax, double ay, double bx, double by, double px, double py)
{
	double dx = bx-ax, dy = by-ay;
	double u = ((px-ax)*dx + (py-ay)*dy) / (dx*dx+dy*dy);

	if (u>=0 && u<=1)
		return dist(ax+u*dx,ay+u*dy,px,py);
	else return fmin(dist(ax,ay,px,py),dist(bx,by,px,py));
}
double distToPlayer(double x, double y, double px, double py, double vx, double vy)
{
	double ax=px-x, ay=py-y;
	return ax*vx + ay*vy;
}

bool Bullet::update(QList<Player> plrs, const Area& a)
{
	double v = sqrt(vx*vx + vy*vy);
	double len = v * FRAME_TIME;

	double ex = x + vx * FRAME_TIME;
	double ey = y + vy * FRAME_TIME;
	double vx0=vx/v, vy0=vy/v;
	int nearest=-1;
	double nDist=1e200;
	for(int i=0; i<plrs.size(); ++i) {
		double px = plrs[i].x, py = plrs[i].y;
		double d;
		if ((d=distToPlayer(x,y,px,py,vx0,vy0))<nDist && dist(x,y,ex,ey,px,py) < PLAYER_RADIUS) {
			nearest = i;
			nDist = d;
		}
	}

	int count = len / STEP + 1;
	double step = len/count;
	double dx = vx/v * step;
	double dy = vy/v * step;
	for(int i=0; i<count; ++i) {
		if ((i+1)*step > nDist) {
			return 1;
		}
		x += dx;
		y += dy;
		int ix=x, iy=y;

		if (a.blocked(ix,iy))
			return 1;
	}
	return 0;
}
