#include "connection.h"
#include "messages.h"

Connection::Connection(Object* obj, Area& a): player(obj), area(a) {
	packetSize=-1;
}

void Connection::connect(QString ip) {
	qDebug()<<"Connecting...";
	connectToHost(ip, 32096);
	bool res = waitForConnected(1000);
	qDebug()<<"result"<<res;
}
void Connection::update(Engine& e)
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
				readState(s, e);
				break;
		}
	}
}
void Connection::readInitial(QDataStream& s)
{
	int w,h;
	s>>w>>h;
	area.w = w;
	qDebug()<<"map size"<<w<<h;
	for(int i=0; i<h; ++i) {
		for(int j=0; j<w; ++j) {
			int a;
			s>>a;
			area.data.append(a);
		}
	}
	s>>player->id;
	qDebug()<<"got player id"<<player->id;
}
void Connection::readState(QDataStream& s, Engine& e)
{
	e.players.clear();

	int pl;
	s>>pl;
//	qDebug()<<"players"<<pl;
	for(int i=0; i<pl; ++i) {
		Object pl;
		s>>pl.id>>pl.x>>pl.y>>pl.direction>>pl.my>>pl.mx>>pl.turn;
//		qDebug()<<pl.x<<pl.y<<pl.my<<pl.mx;
		e.players.append(pl);
//		qDebug()<<pl.id<<player->id;
		if (pl.id==player->id)
			*player = pl;
	}
}
void Connection::sendStatus()
{
//	qDebug()<<"send"<<player->x<<player->y<<player->my<<player->mx;
	QDataStream s(this);
	s << 1 + 8 + 4+4+4;
	s << MSG_STATE;
	s<<player->direction<<player->my<<player->mx<<player->turn;
}
