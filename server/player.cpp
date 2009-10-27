#include "player.h"
#include "constants.h"
#include "messages.h"
#include <cmath>

const double FRAME_TIME = 0.02;

void Player::update()
{
	while(socket->canReadLine()) {
		handleMessage(socket->readLine());
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
void Player::handleMessage(QByteArray msg)
{
	char type = msg[0];
	switch(type) {
		default:
			break;
	}
}
