#include <QDataStream>
#include <cmath>
#include <qglobal.h>
#include "server.h"
#include "messages.h"
#include "constants.h"
#include "bot.h"

Server::Server(int spawns): area(spawns), curSpawn(0), nextID(1)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000/FPS);
	bulletID=1;
	curT.start();
	prevSec=frames=0;
}

void Server::update()
{
	while(hasPendingConnections()) {
		qDebug()<<"got connection; assigning"<<nextID;
		QTcpSocket* sock = nextPendingConnection();
		Player pl(sock,0,0,nextID++);
		spawnPlayer(pl);
		players.append(pl);
		sendInitialInfo(sock, pl.id);
	}
	for(int i=0; i<players.size(); ) {
		if (players[i].socket->state() != QAbstractSocket::ConnectedState) {
			qDebug()<<"dropping client"<<players[i].id;
			players.erase(players.begin()+i);
		}
		else ++i;
	}

	updatePlayers();
	updateBots();
	updateBullets();
	updateItems();

	// Handle spawn changes
	for(int i=0; i<players.size(); ++i) {
		if (players[i].y >= area.startPlaces[curSpawn+1])
			++curSpawn;
	}

	++frames;
	int t = curT.elapsed();
	if (t/1000 > prevSec) {
		qDebug()<<"fps:"<<frames;
		frames=0;
		prevSec=t/1000;
	}
}

void Server::updatePlayers()
{
	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);

	stream << 1 + 4 + players.size()*(4+8+8+8+4+4+4+8);
	stream << MSG_STATE << players.size();
	for(int i=0; i<players.size(); ++i) {
		Player& pl = players[i];
		pl.update(*this);
//		qDebug()<<"pl1"<<pl.x<<pl.y;
		stream<<pl.id<<pl.x<<pl.y<<pl.angle<<pl.moveForward<<pl.moveSide<<pl.turn<<pl.health;
	}
	sendToAll(stateMsg);
}
void Server::updateBots()
{
    if(bots.size() == 0) {
        QPair<int,int> pt = this->area.getSpawnPoint(this->curSpawn + 1);
		qDebug()<<"spawning bot to"<<pt.first<<pt.second;
        Bot b(pt.first+.5,pt.second+.5);
        bots.append(b);
    }
	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);

    stream << 1 + 4 + bots.size()*(8+8+8+4+4+4+8);
    stream << MSG_ENEMY << bots.size();

    for(int i = 0; i < bots.size(); ++i) {
        Bot& bot = bots[i];
		bot.runAI(*this);
        bot.updatePhysics(*this);
        stream << bot.x << bot.y << bot.angle << bot.moveForward <<
            bot.moveSide << bot.turn << bot.health;

		int t = curT.elapsed();
		if (bot.shooting && t>bot.lastShoot+200) {
			bot.shoot(1, *this);
			bot.lastShoot=t;
		}
    }
	sendToAll(stateMsg);
}

void Server::updateItems()
{
	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);
    stream << 1 + 4 + items.size()*(8+8+4);
    stream << MSG_ITEM << items.size();

    for(int i = 0; i < items.size(); ++i) {
        Item& it = items[i];
        stream << it.x << it.y << it.itemNo;
    }
	sendToAll(stateMsg);
}
void Server::updateBullets()
{
	QList<Unit*> common;
	for(int i=0; i<players.size(); ++i)
		common.append(&players[i]);
	for(int i=0; i<bots.size(); ++i)
		common.append(&bots[i]);

	for(int i=0; i<bullets.size(); ) {
		if (bullets[i].update(*this, common)) {
			qDebug()<<"removing bullet"<<bullets[i].x<<bullets[i].y;
			sendHit(bullets[i]);
			bullets[i] = bullets.back();
			bullets.pop_back();
		} else ++i;
	}
	for(int i=0; i<players.size(); ++i)
		if (players[i].health<=0) spawnPlayer(players[i],0);
	for(int i=0; i<bots.size(); ++i)
		if (bots[i].health<=0) spawnPlayer(bots[i],1);
}

void Server::sendInitialInfo(QTcpSocket* sock, int id)
{
	QDataStream s(sock);
	s << 1 + 4+4 + area.w*area.h*4 + 4+4 + 4;
	s<<MSG_INITIAL;
	s<<area.w<<area.h;
	for(int i=0; i<area.parts.size(); ++i)
		for(int j=0; j<area.parts[i].data.size(); ++j)
			s<<area.parts[i].data[j];
	s<<area.parts[0].data.size()/area.w<<area.parts[0].spawnH;
	s<<id;
	qDebug()<<"sent player id"<<id;
	sock->flush();
}

void Server::sendToAll(QByteArray msg)
{
#if 1
	int size;
	QDataStream tmp(msg);
	tmp>>size;
	if (size+4 != msg.size()) qDebug()<<"asd"<<size+4<<msg.size();
	Q_ASSERT(size+4 == msg.size());
#endif
	for(int i=0; i<players.size(); ++i)
		players[i].socket->write(msg);
	for(int i=0; i<players.size(); ++i)
		players[i].socket->flush();
}

void Server::sendHit(const Bullet& b)
{
	QByteArray msg;
	QDataStream s(&msg, QIODevice::WriteOnly);

	s << 1+sizeof(b.id);
	s << MSG_HIT << b.id;

	sendToAll(msg);
}

void Server::hitPlayer(Unit& p, int weapon)
{
	p.health -= damages[weapon];
	qDebug()<<"hit"<<p.health;
}

void Server::spawnPlayer(Unit& p, bool bot)
{
	QPair<int,int> spawn = area.getSpawnPoint(curSpawn+bot);
	p.x = spawn.first+.5;
	p.y = spawn.second+.5;
	p.health = 1;
	p.angle = bot ? M_PI/2 : -M_PI/2;
}

void Server::addBullet(int weap, double x, double y, double vx, double vy, double v)
{
	vx *= v, vy*=v;
	Bullet bullet(bulletID++,weap,x,y,vx,vy);
	bullets.append(bullet);

	QByteArray msg;
	QDataStream os(&msg, QIODevice::WriteOnly);
	os << 1 + 4+4 + 4*8;
	os << MSG_SHOOT << bullet.id << weap;
	os << x<<y<<vx<<vy;
	sendToAll(msg);
}
