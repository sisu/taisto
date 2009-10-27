#include "connection.h"
#include "messages.h"

Connection::Connection(Object* obj): player(obj) {
}

void Connection::connect(QString ip) {
	qDebug()<<"Connecting...";
	connectToHost(ip, 32096);
	bool res = waitForConnected(1000);
	qDebug()<<"result"<<res;
}
void Connection::update(Engine& e)
{
	while (bytesAvailable()) {
		QDataStream s(this);
		quint8 type;
		s>>type;
		qDebug()<<"available: "<<type;
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
	qDebug()<<w<<h;
	for(int i=0; i<h; ++i) {
		for(int j=0; j<w; ++j) {
			int a;
			s>>a;
		}
	}
}
void Connection::readState(QDataStream& s, Engine& e)
{
	e.players.clear();

	int pl;
	s>>pl;
	for(int i=0; i<pl; ++i) {
		Object pl;
		s>>pl.id>>pl.x>>pl.y>>pl.direction>>pl.my>>pl.mx>>pl.turn;
		e.players.append(pl);
	}
}
