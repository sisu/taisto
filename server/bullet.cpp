#include "bullet.h"
#include "server.h"
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

bool Bullet::update(Server& s, QList<Unit*>& plrs)
{
	Area& a = s.area;
	double v = sqrt(vx*vx + vy*vy);
	double len = v * FRAME_TIME;

	double ex = x + vx * FRAME_TIME;
	double ey = y + vy * FRAME_TIME;
	double vx0=vx/v, vy0=vy/v;
	int nearest=-1;
	double nDist=1e200;
	for(int i=0; i<plrs.size(); ++i) {
		double px = plrs[i]->x, py = plrs[i]->y;
		double d;
		if ((d=distToPlayer(x,y,px,py,vx0,vy0))<nDist && dist(x,y,ex,ey,px,py) < PLAYER_RADIUS) {
			nearest = i;
			nDist = d;
		}
	}

	int ix=x, iy=y;
	int idx=vx<0?-1:1, idy=vy<0?-1:1;
	double x0=x,y0=y;
	double d2;
	while((d2=(x-x0)*(x-x0) + (y-y0)*(y-y0)) <= len*len) {
		if (d2 >= nDist*nDist) {
			s.hitPlayer(*plrs[nearest], type);
			return 1;
		}
		if (a.blocked(ix,iy)) return 1;
		int ix2=ix+idx, iy2=iy+idy;
		double dx = idx<0 ? x-ix : ix2-x;
		double dy = idy<0 ? y-iy : iy2-y;
		double xx = dy*fabs(vx0/vy0);
		double yy = dx*fabs(vy0/vx0);
		if (yy>dy) {
			y=idy<0?iy:iy2;
			iy=iy2;
			x += vx0*xx;
		} else {
			x=idx<0?ix:ix2;
			ix=ix2;
			y += vy0*yy;
		}
	}
	if (nDist < len) {
		s.hitPlayer(*plrs[nearest], type);
		return 1;
	}
	x=ex,y=ey;
	return 0;
}
