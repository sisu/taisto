#include "area.h"
#include <cmath>
#include <QPair>

QPair<double,double> getWallHitPoint(double x0, double y0, double x1, double y1, const Area& a)
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
			return QPair<double,double>(x,y);
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
	return QPair<double,double>(x1,y1);
}
