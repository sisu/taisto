#include <QtGui>
#include "renderarea.h"
#define RADIUS 60
RenderArea::RenderArea(QWidget *parent): QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    painter.setRenderHint(painter.Antialiasing,true);
    //painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(90,240,90)));
    for(int i=0;i<players.size();i++) {
        painter.setBrush(QBrush(QColor(90,240,90)));
        painter.drawEllipse(players[i].x-RADIUS,players[i].y-RADIUS,RADIUS*2,RADIUS*2);
        painter.setBrush(QBrush(QColor(190,140,90)));
        painter.drawEllipse(players[i].x+(RADIUS-8)*sin(players[i].direction),players[i].y+(RADIUS-8)*cos(players[i].direction),3,3);

    }
    for(int i=0;i<bots.size();i++) {
        painter.setBrush(QBrush(QColor(190,120,90)));
        painter.drawEllipse(bots[i].x-RADIUS,bots[i].y-RADIUS,RADIUS*2,RADIUS*2);
        painter.setBrush(QBrush(QColor(190,140,90)));
        painter.drawEllipse(bots[i].x+(RADIUS-8)*sin(bots[i].direction),bots[i].y+(RADIUS-8)*cos(bots[i].direction),3,3);

    }
}


void RenderArea::draw(QList<Object>& _players, QList<Object>& _bots) {
    players=_players;
    bots=_bots;
    update();
}


