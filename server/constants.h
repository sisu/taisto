#ifndef CONSTANTS_H
#define CONSTANTS_H

/** Squares per second. */
const double MOVE_SPEED = 3.0;

/** Radians per second. */
const double TURN_SPEED = 2.5;

const double PLAYER_RADIUS = 0.25;
const double ITEM_RADIUS = 0.25;

const int FPS = 50;
const double FRAME_TIME = 1./FPS;

const int WEAPONS = 16;
const int loadTimes[1+WEAPONS] = {0,300,350,50};
const double bulletSpeeds[1+WEAPONS] = {0,20,40,50};
const double damages[1+WEAPONS] = {0,0.07,0.07,0.04};

#endif
