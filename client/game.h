#ifndef GAME_H
#define GAME_H
#include<QObject>
#include <QTimer>
#include<QString>
#include <QTime>
#include "window.h"
#include "connection.h"
#include "engine.h"
#include "player.h"

class Game : public QObject {
Q_OBJECT
public:
    Game();
    QTimer* timer;
    Engine engine;
    Connection conn;
    Window window;
	Player player;
	QTime startTime;

    void start(QString, int);
    public slots:
        void go();

};
#endif
