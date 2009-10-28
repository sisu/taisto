#ifndef BULLET_H
#define BULLET_H

struct Bullet {
    int weapon;
    double x,y,vx,vy;
    Bullet(int _weapon, double _x, double _y, double _vx, double _vy) {
        weapon=_weapon;
        x=_x;
        y=_y;
        vx=_vx;
        vy=_vy;
    }

};
#endif
