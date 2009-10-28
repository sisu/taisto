#include "engine.h"

Engine::Engine() {
//    players.append(Object(0,PLR,10,10,2));
    bullets.insert(0,Bullet(0,0,0,1,1));
}

void Engine::go() {
    //Bullets
    QMap<int,Bullet>::Iterator it;
    for(it = bullets.begin(); it != bullets.end(); it++) {
        it->x+=it->vx*FRAMETIME;
        it->y+=it->vy*FRAMETIME;
    }

}
