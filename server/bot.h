#ifndef BOT_H
#define BOT_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <cmath>
#include "area.h"
#include "constants.h"
#include "unit.h"

class Server;

class Bot : public Unit {
    public:
        Bot(double xx, double yy): Unit(xx,yy) {}
};

#endif
