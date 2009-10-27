#include "game.h"

Game::Game() {
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
    while(conn.hasdata()) {
        process(conn.nextdata());
    }
    engine.go();
    window.draw(engine.players,engine.bots);


}

void Game::process(QString data) {

}
