#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QList>
#include <QString>
#include<QLineEdit>
#include "player.h"
#include "engine.h"
#include "stats.h"
class RenderArea;
class Window : public QWidget
{
    Q_OBJECT
    public:
        QString letterBuffer;
        QString chat;
        QLineEdit* chatInput;
        bool chatInputV;
        QList<QString> listOfCheats;
        QList<QString> activatedCheats;
        QList<int> keysHeld;
        
        Window(Engine& engine, Player& pl);
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
        void updatePlayerMovement(Player&);
        QList<QString> getActivatedCheats();
        void checkCheats();
        void addToLetterBuffer(int event);
    public slots:
        void draw(Player*, Stats*);
        void chatFinish();
    private:
        RenderArea *renderArea;
		Player& player;
};
#endif
