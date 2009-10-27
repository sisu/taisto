#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include "object.h"
class Engine : public QObject {
    Q_OBJECT
    public:
        Engine();
        QList<Object*> players;
        QList<Object*> bots;



};
#endif
