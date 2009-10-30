#include <cstdlib>
#include "connection.h"
#include "messages.h"

Connection::Connection(Player* obj, Engine& e): player(obj), engine(e) {
	packetSize=-1;
	packetCount=0;
}

void Connection::connect(QString ip, int port) {
	qDebug()<<"Connecting..."<<ip<<port;
    
	connectToHost(ip, port);
	bool res = waitForConnected(1000);
	qDebug()<<"result"<<res;
}
void Connection::update()
{
	while(bytesAvailable()) {
		QDataStream s(this);
		if (packetSize<0) {
			if (bytesAvailable()<4) break;
			s>>packetSize;
			if (packetSize>1<<20) qDebug()<<"big packet"<<packetSize;
		}
		if (bytesAvailable()<packetSize) break;
//		qDebug()<<"packet size"<<packetSize;
		packetSize=-1;
		quint8 type;
		s>>type;
//		qDebug()<<"available: "<<type;
		switch(type) {
			case MSG_INITIAL:
				readInitial(s);
				break;
			case MSG_STATE:
				readState(s);
				break;
			case MSG_SHOOT:
				readShoot(s);
				break;
			case MSG_HIT:
				readHit(s);
				break;
			case MSG_ENEMY:
			    readEnemy(s);
			    break;
			case MSG_ITEM:
			    readItem(s);
			    break;
			case MSG_GET:
				readGet(s);
				break;
			case MSG_DIE:
				for(int i=0; i<engine.bulletCounts.size(); ++i) engine.bulletCounts[i]=0;
				player->weapon = 1;
				break;
			default:
				qDebug()<<type;
				abort();
		}
		++packetCount;
	}
}
void Connection::readInitial(QDataStream& s)
{
	int w,h;
	s>>w>>h;
	engine.area.w = w;
	qDebug()<<"map size"<<w<<h;
	for(int i=0; i<h; ++i) {
		for(int j=0; j<w; ++j) {
			int a;
			s>>a;
			engine.area.data.append(a);
		}
	}
	int p,b;
    s>>p>>b;
    engine.area.part=p;
    engine.area.spawn=b;
	s>>player->id;
	qDebug()<<"got player id"<<player->id;
}
void Connection::readState(QDataStream& s)
{
	engine.players.clear();

	int p;
	s>>p;
//	qDebug()<<"players"<<pl;
	for(int i=0; i<p; ++i) {
		Player pl;
		s>>pl.id>>pl.x>>pl.y>>pl.direction>>pl.my>>pl.mx>>pl.turn>>pl.health;
//		qDebug()<<pl.x<<pl.y<<pl.my<<pl.mx;
		engine.players.append(pl);
//		qDebug()<<pl.id<<player->id;
		if (pl.id==player->id) {
			player->x = pl.x;
			player->y = pl.y;
			player->direction = pl.direction;
			player->health = pl.health;
		}
	}
}
void Connection::readShoot(QDataStream& s)
{
	int id,weapon;
	double x,y,vx,vy;
	s>>id>>weapon>>x>>y>>vx>>vy;

    engine.bullets.insert(id,Bullet(weapon,x,y,vx,vy));
	qDebug()<<"got shoot"<<weapon<<x<<y<<vx<<vy;
}
void Connection::readHit(QDataStream& s)
{
	int id;
	double x,y;
	s>>id>>x>>y;
	if (engine.bullets.find(id)->weapon==5) {
		engine.explosions.append(QPointF(x,y));
	}
	engine.bullets.remove(id);
}
void Connection::readEnemy(QDataStream& s) {
	engine.bots.clear();

	int p;
	s>>p;
	for(int i=0; i<p; ++i) {
		Player pl;
		s>>pl.x>>pl.y>>pl.direction>>pl.my>>pl.mx>>pl.turn>>pl.health>>pl.weapon;
//		qDebug()<<"aaa"<<pl.x<<pl.y;
		engine.bots.append(pl);
	}
}

void Connection::readItem(QDataStream& s) {
    engine.items.clear();

	int p;
	s>>p;
	for(int i=0; i<p; ++i) {
		Item it;
		s>>it.x>>it.y>>it.itemNo;
		engine.items.append(it);
	}
}
const int boxSizes[20] = {0,15,50,1,2};
void Connection::readGet(QDataStream& s) {
	int item;
	s>>item;
	if (item>0) engine.bulletCounts[item] += boxSizes[item];
}

void Connection::sendStatus()
{
//	qDebug()<<"send"<<player->x<<player->y<<player->my<<player->mx;
	QDataStream s(this);
	s << 1 + 4+4+4;
	s << MSG_STATE;
	s<<player->my<<player->mx<<player->turn;
	flush();
}
void Connection::sendShoot()
{
	qDebug()<<"sending shoot";
	QDataStream s(this);
	s << 1 + 4;
	s << MSG_SHOOT;
	s << player->weapon;
	flush();
}
