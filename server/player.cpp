#include "player.h"
#include "constants.h"
#include <cmath>

const double FRAME_TIME = 0.02;

void Player::update()
{
	while(socket->bytesAvailable()) {
		handleMessage();
	}

	double ca = cos(angle);
	double sa = sin(angle);
	const double spd = MOVE_SPEED * FRAME_TIME;
	x += moveForward*ca * spd;
	y -= moveForward*sa * spd;
	x += moveSide*sa * spd;
	y -= moveSide*ca * spd;

	angle += turn * TURN_SPEED * FRAME_TIME;
}
void Player::handleMessage()
{
	char type;
	socket->getChar(&type);
	switch(type) {
		default:
			break;
	}
}
