#ifndef OBJECT_H
#define OBJECT_H
#define PLR 1
#define BOT 2

#include <QObject>

class Object : public QObject {
    Q_OBJECT
    public:
        int type;
        int id;
        double x,y;
        double direction;
        int mx, my;
        Object(int, int,double, double, double);

};




#endif
