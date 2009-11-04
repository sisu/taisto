#ifndef PLAYER_H
#define PLAYER_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <cmath>
#include "area.h"
#include "constants.h"
#include "unit.h"

class Server;

class Player : public Unit {
    public:
        Player(QTcpSocket* s, double xx, double yy, int i): Unit(xx,yy), socket(s) {
			id=i;
			kills=deaths=damageDone=0;
		}

        QTcpSocket* socket;
        void update(Server& s);

		int kills, deaths;
		double damageDone;
		QString name;
		QString chatMessage;

    private:
        void readState(QDataStream& s);
        void readShoot(QDataStream& s, Server& serv);
		void readInfo(QDataStream& s);
		void readChat(QDataStream& s);
};

#endif
