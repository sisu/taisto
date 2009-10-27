#ifndef PLAYER_H
#define PLAYER_H
#include<QPainter>
class Player
{
public:
    double x,y,radius;
    Player(double x, double y);
    void draw(QPainter &);
    void move(double t);
};

#endif // PLAYER_H
