#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include "object.h"
#include "area.h"
class Engine : public QObject {
    Q_OBJECT
    public:
        Engine();
        void go();

        QList<Object> players;
        QList<Object> bots;
		Area area;
};
#endif
