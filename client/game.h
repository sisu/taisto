#ifndef GAME_H
#define GAME_H
#include<QObject>
#include "window.h"
#include "connection.h"
#include<QString>
class Game : public QObject {
Q_OBJECT
public:
    Game();
    Connection conn;
    Window window;
    void start();
};
#endif
