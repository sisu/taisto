#ifndef CONNECTION_H
#define CONNECTION_H
#include<QString>
#include<QObject>
#include <QTcpSocket>
#include <QDataStream>
#include "object.h"

class Connection : public QTcpSocket {
	Q_OBJECT
public:
	Connection(Object* pl);
	void connect(QString);
	void update();

private:
	Object* player;

	void readInitial(QDataStream& s);
};
#endif
