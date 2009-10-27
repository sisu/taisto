#include "game.h"

Game::Game(): conn(&player, engine.area), player(0,0,0,0,0) {
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
    window.draw(player.x,player.y,engine.players,engine.bots);
	conn.sendStatus();


}
