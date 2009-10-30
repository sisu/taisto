#ifndef CONSTANTS_H
#define CONSTANTS_H
#include<QColor>
/** Squares per second. */
const double MOVE_SPEED = 3.0;

/** Radians per second. */
const double TURN_SPEED = 2.5;

const double PLAYER_RADIUS = 0.25;
const double ITEM_RADIUS = 0.25;

const int FPS = 50;
const double FRAME_TIME = 1./FPS;

const int WEAPONS = 16;
const int loadTimes[1+WEAPONS] = {0,300,350,50,0,1000};
const double bulletSpeeds[1+WEAPONS] = {0,20,40,50,0,20};
const double damages[1+WEAPONS] = {0,0.07,0.07,0.04,0.02,5};

const double ROCKET_RADIUS = 4;
const double LIGHTNING_RADIUS = 5;

const QColor player_colors[12] = {
    QColor(255,0,0),
    QColor(0,255,0),
    QColor(0,0,255),
    QColor(255,255,0),
    QColor(255,0,255),
    QColor(0,255,255),
    QColor(255,255,255),
    QColor(0,125,255),
    QColor(255,125,0),
    QColor(125,0,255),
    QColor(255,0,125),
    QColor(125,255,0)
};

#endif
