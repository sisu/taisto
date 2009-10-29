#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include "player.h"
#include "engine.h"
class RenderArea;
class Window : public QWidget
{
    Q_OBJECT
    public:
        QList<int> keysHeld;
        Window(Engine& engine, Player& pl);
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
        void updatePlayerMovement(Player&);
    public slots:
        void draw(Player*);
    private:
        RenderArea *renderArea;
		Player& player;
};
#endif
