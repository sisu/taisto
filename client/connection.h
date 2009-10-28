#ifndef CONNECTION_H
#define CONNECTION_H
#include<QString>
#include<QObject>
#include <QTcpSocket>
#include <QDataStream>
#include "object.h"
#include "engine.h"
#include "area.h"

class Connection : public QTcpSocket {
	Q_OBJECT
public:
	Connection(Object* pl, Area& a);
	void connect(QString);
	void update(Engine& e);
	void sendStatus();

private:
	Object* player;
	Area& area;

	void readInitial(QDataStream& s);
	void readState(QDataStream& s, Engine& e);
	int packetSize;
};
#endif
