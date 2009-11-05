#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include "server.h"

int main(int argc, char* argv[])
{
	srand(QTime::currentTime().msec() + 1000*QDateTime::currentDateTime().toTime_t());
	QCoreApplication a(argc, argv);
	Server s(17);
	s.listen(QHostAddress::Any, 8200);

	return a.exec();
}
