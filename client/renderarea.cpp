#include <QtGui>
#include "renderarea.h"
#define RADIUS 20
RenderArea::RenderArea(QWidget *parent): QWidget(parent)
{

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    width=this->size().width();
    height=this->size().height();
    qDebug()<<width<<height;
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing,true);
    //painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(90,240,90)));
    for(int i=0;i<players.size();i+s+) {
        double x=width/2 + centerx - players[i].x;
        double y=height/2 +centery - players[i].y;
        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(90,240,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(190,140,90)));
            painter.drawEllipse(x+(RADIUS-8)*sin(players[i].direction),y+(RADIUS-8)*cos(players[i].direction),3,3);
        }

    }
    for(int i=0;i<bots.size();i++) {
        double x=width/2 + centerx - players[i].x;
        double y=height/2 +centery - players[i].y;
        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(190,120,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(190,140,90)));
            painter.drawEllipse(x+(RADIUS-8)*sin(bots[i].direction),y+(RADIUS-8)*cos(bots[i].direction),3,3);
        }
    }
    for(int i=0;i<bots.size();i++) {
        painter.setBrush(QBrush(QColor(190,120,90)));
        painter.drawEllipse(bots[i].x-RADIUS,bots[i].y-RADIUS,RADIUS*2,RADIUS*2);
        painter.setBrush(QBrush(QColor(190,140,90)));
        painter.drawEllipse(bots[i].x+(RADIUS-8)*sin(bots[i].direction),bots[i].y+(RADIUS-8)*cos(bots[i].direction),3,3);

    }
}


void RenderArea::draw(double x, double y, QList<Object>& _players, QList<Object>& _bots) {
    centerx=x;
    centery=y;
    players=_players;
    bots=_bots;
    update();
}

