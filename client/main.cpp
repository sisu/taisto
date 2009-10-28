#include <QApplication>
#include "game.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Game game;
    qDebug()<<argc;
    if(argc==3) game.start(argv[1],atoi(argv[2]));
    else if(argc==2) game.start(argv[1],32096);
    else game.start("127.0.0.1",32096);
    return app.exec();
}
