#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include "server.h"

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	Server s(16);
	s.listen(QHostAddress::Any, 32096);

	return a.exec();
}
