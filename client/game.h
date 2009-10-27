#ifndef GAME_H
#define GAME_H
#include<QObject>
#include <QTimer>
#include<QString>
#include "window.h"
#include "connection.h"
#include "engine.h"
#include "object.h"
class Game : public QObject {
Q_OBJECT
public:
    Game();
    QTimer* timer;
    Engine engine;
    Connection conn;
    Window window;
	Object player;

    void start();
    public slots:
        void go();

};
#endif
