#include <QtGui>
#include "renderarea.h"
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
        painter.drawEllipse(players[i]->x,players[i]->y,60,60);
    }
    painter.setBrush(QBrush(QColor(190,120,90)));
    for(int i=0;i<bots.size();i++) {
        painter.drawEllipse(players[i]->x,players[i]->y,60,60);
    }
}


void RenderArea::draw(QList<Object*>& _players, QList<Object*>& _bots) {
    players=_players;
    bots=_bots;
    update();
}


