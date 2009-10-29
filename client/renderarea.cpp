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
RenderArea::RenderArea(Engine& _engine, QWidget* parent): QGLWidget(QGLFormat(QGL::SampleBuffers),parent), engine(_engine), player(NULL)
//RenderArea::RenderArea(Engine& _engine, QWidget* parent): QWidget(parent), engine(_engine), player(NULL)
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
    painter.setPen(Qt::NoPen);

    Area& a = engine.area;
    double w2=width/2, h2=height/2;
    int startx = max(0, (centerx-w2)/SQUARE);
    int endx = min(a.w-1, (centerx+w2)/SQUARE);
    int starty = max(0, (centery-h2)/SQUARE);
    int endy = min(a.h()-1, (centery+h2)/SQUARE);
    for(int y=starty; y<=endy; ++y) {
        for(int x=startx; x<=endx; ++x) {
            if (a.data[y*a.w+x]) {
                painter.setBrush(QBrush(QColor(70,40,40)));
//                painter.setPen(QPen(QColor(0,0,0)));

                double x0 = w2 + x*SQUARE - centerx;
                double y0 = h2 + y*SQUARE - centery;
                painter.drawRect(x0, y0, SQUARE, SQUARE);
            } else if(y%a.part<a.spawn) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(QBrush(QColor(230,250,230)));
                double x0 = w2 + x*SQUARE - centerx;
                double y0 = h2 + y*SQUARE - centery; painter.drawRect(x0,y0,SQUARE,SQUARE); }
        }
    }
    painter.setPen(QPen(QColor(0,0,0)));

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

    for(int i=0;i<engine.bots.size();i++) {
//		qDebug()<<"drawing bot"<<engine.bots[i].x<<engine.bots[i].y;
        double px = engine.bots[i].x * SQUARE, py = engine.bots[i].y * SQUARE;
        double x=width/2 - centerx + px;
        double y=height/2 - centery + py;

        painter.setBrush(QBrush(QColor(255,0,0)));
        painter.drawRect(x-5, y+5, 10*engine.bots[i].health, 3);

        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(190,140,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(90,240,90)));
			double a=engine.bots[i].direction;
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

    QList<QString> weaponNames;
    weaponNames << "Bead Gun" << "Shotgun" << "Machine Gun" << "Electrogun" << "Rocket Launcher";

    QList<QColor> weaponColors;
    weaponColors.append(QColor(60,220,155));
    weaponColors.append(QColor(0,125,225));
    weaponColors.append(QColor(225,185,0));
    weaponColors.append(QColor(210,30,105));
    weaponColors.append(QColor(230,120,100));

	//Items
	for(int i=0; i<engine.items.size(); ++i) {
		Item& it = engine.items[i];
		QColor c(255,255,255);
		if (it.itemNo>0) c=weaponColors[it.itemNo-1];
		painter.setBrush(QBrush(c));

		double x = width/2 - centerx + it.x*SQUARE;
		double y = width/2 - centery + it.y*SQUARE;
		int s=5;
		painter.drawRect(x-s,y-s,2*s,2*s);
	}

    /*
    QList<QPoint> lPoints;
    lPoints<<QPoint(100,100)<<QPoint(100,150)<<QPoint(200,120)<<QPoint(125,175)<<QPoint(75,200);
    drawLightning(painter,lPoints);
    */

	//Statusbar
	painter.resetTransform();

	int statusBarHeight = 20;
	int statusBarFromBottom = 5;
	int statusBarY = height - statusBarHeight - statusBarFromBottom;

    painter.setBrush(QBrush(QColor(255,255,255)));
    painter.drawRect(5, statusBarY, width - 10, statusBarHeight);

    // Health bar

    if(player != NULL) {
        painter.setBrush(QBrush(QColor(255,0,0)));
        painter.drawRect(10, statusBarY + 7, 100*player->health, 6);
    }

    // Weapon boxes

    int boxx = 150;

    for(int i = 0; i < weaponColors.size(); ++i) {
        painter.setBrush(QBrush(weaponColors[i])); 
        
        if(player != NULL && player->weapon == i + 1) {
            painter.drawRect(boxx + i*25, statusBarY + 2, 20, 16);
        } else {
            painter.drawRect(boxx + i*25, statusBarY + 4, 20, 12);
        }
        painter.setBrush(QBrush(QColor(60,60,60))); 
        QString str = "";
        str += '1'+i;

        QFont newFont(painter.font());
        newFont.setPixelSize(10);
        painter.setFont(newFont);

        painter.drawText(QPoint(150 + i * 25 + 6, statusBarY + 13), str);
    }

    int index = player != NULL ? player->weapon - 1 : 0; 

    painter.setBrush(QBrush(QColor(60,60,60))); 
    painter.drawText(QPoint(150 + weaponColors.size() * 25 + 10, statusBarY + 14),
            weaponNames[index]);
}

int RenderArea::distance(QPoint a, QPoint b) {
    int dx = a.x() - b.x();
    int dy = a.y() - b.y();
    return (int)sqrt(dx*dx+dy*dy);
}

void RenderArea::drawLightning(QPainter& painter, QList<QPoint> points) {
    // 0 is the beginning
    QList<int> picked;
    picked.append(0);

    QList<QPair<int,int> > graph;
    
    while(picked.size() < points.size()) {
        int cheapest = (int)1e9;
        QPair<int,int> pr;
        for(int i = 0; i < picked.size(); ++i) {
            for(int j = 0; j < points.size(); ++j) {
                if(picked.contains(j)) continue;
                int dist = distance(points[picked[i]],points[j]);
                if(dist < cheapest) {
                    cheapest = dist;
                    pr = QPair<int,int>(picked[i],j);
                }
            }
        }
        graph.append(pr); 
        picked.append(pr.second);
    }

    painter.setBrush(QBrush(QColor(130,175,200))); 
    for(int i = 0; i < graph.size(); ++i) {
        painter.drawLine(points[graph[i].first],points[graph[i].second]);
    }
}

void RenderArea::draw(Player* player) {
    //qDebug()<<"Health: "<<player->health;
    this->player = player;
    centerx=player->x*SQUARE;
    centery=player->y*SQUARE;
    update();
}
