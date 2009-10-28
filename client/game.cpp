#include "game.h"

Game::Game(): conn(&player, engine.area), window(engine), player() {
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(go()));
     timer->start(40);
	 player.x=5, player.y=5;
	 engine.players.append(player);
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
    window.updatePlayerMovement(player);
    window.draw(player.x,player.y);
	conn.sendStatus();
}
