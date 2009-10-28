#include "connection.h"
#include "messages.h"

Connection::Connection(Player* obj, Engine& e): player(obj), engine(e) {
	packetSize=-1;
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
		}
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
	s>>player->id;
	qDebug()<<"got player id"<<player->id;
}
void Connection::readState(QDataStream& s)
{
	engine.players.clear();

	int pl;
	s>>pl;
//	qDebug()<<"players"<<pl;
	for(int i=0; i<pl; ++i) {
		Object pl;
		s>>pl.id>>pl.x>>pl.y>>pl.direction>>pl.my>>pl.mx>>pl.turn;
//		qDebug()<<pl.x<<pl.y<<pl.my<<pl.mx;
		engine.players.append(pl);
//		qDebug()<<pl.id<<player->id;
		if (pl.id==player->id) {
			player->x = pl.x;
			player->y = pl.y;
			player->direction = pl.direction;
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
