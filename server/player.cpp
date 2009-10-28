#include "player.h"
#include "constants.h"
#include "messages.h"
#include <cmath>

const double FRAME_TIME = 1.0/FPS;

inline void Player::fix(double px, double py, double d)
{
	double dx=x-px, dy=y-py;
	double r2 = dx*dx + dy*dy;
	if (r2 < d*d) {
		double a = d / sqrt(r2);
		x = px + dx*a;
		y = py + dy*a;
	}
}

void Player::update(const Area& a)
{
	while(socket->bytesAvailable()) {
		QDataStream s(socket);
		if (packetSize<0) {
			if (socket->bytesAvailable()<4) break;
			s>>packetSize;
		}
		if (socket->bytesAvailable()<packetSize) break;
		packetSize=-1;
		quint8 type;
		s>>type;
//		qDebug()<<"msg"<<type;
		switch(type) {
			case MSG_STATE:
				readState(s);
				break;
		}
	}

	double ca = cos(angle);
	double sa = sin(angle);
	const double spd = MOVE_SPEED * FRAME_TIME;
	x += moveForward*ca * spd;
	y -= moveForward*sa * spd;
	x += moveSide*sa * spd;
	y -= moveSide*ca * spd;

	int ix=x, iy=y;
    
	if (a.blocked(ix-1,iy)) fix(ix, y);
	if (a.blocked(ix+1,iy)) fix(ix+1, y);
	if (a.blocked(ix,iy-1)) fix(x, iy);
	if (a.blocked(ix,iy+1)) fix(x, iy+1);
	if (a.blocked(ix-1,iy-1)) fix(ix,iy);
	if (a.blocked(ix+1,iy-1)) fix(ix+1,iy);
	if (a.blocked(ix+1,iy+1)) fix(ix+1,iy+1);
	if (a.blocked(ix-1,iy+1)) fix(ix,iy+1);

	angle += turn * TURN_SPEED * FRAME_TIME;
}

void Player::readState(QDataStream& s)
{
	s>>angle>>moveForward>>moveSide>>turn;
//	qDebug()<<"got"<<x<<y<<moveForward<<moveSide<<turn;
}
