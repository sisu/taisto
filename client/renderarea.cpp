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
    //    qDebug()<<width<<height;
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing,true);
	painter.translate(0, height);
	painter.scale(1,-1);
    //painter.setPen(Qt::NoPen);

    //Borders
	painter.setBrush(QBrush(QColor(40,40,40)));

    if(centerx-width/2<0) {
        double over=width/2-centerx;
        painter.drawRect(0,0,(int)(over),(int)(height));
    }
   // qDebug()<<centerx+width/2<<engine.area.w*SQUARE;
    //   qDebug()<<centerx<<centery;
//    painter.drawEllipse(centerx+width/2,100,10,10);
    if(centerx+width/2>engine.area.w*SQUARE) {
        double over=centerx+width/2-engine.area.w*SQUARE;
        painter.drawRect((int)(width-over),0,(int)(over),(int)(height));
    }




    //Boxes

	/*
    double sx= centerx/SQUARE;
    double sy= centery/SQUARE;
    int topx=max(0,(int)( sx - width/2/SQUARE));
    int topy=max(0,(int)(sy-height/2/SQUARE));
    int botx=min(engine.area.w-1,(int)(sx+width/2/SQUARE));
    int boty=min(engine.area.data.size()/engine.area.w-1,(int)(sy+height/2/SQUARE));
    qDebug()<<"Piirretään "<<topx<<topy<<botx<<boty;
    painter.setBrush(QBrush(Qt::NoBrush));
    for(int xa=topx;xa<=botx;xa++) {
        for(int ya=topy;ya<=boty;ya++) {
        if(engine.area.data[ya*engine.area.w+xa]!=0||true) painter.drawRect(
                (xa-topx)*SQUARE + (sx-(int)(sx))*SQUARE ,
                (ya-topy)*SQUARE + (sy-(int)(sy))*SQUARE ,
                SQUARE,SQUARE);
        }
    }*/
//	qDebug()<<centerx<<centery;
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

    /*
       qDebug()<<"Topleft "<<max(0,(int)((centerx-width/2)/SQUARE))<<max(0,(int)((centery-height/2)/SQUARE));
       qDebug()<<"Btmright "<<(int)((centerx+width/2)/SQUARE)<<(int)((centery+height/2)/SQUARE);
       for(int xa=max(0,(int)((centerx-width/2)/SQUARE));xa<=(int)(1+(centerx+width/2)/SQUARE);xa++) {
       for(int ya=max((int)((centery-height/2)/SQUARE),0);ya<=(int)(1+(centery+height/2)/SQUARE);ya++) {
       painter.drawRect(xa*SQUARE,ya*SQUARE,SQUARE,SQUARE);
       }
       }*/




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
}


void RenderArea::draw(double x, double y) {
    centerx=(x)*SQUARE;
    centery=(y)*SQUARE;
    update();
}

