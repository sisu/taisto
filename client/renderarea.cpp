#include <QtGui>
#include "renderarea.h"
#define RADIUS 20
#define SQUARE 10
RenderArea::RenderArea(Engine& _engine,QWidget* parent): engine(_engine), QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    width=this->size().width();
    height=this->size().height();
//    qDebug()<<width<<height;
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing,true);
    //painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(90,240,90)));
    for(int i=0;i<engine.players.size();i++) {
		double px = engine.players[i].x * SQUARE, py = engine.players[i].y * SQUARE;
        double x=width/2 - centerx + px;
        double y=height/2 - centery + py;
        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(90,240,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(190,140,90)));
            painter.drawEllipse(x+(RADIUS-8)*sin(engine.players[i].direction),y+(RADIUS-8)*cos(engine.players[i].direction),3,3);
        }

    }
#if 0
    for(int i=0;i<bots.size();i++) {
        double x=width/2 + centerx - players[i].x;
        double y=height/2 +centery - players[i].y;
        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(190,120,90)));
            painter.drawEllipse(x*SQUARE-RADIUS,y*SQUARE-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(190,140,90)));
            painter.drawEllipse(x*SQUARE+(RADIUS-8)*sin(bots[i].direction),y*SQUARE+(RADIUS-8)*cos(bots[i].direction),3,3);
        }
    }
#endif
}


void RenderArea::draw(double x, double y) {
    centerx=(x)*SQUARE;
    centery=(y)*SQUARE;
    update();
}

