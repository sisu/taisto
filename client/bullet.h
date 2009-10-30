#ifndef BULLET_H
#define BULLET_H
#include <cmath>
struct Bullet {
    int weapon;
    double x,y,vx,vy;
    double dir;
    double dirdeg;
    Bullet(int _weapon, double _x, double _y, double _vx, double _vy) {
        weapon=_weapon;
        x=_x;
        y=_y;
        vx=_vx;
        vy=_vy;
        if(vx==0) dir=0;
        else dir=atan(vy/vx)+3.141/2;
        if(vx<0) dir+=3.141;
        dirdeg=dir/3.141*180;
    }

};
#endif
