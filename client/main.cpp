#include <QApplication>
#include <QMessageBox>
#include "game.h"
#include "setupdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //Game game;
    qDebug()<<argc;

	/*
    if(argc==3) game.start(argv[1],atoi(argv[2]));
    else if(argc==2) game.start(argv[1],32096);
    else game.start();
	*/

	QString addr;
	SetupDialog* setup = new SetupDialog;
	if(setup->exec())
	{
//setup->deleteLater();
		qDebug()<<"start()\n";
		addr = setup->hostAddress();
	}else
	{
		return 0;
	}

	Game game;
	game.start(addr,32096);

    return app.exec();
}
