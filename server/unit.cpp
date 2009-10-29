#include "unit.h"
#include "constants.h"
#include "messages.h"
#include "server.h"
#include <cmath>
#include "utils.h"

void Unit::fix(double px, double py, double d)
{
	double dx=x-px, dy=y-py;
	double r2 = dx*dx + dy*dy;
	if (r2 < d*d) {
		double a = d / sqrt(r2);
		x = px + dx*a;
		y = py + dy*a;
	}
}

void Unit::updatePhysics(Server& s) {
	double ca = cos(angle);
	double sa = sin(angle);
	const double spd = MOVE_SPEED * FRAME_TIME;
	x += moveForward*ca * spd;
	y -= moveForward*sa * spd;
	x -= moveSide*sa * spd;
	y -= moveSide*ca * spd;

	Area& a = s.area;
	int ix=x, iy=y;
    
	if (a.blocked(ix-1,iy)) fix(ix, y);
	if (a.blocked(ix+1,iy)) fix(ix+1, y);
	if (a.blocked(ix,iy-1)) fix(x, iy);
	if (a.blocked(ix,iy+1)) fix(x, iy+1);
	if (a.blocked(ix-1,iy-1)) fix(ix,iy);
	if (a.blocked(ix+1,iy-1)) fix(ix+1,iy);
	if (a.blocked(ix+1,iy+1)) fix(ix+1,iy+1);
	if (a.blocked(ix-1,iy+1)) fix(ix,iy+1);

	for(int i=0; i<s.players.size(); ++i)
		if (this!=&s.players[i]) fix(s.players[i].x,s.players[i].y,2*PLAYER_RADIUS);
	for(int i=0; i<s.bots.size(); ++i)
		if (this!=&s.bots[i]) fix(s.bots[i].x,s.bots[i].y,2*PLAYER_RADIUS);
	angle += turn * TURN_SPEED * FRAME_TIME;
}

void Unit::shoot(int weap, Server& serv)
{
	switch(weap) {
		case 1:
			makeBullet(serv, weap, angle, 0.1);
			break;
		case 2:
			for(int i=0; i<4; ++i) {
				makeBullet(serv, weap, angle+((i-1.5)*.05), 0.03);
			}
			break;
		case 3:
			makeBullet(serv, weap, angle, 0.02);
			break;
		default:
			qDebug()<<"unknown weapon"<<weap;
			break;
	}
}
void Unit::makeBullet(Server& serv, int weap, double a, double distr)
{
	a += distr*(rndf()-.5);
	double dx=cos(a), dy=-sin(a);
	double px=x+dx*PLAYER_RADIUS*1.5, py=y+dy*PLAYER_RADIUS*1.5;
	serv.addBullet(weap,px,py,dx,dy,bulletSpeeds[weap]);
}
