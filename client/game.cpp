#include "game.h"

Game::Game(): conn(&player, engine.area), player(0,0,0,0,0), window(engine) {
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
    for(int i = 0; i < window.keysHeld.size(); ++i) {
        switch(window.keysHeld[i]) {
            case Qt::Key_Left:
                player.mx = -1;
                break;
            case Qt::Key_Right:
                player.mx = 1;
                break;
            case Qt::Key_Down:
                player.mx = -1;
                break;
            case Qt::Key_Up:
                player.mx = 1;
                break;
            default:
                break;
        }
    }
    window.draw(player.x,player.y);
	conn.sendStatus();


}
