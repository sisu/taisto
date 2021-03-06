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
#include "stats.h"
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
	int prevSec;


    void start(QString, int);
    void start();
    public slots:
        void go();
		void end();

};
#endif
