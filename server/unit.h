#ifndef UNIT_H
#define UNIT_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <cmath>
#include "area.h"
#include "constants.h"

class Server;
class Player;

class Unit {
    public:
        Unit(double xx, double yy): x(xx), y(yy) {
            moveForward=moveSide=turn=0;
            angle=-M_PI/2;
            packetSize=-1;
			health=1;
			armor=1;
			lastHitT=-1e9;
			lastShooter=0;
        }

        double x,y,angle;
        int moveForward, moveSide, turn;
        void shoot(int weapon, Server& serv);
        void updatePhysics(Server&);
		double health;
		double armor;
		int id; // -1 for bots

		int lastHitT;
		Player* lastShooter;

    protected:
        void fix(double px, double py, double d=PLAYER_RADIUS);
        int packetSize;

	private:
		void makeBullet(Server& s, int weap, double a, double distr);
};

#endif
