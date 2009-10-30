#include <cstdlib>
#include "game.h"
#include "constants.h"
#include "setupdialog.h"

Game::Game(): conn(&player, engine), window(engine,player), player() {
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(go()));
	 player.x=5, player.y=5;
	 engine.players.append(player);
	 startTime.start();
	 prevSec=0;

	connect(this, SIGNAL(disconnected()), this, SLOT(end()));
}

void Game::start(QString ip, int port) {
    
    //Moottori
    
    
    // Luo verkko
	// 

	conn.connect(ip,port);
	conn.waitForConnected(3000);

	//Luo ikkuna + piirtopinta
	window.show();
	timer->start(FRAMETIME*1000);

}
void Game::start(){
	SetupDialog setup;
	if(setup.exec()) {
		start(setup.hostAddress(),32096);
	}

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
	if (player.shooting && t>player.shootTime && (player.weapon==1 || engine.bulletCounts[player.weapon]>0)) {
		conn.sendShoot();
		player.shootTime = t + loadTimes[player.weapon];
		engine.bulletCounts[player.weapon] -= 1;
		if(engine.bulletCounts[player.weapon]==0) player.weapon=1;
	}           
	if (t/1000>prevSec) {
		prevSec = t/1000;
		qDebug()<<"packet count:"<<conn.packetCount;
		conn.packetCount = 0;
	}

	if (conn.state() != QAbstractSocket::ConnectedState) {
		qDebug()<<conn.state();
		end();
	}

	// check applied cheats

	QList<QString> cheats = window.getActivatedCheats();

	for(int i = 0; i < cheats.size(); ++i) {
		if(cheats[i] == "ibeatyou") {
			for(int j = 0; j < engine.bulletCounts.size(); ++j) {
				engine.bulletCounts[j] = 100000;
			}
		}
	}

	// Remove old lightings
	for(int i=0; i<engine.lightnings.size(); ++i) {
		if (engine.lightnings[i].first.elapsed()>100) {
			qDebug()<<"removing lighting";
			engine.lightnings[0] = engine.lightnings.back();
			engine.lightnings.pop_back();
		}
	}
}
void Game::end() {
	qDebug()<<"disconnected";
	exit(0);
}
