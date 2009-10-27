#ifndef CONNECTION_H
#define CONNECTION_H
#include<QString>
#include<QObject>
#include <QTcpSocket>
#include <QDataStream>
#include "object.h"
#include "engine.h"

class Connection : public QTcpSocket {
	Q_OBJECT
public:
	Connection(Object* pl);
	void connect(QString);
	void update(Engine& e);

private:
	Object* player;

	void readInitial(QDataStream& s);
	void readState(QDataStream& s, Engine& e);
};
#endif
