#include "engine.h"

Engine::Engine() {
//    players.append(Object(0,PLR,10,10,2)); aa
//    bullets.insert(0,Bullet(0,0,0,1,1));
	for(int i=0; i<20; ++i) bulletCounts.append(0);
}

void Engine::go() {
    //Bullets
    QMap<int,Bullet>::Iterator it;
    for(it = bullets.begin(); it != bullets.end(); it++) {
        it->x+=it->vx*FRAMETIME;
        it->y+=it->vy*FRAMETIME;
    }


}
