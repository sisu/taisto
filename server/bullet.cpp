#include "bullet.h"
#include <QList>

bool Bullet::update(QList<Player> plrs, const Area& a)
{
	double v = sqrt(vx*vx + vy*vy);
	double len = v * FRAME_TIME;
	int count = len / STEP + 1;
	double step = len/count;
	double dx = vx/v * step;
	double dy = vy/v * step;
	for(int i=0; i<count; ++i) {
		x += dx;
		y += dy;
		int ix=x, iy=y;

		if (a.blocked(ix,iy))
			return 1;
	}
	return 0;
}
