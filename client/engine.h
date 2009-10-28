#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include <QMap>
#include "object.h"
#include "area.h"
#include "bullet.h"
const double FRAMETIME=0.040;
class Engine : public QObject {
    Q_OBJECT
    public:
        Engine();
        void go();

        QList<Object> players;
        QList<Object> bots;
        QMap<int,Bullet> bullets;
        
		Area area;
};
#endif
