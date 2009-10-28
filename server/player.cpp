#include "player.h"
#include "constants.h"
#include "messages.h"
#include "server.h"
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

void Player::update(Server& s)
{
	while(socket->bytesAvailable()) {
		QDataStream is(socket);
		if (packetSize<0) {
			if (socket->bytesAvailable()<4) break;
			is>>packetSize;
		}
		if (socket->bytesAvailable()<packetSize) break;
		packetSize=-1;
		quint8 type;
		is>>type;
//		qDebug()<<"msg"<<type;
		switch(type) {
			case MSG_STATE:
				readState(is);
				break;
			case MSG_SHOOT:
				readShoot(is, s);
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

	angle += turn * TURN_SPEED * FRAME_TIME;
}

void Player::readState(QDataStream& s)
{
	s>>angle>>moveForward>>moveSide>>turn;
//	qDebug()<<"got"<<x<<y<<moveForward<<moveSide<<turn;
}
static double rndf()
{
	return double(rand())/RAND_MAX;
}
void Player::readShoot(QDataStream& s, Server& serv)
{
	qDebug()<<"got shoot";
	int weapon;
	s>>weapon;

	double a = angle + .1*(rndf()-.5);
	double dx = sin(a);
	double dy = -cos(a);
	double px = x + dx*PLAYER_RADIUS*1.5;
	double py = y + dy*PLAYER_RADIUS*1.5;
	double v = 20;
	double vx = dx * v;
	double vy = dy * v;

	QByteArray msg;
	QDataStream os(&msg, QIODevice::WriteOnly);
	os << 1 + 4 + 4*8;
	os << MSG_SHOOT << weapon;
	os << px<<py<<vx<<vy;
	serv.sendToAll(msg);
}
