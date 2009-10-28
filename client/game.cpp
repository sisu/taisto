#include "game.h"
#include "constants.h"

Game::Game(): conn(&player, engine), window(engine), player() {
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(go()));
     timer->start(FRAMETIME*1000);
	 player.x=5, player.y=5;
	 engine.players.append(player);
	 startTime.start();
	 prevSec=0;
}

void Game::start(QString ip, int port) {
    
    //Moottori
    
    
    // Luo verkko
    
    conn.connect(ip,port);

    //Luo ikkuna + piirtopinta
    window.show();

}

void Game::go() {
	conn.update();
    engine.go();
    window.updatePlayerMovement(player);
    window.draw(&player); //(player.x,player.y);
	conn.sendStatus();

//	qDebug()<<player.x<<player.y;

	int t = startTime.elapsed();
//	qDebug()<<player.shooting<<startTime.elapsed()<<player.shootTime+loadTimes[player.weapon];
	if (player.shooting && t>player.shootTime+loadTimes[player.weapon]) {
		conn.sendShoot();
		player.shootTime = t;
	}
	if (t/1000>prevSec) {
		prevSec = t/1000;
		qDebug()<<"packet count:"<<conn.packetCount;
		conn.packetCount = 0;
	}
}
