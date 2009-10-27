#include <QCoreApplication>
#include "server.h"

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	Server s(20);
	s.listen(QHostAddress::Any, 32096);
	qDebug()<<"jee";
	return a.exec();
}
