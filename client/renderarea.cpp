#include <QtGui>
#include "renderarea.h"
#include "constants.h"
#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif
#define SQUARE 20
#define RADIUS (PLAYER_RADIUS*SQUARE)
const double EYE_SIZE = SQUARE*PLAYER_RADIUS*0.3;
const double EYE_DIST = SQUARE*PLAYER_RADIUS*0.1;
RenderArea::RenderArea(Engine& _engine, QWidget* parent): engine(_engine), QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    width=this->size().width();
    height=this->size().height();
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing,true);
    
	painter.setBrush(QBrush(QColor(220,210,180)));
    painter.drawRect(0,0,width,height);    
	painter.translate(0, height);
	painter.scale(1,-1);
    
    
    //Borders
	painter.setBrush(QBrush(QColor(40,40,40)));

    if(centerx-width/2<0) {
        double over=width/2-centerx;
        painter.drawRect(0,0,(int)(over),(int)(height));
    }
    if(centerx+width/2>engine.area.w*SQUARE) {
        double over=centerx+width/2-engine.area.w*SQUARE;
        painter.drawRect((int)(width-over),0,(int)(over),(int)(height));
    }




    //Boxes

	Area& a = engine.area;
	double w2=width/2, h2=height/2;
	int startx = max(0, (centerx-w2)/SQUARE);
	int endx = min(a.w-1, (centerx+w2)/SQUARE);
	int starty = max(0, (centery-h2)/SQUARE);
	int endy = min(a.h()-1, (centery+h2)/SQUARE);
	for(int y=starty; y<=endy; ++y) {
		for(int x=startx; x<=endx; ++x) {
			if (a.data[y*a.w+x]) {
				double x0 = w2 + x*SQUARE - centerx;
				double y0 = h2 + y*SQUARE - centery;
				painter.drawRect(x0, y0, SQUARE, SQUARE);
			}
		}
	}

    //Player
    for(int i=0;i<engine.players.size();i++) {
        double px = engine.players[i].x * SQUARE, py = engine.players[i].y * SQUARE;
        double x=width/2 - centerx + px;
        double y=height/2 - centery + py;
        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(90,240,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(190,140,90)));
			double a=engine.players[i].direction;
            painter.drawEllipse(
					x+(RADIUS-EYE_DIST-EYE_SIZE)*cos(a)-EYE_SIZE,
					y+(RADIUS-EYE_DIST-EYE_SIZE)*-sin(a)-EYE_SIZE,
					2*EYE_SIZE,
					2*EYE_SIZE);
        }

    }
    //Bullets
    QMap<int,Bullet>::Iterator it;
    painter.setBrush(QBrush(QColor(220,40,35)));
    for(it = engine.bullets.begin(); it != engine.bullets.end(); it++) {
        double px = it.value().x * SQUARE, py = it.value().y * SQUARE;

        double x = width/2 - centerx + px;
        double y = height/2 - centery + py;

        painter.drawEllipse(x-2,y-2,4,4);
    }

}


void RenderArea::draw(double x, double y) {
    centerx=(x)*SQUARE;
    centery=(y)*SQUARE;
    update();
}




