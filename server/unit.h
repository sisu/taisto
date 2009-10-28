#ifndef UNIT_H
#define UNIT_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <cmath>
#include "area.h"
#include "constants.h"

class Server;

class Unit {
    public:
        Unit(QTcpSocket* s, double xx, double yy, int i): socket(s), x(xx), y(yy), id(i) {
            moveForward=moveSide=turn=0;
            angle=-M_PI/2;
            packetSize=-1;
        }

        QTcpSocket* socket;
        double x,y,angle;
        int moveForward, moveSide, turn;
        int id;

    protected:
        void fix(double px, double py, double d=PLAYER_RADIUS);
        int packetSize;
};

#endif
