#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include <QMap>
#include <QPointF>
#include <QTime>
#include <QPair>
#include "object.h"
#include "area.h"
#include "bullet.h"
#include "player.h"
#include "item.h"

const double FRAMETIME=0.02;
class Engine : public QObject {
    Q_OBJECT
    public:
        Engine();
        void go();

        QList<Player> players;
        QList<Player> bots;
        QList<Item> items;
        QMap<int,Bullet> bullets;
		QList<int> bulletCounts;

		QList<QPointF> explosions;
		QList<QPair<QTime,QList<QPointF> > > lightnings;
        
		Area area;
};
#endif
