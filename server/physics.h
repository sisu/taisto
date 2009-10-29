#ifndef PHYSICS_H
#define PHYSICS_H

#include <cmath>
#include <QPointF>

template<class Area>
inline QPointF getWallHitPoint(double x0, double y0, double x1, double y1, const Area& a)
{
	double vx=x1-x0, vy=y1-y0;
	double len2 = vx*vx + vy*vy;
	double len = sqrt(len2);
	double vx0=vx/len, vy0=vy/len;

	int ix=x0, iy=y0;
	int idx=vx<0?-1:1, idy=vy<0?-1:1;
	double x=x0,y=y0;
	double d2;
	while((d2=(x-x0)*(x-x0) + (y-y0)*(y-y0)) <= len2) {
		if (a.blocked(ix,iy)) {
			return QPointF(x,y);
		}
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
	return QPointF(x1,y1);
}
template<class Area>
inline bool rayHitsWall(double x0, double y0, double x1, double y1, const Area& a)
{
	QPointF p = getWallHitPoint(x0,y0,x1,y1,a);
	return fabs(p.x()-x1)>1e-3 || fabs(p.y()-y1)>1e-3;
}

#endif
