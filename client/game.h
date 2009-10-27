#ifndef GAME_H
#define GAME_H
#include<QObject>
#include "window.h"
#include "connection.h"
#include "engine.h"
#include <QTimer>
#include<QString>
class Game : public QObject {
Q_OBJECT
public:
    Game();
    QTimer* timer;
    Engine engine;
    Connection conn;
    Window window;
    void start();
    void process(QString);
    public slots:
        void go();

};
#endif
