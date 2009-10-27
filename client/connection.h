#ifndef CONNECTION_H
#define CONNECTION_H
#include<QString>
#include<QObject>
#include <QTcpSocket>
#include "object.h"

class Connection : public QObject {
	Q_OBJECT
public:
	Connection(Object* pl);
	void connect(QString);
	void update();

private:
	Object* player;
	QTcpSocket socket;
};
#endif
