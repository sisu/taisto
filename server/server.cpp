#include <QDataStream>
#include <cmath>
#include <qglobal.h>
#include "server.h"
#include "messages.h"
#include "constants.h"
#include "bot.h"
#include "physics.h"

Server::Server(int spawns): area(spawns), curSpawn(0), nextID(1), nextItem(0)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000/FPS);
	bulletID=1;
	curT.start();
	prevSec=frames=0;
	lastSpawn=-1e9;

	spawnStuff();
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

	if (curSpawn>0) {
		int botCount=0;
		for(int i=0; i<bots.size(); ++i)
			if (bots[i].y <= area.startPlaces[curSpawn]+area.parts[curSpawn].spawnH) ++botCount;
		if (botCount>players.size()) --curSpawn;
	}

	bool playerNext=0;
	for(int i=0; i<players.size(); ++i)
		if (players[i].y >= area.startPlaces[curSpawn+1]) playerNext=1;
	bool botNext=0;
	for(int i=0; i<bots.size(); ++i)
		if (bots[i].y <= area.startPlaces[curSpawn+1]+area.parts[curSpawn+1].spawnH) botNext=1;

	if (playerNext && !botNext) {
//		items.clear();
		++curSpawn;
		spawnStuff();
	}

	++frames;
	int t = curT.elapsed();
	if (t/1000 > prevSec) {
		qDebug()<<"fps:"<<frames;
		frames=0;
		prevSec=t/1000;

		sendStats();
	}

//	qDebug()<<t<<lastSpawn+area.spawnIntervals[curSpawn];
	if (t > lastSpawn + area.spawnIntervals[curSpawn])
		spawnStuff(playerNext);

	for(int i=0; i<players.size(); ++i)
		players[i].socket->flush();
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
#if 0
    if(bots.size() == 0) {
        QPair<int,int> pt = this->area.getSpawnPoint(this->curSpawn + 1);
		qDebug()<<"spawning bot to"<<pt.first<<pt.second;
        Bot b(pt.first+.5,pt.second+.5);
        bots.append(b);
    }
#endif
	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);

    stream << 1 + 4 + bots.size()*(8+8+8+4+4+4+8+4);
    stream << MSG_ENEMY << bots.size();

	int t = curT.elapsed();
    for(int i = 0; i < bots.size(); ++i) {
        Bot& bot = bots[i];
		bot.runAI(*this, t);
        bot.updatePhysics(*this);
        stream << bot.x << bot.y << bot.angle << bot.moveForward <<
            bot.moveSide << bot.turn << bot.health << bot.weapon;

		int t = curT.elapsed();
		if (bot.shooting && t>bot.lastShoot+loadTimes[bot.weapon]) {
			bot.shoot(bot.weapon, *this);
			bot.lastShoot=t;
		}
    }
	sendToAll(stateMsg);
}

void Server::updateItems()
{
	for(int j=0; j<players.size(); ++j) {
		for(int i=0; i<items.size(); ) {
			double dx=items[i].x-players[j].x;
			double dy=items[i].y-players[j].y;
			double d2 = dx*dx + dy*dy;
			double s=ITEM_RADIUS+PLAYER_RADIUS;
			if (d2 < s*s) {
				qDebug()<<"sending item";
				QDataStream s(players[j].socket);
				s << 1+4+4;
//				s << MSG_GET << items[i].itemNo;
				s << MSG_GET << players[j].id << items[i].id;
				players[j].socket->flush();

				if (items[i].itemNo==0) players[j].health=1;
				items[i]=items.back();
				items.pop_back();
			} else ++i;
		}
	}

	/*
	QByteArray stateMsg;
	QDataStream stream(&stateMsg, QIODevice::WriteOnly);
    stream << 1 + 4 + items.size()*(8+8+4);
    stream << MSG_ITEM << items.size();

    for(int i = 0; i < items.size(); ++i) {
        Item& it = items[i];
        stream << it.x << it.y << it.itemNo;
    }
	sendToAll(stateMsg);
	*/
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
//			qDebug()<<"removing bullet"<<bullets[i].x<<bullets[i].y;
			sendHit(bullets[i]);
			bullets[i] = bullets.back();
			bullets.pop_back();
		} else ++i;
	}
	for(int i=0; i<players.size(); ++i)
		if (players[i].health<=0) {
			char tmp[] = {0,0,0,1,MSG_DIE};
			players[i].socket->write(tmp,5);
			players[i].socket->flush();
			spawnPlayer(players[i],0);
			++players[i].deaths;

			Player* shooter = players[i].lastShooter;
			if (shooter) ++shooter->kills;
		}
	for(int i=0; i<bots.size(); ) {
//		if (bots[i].health<=0) spawnPlayer(bots[i],1);
		if (bots[i].health<=0) {
			Player* shooter = bots[i].lastShooter;
			if (shooter) ++shooter->kills;
//			qDebug()<<"killed by"<<bots[i].lastShooter;

			bots[i]=bots.back();
			bots.pop_back();
		} else ++i;
	}
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


	s << 1+4+items.size()*(8+8+4+4);
	s << MSG_ITEM << items.size();
	for(int i=0; i<items.size(); ++i) {
		Item& it = items[i];
		s<<it.x<<it.y<<it.itemNo<<it.id;
	}
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
}

void Server::sendHit(const Bullet& b)
{
	QByteArray msg;
	QDataStream s(&msg, QIODevice::WriteOnly);

	s << int(1+sizeof(b.id)+8+8);
	s << MSG_HIT << b.id << b.x<<b.y;

	sendToAll(msg);
}
void Server::sendStats()
{
	QByteArray msg;
	QDataStream s(&msg, QIODevice::WriteOnly);

	int lens=0;
	for(int i=0; i<players.size(); ++i) lens+=players[i].name.size();
	qDebug()<<"stats"<<lens;
	s << 1+4+2*lens+players.size()*(4+4+4+8+4);
	s << MSG_STATS << players.size();
	for(int i=0; i<players.size(); ++i) {
		Player& p = players[i];
		qDebug()<<"name"<<p.name.size()<<p.name;
		s << p.id << p.kills << p.deaths << p.damageDone << p.name.size();// << p.name;
		s.writeRawData((char*)p.name.data(), 2*p.name.size());
	}

	sendToAll(msg);
}

void Server::rocketDamage(Unit& u, const Bullet& b)
{
	double dx=u.x-b.x, dy=u.y-b.y;
	double d2 = dx*dx + dy*dy;
	if (d2 > ROCKET_RADIUS*ROCKET_RADIUS) return;
	if (rayHitsWall(b.x,b.y,u.x,u.y,area)) return;
	double d = sqrt(d2);
	double dmg = damages[b.type] * (1-d/ROCKET_RADIUS);
	u.health -= dmg / u.armor;
	if (b.shooter) b.shooter->damageDone += dmg/u.armor;
	u.lastShooter = b.shooter;
}
void Server::bulletHit(Unit* p, const Bullet& b)
{
	if (b.type==5) {
		for(int i=0; i<players.size(); ++i)
			rocketDamage(players[i], b);
		for(int i=0; i<bots.size(); ++i)
			rocketDamage(bots[i], b);
		return;
	}
	if (p==0) return;

	p->health -= damages[b.type] / p->armor;
	p->lastHitT = curT.elapsed();
	if (b.shooter) b.shooter->damageDone += damages[b.type] / p->armor, qDebug()<<"adding for"<<b.shooter->id<<damages[b.type]/p->armor;
	p->lastShooter = b.shooter;
	qDebug()<<"hit"<<p->health;
}

void Server::spawnPlayer(Unit& p, bool bot)
{
	QPair<int,int> spawn = area.getSpawnPoint(curSpawn+bot);
	p.x = spawn.first+.5;
	p.y = spawn.second+.5;
	p.health = 1;
	p.angle = bot ? M_PI/2 : -M_PI/2;
}

void Server::addBullet(int weap, double x, double y, double vx, double vy, double v, Player* pl)
{
	vx *= v, vy*=v;
	Bullet bullet(bulletID++,weap,x,y,vx,vy,pl);
	bullets.append(bullet);

	QByteArray msg;
	QDataStream os(&msg, QIODevice::WriteOnly);
	os << 1 + 4+4 + 4*8;
	os << MSG_SHOOT << bullet.id << weap;
	os << x<<y<<vx<<vy;
	sendToAll(msg);
}
void Server::createBot(int place, int w)
{
	if (bots.size()>=area.maxBots[curSpawn]) return;
	if (place>15 && curSpawn<15) place=15;
	QPair<int,int> spawn = area.getSpawnPoint(place);
	Bot b(spawn.first + .5, spawn.second+.5, w);
	bots.append(b);
}
Item& Server::createItem(int type)
{
	QPair<int,int> spawn = area.getSpawnPoint(curSpawn);
	Item i(spawn.first + .5, spawn.second+.5, type, nextItem++);
	items.append(i);
	qDebug()<<"item created"<<type;
	return items.back();
}
void Server::spawnStuff(bool next)
{

	lastSpawn = curT.elapsed();
	double s = players.size();
	if (!s) s=1;
	s = sqrt(s);

	int itemCount=0;
	for(int i=0; i<6; ++i) itemCount += area.itemCounts[i][curSpawn]*s;

	QByteArray msg;
	QDataStream os(&msg, QIODevice::WriteOnly);
	os << 1+4+itemCount*(8+8+4+4);
	os << MSG_ITEM << itemCount;

	for(int j=0; j<6; ++j) {
		qDebug()<<"spawning"<<area.spawnCounts[j][curSpawn]*s<<"bots ;"<<next;
		for(int i=0; i<area.spawnCounts[j][curSpawn] * s; ++i)
			createBot(curSpawn+1+next, j);
		if (area.itemCounts[j][curSpawn]) qDebug()<<"creating"<<j<<area.itemCounts[j][curSpawn];
		for(int i=0; i<area.itemCounts[j][curSpawn] * s; ++i) {
			Item& i = createItem(j);
			os << i.x << i.y << i.itemNo << i.id;
		}
	}

	sendToAll(msg);
}
void Server::lightningDamage(Unit& shooter, Unit& pl, QList<QPointF>& pts, Player* player)
{
	double dx=shooter.x-pl.x;
	double dy=shooter.y-pl.y;
	if (dx*dx + dy*dy > LIGHTNING_RADIUS*LIGHTNING_RADIUS) return;

	pts.append(QPointF(pl.x,pl.y));
	pl.health -= damages[4] / pl.armor;
	if (player) player->damageDone += damages[4] / pl.armor;
	pl.lastShooter = player;
//	qDebug()<<"lightning damage"<<pl.health;
}
void Server::hitLightning(Unit& u)
{
	QList<QPointF> pts;
	pts.append(QPointF(u.x,u.y));
	Player* player = getPlayer(u.id);
	for(int i=0; i<players.size(); ++i)
		if (&u!=&players[i]) lightningDamage(u, players[i], pts, player);
	for(int i=0; i<bots.size(); ++i)
		if (&u!=&bots[i]) lightningDamage(u, bots[i], pts, player);
//	qDebug()<<"Mui."<<pts.size();

	QByteArray msg;
	QDataStream s(&msg, QIODevice::WriteOnly);
	s << 1 + 4 + pts.size()*(8+8);
	s << MSG_LIGHTNING << pts.size();
	for(int i=0; i<pts.size(); ++i)
		s << pts[i].x() << pts[i].y();
	sendToAll(msg);
}
