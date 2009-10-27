#include "game.h"

Game::Game(): conn(&player), player(0,0,0,0,0) {
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(go()));
     timer->start(40);
}

void Game::start() {
    
    //Moottori
    
    
    // Luo verkko
    
    conn.connect("127.0.0.1");
    

    //Luo ikkuna + piirtopinta
    window.show();

}

void Game::go() {
	conn.update(engine);
    engine.go();
    window.draw(engine.players,engine.bots);


}
