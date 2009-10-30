#include <QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include "game.h"
#include "setupdialog.h"

int main(int argc, char *argv[])
{ 
	QCoreApplication::setOrganizationName("MikkoMikaJussiSoft");
//QCoreApplication::setOrganizationDomain("sisu.com");
	QCoreApplication::setApplicationName("Taisto");

	QApplication app(argc, argv);
	Game game;
	qDebug()<<argc;

	if(argc==3) game.start(argv[1],atoi(argv[2]));
	else if(argc==2) game.start(argv[1],32096);
	else game.start();



	return app.exec();
}
