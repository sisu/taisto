#include "player.h"

Player::Player(double x, double y)
{
    
    this->x=x;
    this->y=y;
}

void Player::draw(QPainter& painter) {
    painter.setBrush(QBrush(QColor(90,240,90)));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(x,y,radius,radius);
}

void Player::move(double t) {

}


