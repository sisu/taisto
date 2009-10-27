#include "connection.h"
#include "messages.h"

Connection::Connection(Object* obj): player(obj) {
}

void Connection::connect(QString ip) {
	socket.connectToHost(ip, 32096);
}
void Connection::update()
{
	while (socket.bytesAvailable()) {
		QDataStream s(&socket);
		quint8 type;
		s>>type;
		switch(type) {
			case MSG_INITIAL:
				break;
		}
	}
}
