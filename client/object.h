#ifndef OBJECT_H
#define OBJECT_H
#define PLR 1
#define BOT 2

class Object {
    public:
        int type;
        int id;
        double x,y;
        double direction;
        int mx, my, turn;
        Object(int, int,double, double, double);
		Object() {}

};




#endif
