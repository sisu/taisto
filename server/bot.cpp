#include "bot.h"
#include "server.h"
#include <cmath>

const double A=0.5;

void Bot::runAI(Server& s)
{
	shooting=0;
	turn=moveForward=moveSide=0;
	double vx=cos(angle), vy=-sin(angle);
	for(int i=0; i<s.players.size(); ++i) {
		Player& pl = s.players[i];
		double dx = pl.x-x;
		double dy = pl.y-y;
		double l = sqrt(dx*dx+dy*dy);
		double v = sqrt(vx*vx+vy*vy);
		if (l < 8) {
			double a = acos((vx*dx+vy*dy)/(l*v));
			if (a>-A && a<A) {
				qDebug()<<"YYYAAAAARRRRRRRR";
				double dir = vx*dy - vy*dx;
				if (dir>0) turn=-1;
				else turn=1;
				shooting=1;
				if (l>4) moveForward=1;
				return;
			}
		}
	}
	moveForward=1;
	turn = rand()%2-1;
}
